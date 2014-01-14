/*
 * ofxCvMotionTemplates.cpp
 *
 *   Author: Felix Lange | spta.de
 */
#include "ofxCvMotionTemplates.h"

ofxCvMotionTemplates::ofxCvMotionTemplates(int _width,int _height){
	width = _width;
	height = _height;

	//default values
	MHI_DURATION = 1;
	MAX_TIME_DELTA = 0.5;
	MIN_TIME_DELTA = 0.05;
	aperture_size = 3;
	threshold = 50;
	seg_threshold = MAX_TIME_DELTA; //TODO - see opencv learning book 346
	erodeIterations = 1;
	frameBufferSize = 4;
	minMotionArea = 100;
	isInit = false;
	isSilhouetteBufferActive = false;
}

void ofxCvMotionTemplates::setup(){

	lastIdx = 0;
	CvSize size = cvSize(width,height);
	buf = (IplImage**)malloc(frameBufferSize*sizeof(buf[0]));
	memset( buf, 0, frameBufferSize*sizeof(buf[0]));

	for(int i = 0; i < frameBufferSize; i++ ) {
			buf[i] = cvCreateImage( size, IPL_DEPTH_8U, 1 );
			cvZero( buf[i] );
	}

	mhi = cvCreateImage( size, IPL_DEPTH_32F, 1 );
	cvZero( mhi ); // clear MHI at the beginning
	orientation = cvCreateImage( size, IPL_DEPTH_32F, 1 );
	segmask = cvCreateImage( size, IPL_DEPTH_32F, 1 );
	mask = cvCreateImage( size, IPL_DEPTH_8U, 1 );
	motion = cvCreateImage( size, 8, 1 );
	silh = NULL;

	storage = cvCreateMemStorage(0);
	isInit = true;
}

void ofxCvMotionTemplates::clear(){

	//release images and clear storage
	for(int i=0; i < frameBufferSize; i++){
		cvReleaseImage(&(buf[i]));
		free(buf[i]);
	}
	free(buf);
	cvReleaseImage(&mhi);
	cvReleaseImage(&orientation);
	cvReleaseImage(&segmask);
	cvReleaseImage(&mask);
	cvReleaseImage(&motion);
	cvClearMemStorage(storage);

	isInit = false;
}

void ofxCvMotionTemplates::activateSilhouetteBuffer(int _bufferSize){
	if(isSilhouetteBufferActive && silhouetteBufferSize == _bufferSize){
		return;
	}else if(isSilhouetteBufferActive){
		//release images and clear storage
		for(int i=0; i < silhouetteBufferSize; i++){
			cvReleaseImage(&(silhouetteBuffer[i]));
			free(silhouetteBuffer[i]);
		}
		free(silhouetteBuffer);
	}
	if(_bufferSize < 1){
		return;
	}

	lastSilhIdx = -1;
	silhouetteBufferSize = _bufferSize;

	CvSize size = cvSize(width,height);
	silhouetteBuffer = (IplImage**)malloc(silhouetteBufferSize*sizeof(silhouetteBuffer[0]));
	memset( silhouetteBuffer, 0, silhouetteBufferSize*sizeof(silhouetteBuffer[0]));

	for(int i = 0; i < silhouetteBufferSize; i++ ) {
		silhouetteBuffer[i] = cvCreateImage( size, IPL_DEPTH_8U, 1 );
		cvZero( silhouetteBuffer[i] );
	}

	isSilhouetteBufferActive = true;
}

void ofxCvMotionTemplates::calculateBuffers(ofxCvGrayscaleImage & frame){
	int idx1, idx2;

	//FXTODO - memory leak? have to clear something here?
	cvCopyImage(frame.getCvImage(),buf[lastIdx]);

	idx1 = lastIdx;
	//index of (last - (N-1))th frame
	idx2 = (lastIdx + 1) % frameBufferSize;
	lastIdx = idx2;

	silh = buf[idx2];
	//get difference between frames
	cvAbsDiff( buf[idx1], buf[idx2], silh );
	cvErode(silh,silh,0,erodeIterations);

	//threshold the difference
	cvThreshold( silh, silh, threshold,255, CV_THRESH_BINARY ); // and threshold it

	if(isSilhouetteBufferActive){
		lastSilhIdx = (lastSilhIdx + 1) % silhouetteBufferSize;
		cvCopyImage(silh,silhouetteBuffer[lastSilhIdx]);
	}
}

IplImage* ofxCvMotionTemplates::calculateMotions(ofxCvGrayscaleImage & frame){
	if(!isInit){
		printf("ERROR - MotionTemplates have to be setup first!");
		return NULL;
	}
	if(!motions.empty()){
		motions.clear();
	}
	//Declarations
	double timestamp = (double)clock()/CLOCKS_PER_SEC; // get current time in seconds
	CvSeq* seq;
	CvRect comp_rect;
	double count;
	double angle;
	CvPoint center;
	double magnitude;
	CvScalar color2;


	calculateBuffers(frame);

	/** UPDATE THE MOTION HISTORY IMAGE(mhi)
	 * silh & old mhi -> new mhi
	 * any mhi pixels which are older than timestamp - duration are set to 0
	 * */
		cvUpdateMotionHistory( silh, mhi, timestamp, MHI_DURATION );

	/** Convert MHI to 8u greyscale image (motion)
	 * by scaling and shifting the old mask
	 */
		cvCvtScale( mhi, mask, 255./MHI_DURATION,
				(MHI_DURATION - timestamp)*255./MHI_DURATION );
		cvZero( motion );

		cvCopyImage( mask, motion );

	/** CALCULATE THE MOTION GRADIENTS
	 * mhi -> mask & orientation
	 * mask = valid gradients
	 * orientation = direction's angle
	 * min and max allowed gradient magnitude
	 * aperture_size = gradient filter (see declaration)
	 */
		cvCalcMotionGradient( mhi, mask, orientation, MIN_TIME_DELTA, MAX_TIME_DELTA, aperture_size );

	/** ISOLATE LOCAL MOTIONS
	 * mhi -> segmask & seg
	 * segmask = image in which each region of isolated motion is marked a distinct nonzero number
	 * seq = CvSeq* structures - one for each motion
	 * seg_tresh = maximum downward step that will be accepted as attached motion.
	 * 		there might be overlapping silhouettes from recent and much older motion tha you don't want to connect together
	 * */
		seq = cvSegmentMotion( mhi, segmask, storage, timestamp, seg_threshold );

	/** CALCULATE THE GLOBAL AND LOCAL MOTIONS
	 * */
		for( int i = -1; i < seq->total; i++ ) {
			// case of the whole image
			if( i < 0 ) {
				continue;
				comp_rect = cvRect( 0, 0, width, height );
				color2 = CV_RGB(255,255,255);
				magnitude = 100;
			}else { // i-th motion component
				comp_rect = ((CvConnectedComp*)cvGetSeqElem( seq, i ))->rect;
				if( (comp_rect.width + comp_rect.height)*2 < minMotionArea ) // reject very small components
					continue;
				color2 = CV_RGB(255,0,0);
				magnitude = 30;
			}

			// select component ROI
			cvSetImageROI( silh, comp_rect );
			cvSetImageROI( mhi, comp_rect );
			cvSetImageROI( orientation, comp_rect );
			cvSetImageROI( mask, comp_rect );

			// calculate orientation
			angle = cvCalcGlobalOrientation( orientation, mask, mhi, timestamp, MHI_DURATION);
			angle = 360.0 - angle;  // adjust for images with top-left origin

			// calculate number of points within silhouette ROI
			count = cvNorm( silh, 0, CV_L1, 0 );

			cvResetImageROI( mhi );
			cvResetImageROI( orientation );
			cvResetImageROI( mask );
			cvResetImageROI( silh );

			// check for the case of little motion
			float area = (float)(comp_rect.width*comp_rect.height);
			if( count < area * 0.25 )//FXTODO - revisit
				continue;

			center = cvPoint( (comp_rect.x + comp_rect.width/2),
							  (comp_rect.y + comp_rect.height/2) );


			ofxCvMotionBlob blob;
			blob.angle = (float)angle;
			blob.area = fabs(area);
			blob.boundingRect = ofRectangle(comp_rect.x,comp_rect.y,comp_rect.width,comp_rect.height);
			blob.centroid = ofPoint(center.x,center.y);
			blob.count = count;
			blob.hole = false;
			blob.pts.push_back(ofPoint(blob.boundingRect.x, blob.boundingRect.y));
			blob.pts.push_back(ofPoint(blob.boundingRect.x + blob.boundingRect.width, blob.boundingRect.y));
			blob.pts.push_back(ofPoint(blob.boundingRect.x + blob.boundingRect.width, blob.boundingRect.y + blob.boundingRect.height));
			blob.pts.push_back(ofPoint(blob.boundingRect.x, blob.boundingRect.y + blob.boundingRect.height));
			blob.nPts = 4;
			blob.w = width;
			blob.h  = height;

			motions.push_back(blob);
		}

	return motion;
}

void ofxCvMotionTemplates::setBlackToTransparent(ofImage & img){
	//TODO check if img.type == alpha
	unsigned char * pixels = img.getPixels();
	int pixelCount = 0;
	for(int x=0;x<img.getWidth()*4;x+=4){
		for(int y=0;y<img.getHeight();y++){
			int line = y*img.getWidth()*4;
			if(pixels[line+x]+pixels[line+x+1]+pixels[line+x+2]<10){
				pixels[line+x+3]=0;
			}
		}
	}
	img.update();
}
