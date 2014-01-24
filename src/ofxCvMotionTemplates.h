#pragma once

#include "ofxCvMotionBlob.h"

#define USE_OFXGUI_FOR_MT
#ifdef USE_OFXGUI_FOR_MT
#include "ofxGui.h"
#endif

class ofxCvMotionTemplates{
public:
	ofxCvMotionTemplates(int _width, int _height);

	~ofxCvMotionTemplates(){
		clear();
	}

	void setup();
	void clear();

	void setFrameBufferSize(int _N);
	int getFrameBufferSize();

	void calculateMotions(ofxCvGrayscaleImage & frame);
	void calculateBuffers(ofxCvGrayscaleImage & frame);

	void activateSilhouetteBuffer(int _bufferSize);
	IplImage * getBufferedSilhouetteImg(int negativeIndex);

	static void setBlackToTransparent(ofImage & img);

	IplImage * getMotionImage();
	vector <ofxCvMotionBlob> & getLocalMotions();

	int erodeIterations, erodeKernelSize;

	#ifdef USE_OFXGUI_FOR_MT //parameter documentation below
    ofxPanel gui;
    ofxFloatSlider MHI_DURATION, minMotionArea, threshold;
    ofxFloatSlider MIN_TIME_DELTA, MAX_TIME_DELTA, seg_threshold, minCountPerArea;
    ofxIntSlider aperture_size;

	#else
	float 	MHI_DURATION;			// sets how long motion history pixel are allowed to remain in the mhi
	float	minMotionArea;			// motions with smaller area get ignored
	float 	threshold;				// threshold for the silhouette
	int 	aperture_size;			// size of the gradient operator
										// -1 => 3x3 CV_SCHARR gradient filter
										// 3,5,7 => default Sobel filter (3x3,5x5 or 7x7)
	float 	MIN_TIME_DELTA;			// minimal and ..
	float 	MAX_TIME_DELTA;			// ... maximal gradient magnitude allowed
										// halfway below and halfway above the average number of time-stamp ticks between each
										// ... silhouette in successive calls to cvUpdateMotionHistory should work well
	double seg_threshold;				// maximum downward step (from current time to previous motion)
										// ... that you'll accept as attached motion. (1.5 times avg. difference between timestamps)

	float minCountPerArea;
	#endif

protected:
	#ifdef USE_OFXGUI_FOR_MT
	void setupGui();
	#endif

	int frameBufferSize;			// number of cyclic frame buffer used for motion detection
									// (should, probably, depend on FPS)
									// to change N requires a reset of the frame buffer

	int 	lastIdx;				//points to the current index in the frame buffer

	IplImage 	**buf,				// image ring  buffer
				*mhi,				// motion template (aka motion history image) - updated
				*segmask,			// image in which each region of isolated motion is marked a distinct nonzero number
				*mask,				// valid gradient orientation mask - nonzero values indicates valid gradients
				*orientation,		// floating-point image that gives the gradient direction's angle at each point
				*motion,			// motion output image
				*silh;


	//silhouette ring buffer - used for visual/debug output
	IplImage ** silhouetteBuffer;
	int 		silhouetteBufferSize,
				lastSilhIdx;	//size and pointer to the current index
	bool isSilhouetteBufferActive;


	CvMemStorage * storage;			//openCv intern memory storage

	vector<ofxCvMotionBlob> motions;

	int width, height;
	bool 	verbose,
			isInit;

	double lastTimestamp;
};
