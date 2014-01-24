#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCvMotionTemplates.h"

#pragma once

class silhouettesApp : public ofBaseApp{

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
        ofxCvGrayscaleImage 	grayImg;
        ofxCvGrayscaleImage		silhouetteImg, smallSilhouetteImg;

        ofImage transSilhouetteImg,transSilhouetteImg2;

        bool live;

        int inputWidht, inputHeight, smallW, smallH;
};
