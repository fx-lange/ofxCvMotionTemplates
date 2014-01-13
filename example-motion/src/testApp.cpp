#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	vidPlayer.loadMovie("wink3.mp4");
	inputWidht = vidPlayer.getWidth();
	inputHeight = vidPlayer.getHeight();

    vidPlayer.play();
    vidPlayer.setLoopState(OF_LOOP_NORMAL);

	motionHistory = new ofxCvMotionTemplates(inputWidht,inputHeight);

	colorImg.allocate(inputWidht,inputHeight);
	grayImage.allocate(inputWidht,inputHeight);
	smallColorImg.allocate(320,240);
	smallGrayImg.allocate(320,240);
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
	smallColorImg.scaleIntoMe(colorImg);
	smallGrayImg.scaleIntoMe(grayImage);
	motionImg = motionHistory->calculateMotions(grayImage);
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(255,255,255);
	smallColorImg.draw(0,0);
	smallGrayImg.draw(0,240);
	motionImg.draw(320,0);
	vector <ofxCvMotionBlob> & motions = motionHistory->getLocalMotions();
	for(int i=0;i < motions.size(); i++){
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
