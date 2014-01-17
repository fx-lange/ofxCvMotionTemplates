#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCvMotionTemplates.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

		void setBlackToTransparent(ofImage & img);

		ofVideoPlayer 		vidPlayer;

		ofxCvMotionTemplates * 	motionHistory;

        ofxCvColorImage			colorImg;
        ofxCvColorImage			smallColorImg;
        ofxCvGrayscaleImage		motionImg;
        ofxCvGrayscaleImage 	grayImage;
        ofxCvGrayscaleImage		smallGrayImg;
        ofxCvGrayscaleImage		silhuetteImg;
        ofxCvGrayscaleImage		smallSilhuetteImg;

        ofImage transSilhuetteImg,transSilhuetteImg2;

        bool live;

        int inputWidht, inputHeight, smallW, smallH;

};

#endif
