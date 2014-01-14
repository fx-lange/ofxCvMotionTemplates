#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	vidPlayer.loadMovie("wink3.mp4");
	inputWidht = vidPlayer.getWidth();
	inputHeight = vidPlayer.getHeight();

    vidPlayer.play();
    vidPlayer.setLoopState(OF_LOOP_NORMAL);

	motionHistory = new ofxCvMotionTemplates(inputWidht,inputHeight);
	motionHistory->setup();

	colorImg.allocate(inputWidht,inputHeight);
	grayImage.allocate(inputWidht,inputHeight);
	motionImg.allocate(inputWidht,inputHeight);
}

//--------------------------------------------------------------
void testApp::update(){
	vidPlayer.update();
	bool bNewFrame = vidPlayer.isFrameNew();
	if(bNewFrame){
		colorImg.setFromPixels(vidPlayer.getPixels(), inputWidht,inputHeight);
	}
	grayImage = colorImg;
	motionImg = motionHistory->calculateMotions(grayImage);
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(255,255,255);
	colorImg.draw(0,0,320,240);
	grayImage.draw(0,240,320,240);
	ofSetColor(0,25,225);
	motionImg.draw(320,0);
	vector <ofxCvMotionBlob> & motions = motionHistory->getLocalMotions();
	for(int i=0;i < (int)motions.size(); i++){
		motions[i].draw(320,0);
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	int N = motionHistory->getFrameBufferSize();
	motionHistory->setFrameBufferSize(N+1);
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::exit(){

}
