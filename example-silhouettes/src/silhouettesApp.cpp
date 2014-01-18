#include "silhouettesApp.h"

//--------------------------------------------------------------
void silhouettesApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	vidPlayer.loadMovie("wink3.mp4");
	inputWidht = vidPlayer.getWidth();
	inputHeight = vidPlayer.getHeight();
	smallW  = 320;
	smallH = 240;

    vidPlayer.play();
    vidPlayer.setLoopState(OF_LOOP_NORMAL);

	motionHistory = new ofxCvMotionTemplates(inputWidht,inputHeight);
	motionHistory->setup();

	colorImg.allocate(inputWidht,inputHeight);
	grayImg.allocate(inputWidht,inputHeight);
	silhuetteImg.allocate(inputWidht,inputHeight);
	smallSilhuetteImg.allocate(smallW,smallH);
	motionImg.allocate(inputWidht,inputHeight);

	motionHistory->activateSilhouetteBuffer(10);
}

//--------------------------------------------------------------
void silhouettesApp::update(){
	vidPlayer.update();
	bool bNewFrame = vidPlayer.isFrameNew();
	if(bNewFrame){
		colorImg.setFromPixels(vidPlayer.getPixels(), inputWidht,inputHeight);

		grayImg = colorImg;
		motionImg = motionHistory->calculateMotions(grayImg);

		silhuetteImg = motionHistory->getBufferedSilhouetteImg(2);
		smallSilhuetteImg.scaleIntoMe(silhuetteImg);
		transSilhuetteImg.setFromPixels(smallSilhuetteImg.getPixels(),smallW,smallH,OF_IMAGE_GRAYSCALE);
		transSilhuetteImg.setImageType(OF_IMAGE_COLOR_ALPHA);
		setBlackToTransparent(transSilhuetteImg);

		silhuetteImg = motionHistory->getBufferedSilhouetteImg(5);
		smallSilhuetteImg.scaleIntoMe(silhuetteImg);
		transSilhuetteImg2.setFromPixels(smallSilhuetteImg.getPixels(),smallW,smallH,OF_IMAGE_GRAYSCALE);
		transSilhuetteImg2.setImageType(OF_IMAGE_COLOR_ALPHA);
		setBlackToTransparent(transSilhuetteImg2);//wird als ofx funktion benötigt
	}
}

void silhouettesApp::setBlackToTransparent(ofImage & img){
	unsigned char * pixels = img.getPixels();
	int pixelCount = 0;
	for(int x=0;x<smallW*4;x+=4){
		for(int y=0;y<smallH;y++){
			int line = y*smallW*4;
			if(pixels[line+x]+pixels[line+x+1]+pixels[line+x+2]<10){
				pixels[line+x+3]=0;
			}
		}
	}
	img.update();
}

//--------------------------------------------------------------
void silhouettesApp::draw(){
	ofSetColor(255,255,255);
	colorImg.draw(0,0,smallW,smallH);
	grayImg.draw(0,smallH,smallW,smallH);
	ofEnableAlphaBlending();
	ofSetColor(255,35,48,100);
	transSilhuetteImg2.draw(0,smallH);
	ofSetColor(35,255,25,200);
	transSilhuetteImg.draw(0,smallH);
	ofDisableAlphaBlending();

	ofSetColor(50,50,50);
	ofFill();
	ofRect(0,480,smallW,smallH);
	ofEnableAlphaBlending();
	ofSetColor(255,35,48,255);
	transSilhuetteImg2.draw(0,480);
	ofSetColor(35,255,25,200);
	transSilhuetteImg.draw(0,480);
	ofDisableAlphaBlending();

	ofSetColor(255,35,48);
	motionImg.draw(smallW,0);
	vector <ofxCvMotionBlob> & motions = motionHistory->getLocalMotions();
	ofSetColor(255,255,255);
	for(int i=0;i < motions.size(); i++){
		motions[i].draw(smallW,0);
	}
}

//--------------------------------------------------------------
void silhouettesApp::keyPressed(int key){
	int N = motionHistory->getFrameBufferSize();
	motionHistory->setFrameBufferSize(N+1);
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
