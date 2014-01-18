#include "motionApp.h"

//--------------------------------------------------------------
void motionApp::setup(){
	vidPlayer.loadMovie("wink3.mp4");
	inputWidht = vidPlayer.getWidth();
	inputHeight = vidPlayer.getHeight();

    vidPlayer.play();
    vidPlayer.setLoopState(OF_LOOP_NORMAL);

	motionHistory = new ofxCvMotionTemplates(inputWidht,inputHeight);
	motionHistory->setup();

	colorImg.allocate(inputWidht,inputHeight);
	grayImg.allocate(inputWidht,inputHeight);
	motionImg.allocate(inputWidht,inputHeight);
}

//--------------------------------------------------------------
void motionApp::update(){
	vidPlayer.update();
	bool bNewFrame = vidPlayer.isFrameNew();
	if(bNewFrame){
		colorImg.setFromPixels(vidPlayer.getPixels(), inputWidht,inputHeight);
	}
	grayImg = colorImg;
	motionImg = motionHistory->calculateMotions(grayImg);
}

//--------------------------------------------------------------
void motionApp::draw(){
	ofSetColor(255,255,255);
	colorImg.draw(0,0,320,240);
	grayImg.draw(0,240,320,240);
	ofSetColor(0,25,225);
	motionImg.draw(320,0);
	vector <ofxCvMotionBlob> & motions = motionHistory->getLocalMotions();
	for(int i=0;i < (int)motions.size(); i++){
		motions[i].draw(320,0);
	}
}

//--------------------------------------------------------------
void motionApp::keyPressed(int key){
	int N = motionHistory->getFrameBufferSize();
	motionHistory->setFrameBufferSize(N+1);
}

//--------------------------------------------------------------
void motionApp::keyReleased(int key){

}

//--------------------------------------------------------------
void motionApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void motionApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void motionApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void motionApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void motionApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void motionApp::exit(){

}
