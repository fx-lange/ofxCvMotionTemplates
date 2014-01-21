#ifndef OFXCVMOTIONBLOB_H_
#define OFXCVMOTIONBLOB_H_

#include "ofxOpenCv.h"

class ofxCvMotionBlob : public ofxCvBlob{
public:
	float angle;
	int count;
	int w, h;
	ofVec2f forceDir;

	void draw(int x,int y,int _width=0,int _height=0);
};

#endif /* OFXCVMOTIONBLOB_H_ */
