/*
 * ofxCvMotionTemplates.h
 *
 *   Author: Felix Lange | spta.de
 */
#pragma once

#include "ofxOpenCv.h"

class ofxCvMotionBlob : public ofxCvBlob{
public:
	float angle;
	int count;
	int w, h;

	void setWH(int _w,int _h){
		w = _w,h = _h;
	}


	void draw(int x,int y,int _width=0,int _height=0){



		float scalex = 0.0f;
		float scaley = 0.0f;

		if( _width != 0 ) { scalex = w/_width; } else { scalex = 1.0f; }
		if( _height != 0 ) { scaley = h/_height; } else { scaley = 1.0f; }

		ofPushMatrix();
//		ofTranslate(x,y);
//		glScaled(scalex, scaley, 0.0 );
		ofNoFill();
		ofSetColor(10,25,255);
		ofBeginShape();
		for(int i=0;i<nPts;i++){
			ofVertex(pts[i].x,pts[i].y);
		}
		ofEndShape(true);
		double magnitude =  (count/area/5);
		ofSetColor(10,255,25);
		ofEllipse(centroid.x,centroid.y,round(magnitude*1.2),round(magnitude*1.2));
		ofLine(centroid.x,centroid.y,
				round(centroid.x + magnitude*cos(angle*CV_PI/180)),
				round(centroid.y + magnitude*sin(angle*CV_PI/180)));
		ofPopMatrix();
	}
};

class ofxCvMotionTemplates{

private:

	int frameBufferSize;			// number of cyclic frame buffer used for motion detection
									// (should, probably, depend on FPS)
									// to change N requires a reset of the frame buffer

	int 	lastIdx;				//points to the current index in the frame buffer

	IplImage 	**buf,				// image ring  buffer
				*mhi,				// motion template (aka motion history image) - updatet
				*segmask,			// image in which each region of isolated motion is marked a distinct nonzero number
				*mask,				// valid gradient orientation mask - nonzero values indicates valid gradients
				*orientation,		// floating-point image that gives the gradient direction's angle at each point
				*motion,			// motion output image
				*silh;


	/**silhouette ringbuffer
	 * size, active, pointer, counter
	 * perhaps silhouette buffer should contain IplImages instead of ofxCvfGrayscales
	 */
	IplImage ** silhouetteBuffer;			//silhouette image ring buffer - used for visual output
	int 		silhouetteBufferSize,
				lastSilhIdx;	//size and pointer to the current index
	bool isSilhouetteBufferActive;




	CvMemStorage * storage;			//openCv intern memory storage

	vector <ofxCvMotionBlob> motions;

	int width, height;
	bool 	verbose,
			isInit;

	void clear();
	void setup();

public:

	float 	MHI_DURATION;			// sets how long motion history pixel are allowed to remain in the mhi
	int		minMotionArea;			// motions with smaller area get ignored
	int 	threshold;				// threshold for the silhouette
	int 	aperture_size;			// size of the gradient operator
										// -1 => 3x3 CV_SCHARR gradient filter
										// 3,5,7 => default Sobel filter (3x3,5x5 or 7x7)
	float 	MIN_TIME_DELTA;			// minimal and ..
	float 	MAX_TIME_DELTA;			// ... maximal gradient magnitude allowed
										// halfway below and halfway above the average number of time-stamp ticks between each
										// ... silhouette in successive calls to cvUpdateMotionHistory should work well
	double seg_threshold;				// maximum downward step (from current time to previous motion)
										// ... that you'll accept as attached motion.

	int erodeIterations, erodeKernelSize;

	ofxCvMotionTemplates(int _width, int _height);

	~ofxCvMotionTemplates(){
		//FXTODO delete[] silhBuffer - memoryleak
	}

	void setFrameBufferSize(int _N){
		clear();
		frameBufferSize = _N;
		setup();
	}

	int getFrameBufferSize(){
		return frameBufferSize;
	}

	IplImage * getBufferedSilhouetteImg(int negativeIndex){
		if(isSilhouetteBufferActive){
			int idx = (lastSilhIdx-negativeIndex) % silhouetteBufferSize;
			idx = (idx+silhouetteBufferSize) % silhouetteBufferSize;
			return silhouetteBuffer[idx];
		}else{
			return NULL;
		}
	}

	IplImage* getMotionImage(){
		if(!isInit){
			printf("ERROR - ofxCvMotionTemplates have to be setup first!");
			return NULL;
		}
		return motion;
	}

	vector <ofxCvMotionBlob> & getLocalMotions(){
		return motions;
	}

	void activateSilhouetteBuffer(int _bufferSize);

	IplImage* calculateMotions(ofxCvGrayscaleImage & frame);
	void calculateBuffers(ofxCvGrayscaleImage & frame);

	void setBlackToTransparent(ofImage & img);
};
