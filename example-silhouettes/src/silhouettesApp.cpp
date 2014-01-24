#include "silhouettesApp.h"

//--------------------------------------------------------------
void silhouettesApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofEnableAlphaBlending();

	vidPlayer.loadMovie("wink3.mp4");
	inputWidth = vidPlayer.getWidth();
	inputHeight = vidPlayer.getHeight();
	smallW  = inputWidth/2.f;
	smallH = inputHeight/2.f;

    vidPlayer.play();
    vidPlayer.setLoopState(OF_LOOP_NORMAL);

	motionHistory = new ofxCvMotionTemplates(inputWidth,inputHeight);
	motionHistory->setup();

	colorImg.allocate(inputWidth,inputHeight);
	grayImg.allocate(inputWidth,inputHeight);
	silhouetteImg.allocate(inputWidth,inputHeight);

	motionHistory->activateSilhouetteBuffer(10);
}

//--------------------------------------------------------------
void silhouettesApp::update(){
	vidPlayer.update();
	bool bNewFrame = vidPlayer.isFrameNew();
	if(bNewFrame){
		colorImg.setFromPixels(vidPlayer.getPixels(), inputWidth,inputHeight);

		grayImg = colorImg;
		motionHistory->calculateBuffers(grayImg);

		silhouetteImg = motionHistory->getBufferedSilhouetteImg(2);
		transSilhouetteImg.setFromPixels(silhouetteImg.getPixelsRef());
		transSilhouetteImg.setImageType(OF_IMAGE_COLOR_ALPHA);
		ofxCvMotionTemplates::setBlackToTransparent(transSilhouetteImg);

		silhouetteImg = motionHistory->getBufferedSilhouetteImg(5);
		transSilhouetteImg2.setFromPixels(silhouetteImg.getPixelsRef());
		transSilhouetteImg2.setImageType(OF_IMAGE_COLOR_ALPHA);
		ofxCvMotionTemplates::setBlackToTransparent(transSilhouetteImg2);//TODO should be static
	}
}

//--------------------------------------------------------------
void silhouettesApp::draw(){
	ofSetColor(255,255,255);
	colorImg.draw(0,0,smallW,smallH);

	grayImg.draw(smallW,0);
	ofSetColor(255,35,48,100);
	transSilhouetteImg2.draw(smallW,0);
	ofSetColor(35,255,25,200);
	transSilhouetteImg.draw(smallW,0);

	ofSetColor(50,50,50);
	ofFill();
	ofRect(0,smallH,smallW,smallH);
	ofSetColor(255,35,48,255);
	transSilhouetteImg2.draw(0,smallH,smallW,smallH);
	ofSetColor(35,255,25,200);
	transSilhouetteImg.draw(0,smallH,smallW,smallH);
}

//--------------------------------------------------------------
void silhouettesApp::keyPressed(int key){
}

//--------------------------------------------------------------
void silhouettesApp::keyReleased(int key){

}

//--------------------------------------------------------------
void silhouettesApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void silhouettesApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void silhouettesApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void silhouettesApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void silhouettesApp::windowResized(int w, int h){
}

//--------------------------------------------------------------
void silhouettesApp::exit(){
	vidPlayer.stop();
	ofLogVerbose("exit");
}
