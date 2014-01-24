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
	silhouetteImg.allocate(inputWidht,inputHeight);
	smallSilhouetteImg.allocate(smallW,smallH);

	motionHistory->activateSilhouetteBuffer(10);
}

//--------------------------------------------------------------
void silhouettesApp::update(){
	vidPlayer.update();
	bool bNewFrame = vidPlayer.isFrameNew();
	if(bNewFrame){
		colorImg.setFromPixels(vidPlayer.getPixels(), inputWidht,inputHeight);

		grayImg = colorImg;
		motionHistory->calculateBuffers(grayImg);

		silhouetteImg = motionHistory->getBufferedSilhouetteImg(2);
		smallSilhouetteImg.scaleIntoMe(silhouetteImg);
		transSilhouetteImg.setFromPixels(smallSilhouetteImg.getPixels(),smallW,smallH,OF_IMAGE_GRAYSCALE);
		transSilhouetteImg.setImageType(OF_IMAGE_COLOR_ALPHA);
		setBlackToTransparent(transSilhouetteImg);

		silhouetteImg = motionHistory->getBufferedSilhouetteImg(5);
		smallSilhouetteImg.scaleIntoMe(silhouetteImg);
		transSilhouetteImg2.setFromPixels(smallSilhouetteImg.getPixels(),smallW,smallH,OF_IMAGE_GRAYSCALE);
		transSilhouetteImg2.setImageType(OF_IMAGE_COLOR_ALPHA);
		setBlackToTransparent(transSilhouetteImg2);//TODO should be static
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
	transSilhouetteImg2.draw(0,smallH);
	ofSetColor(35,255,25,200);
	transSilhouetteImg.draw(0,smallH);
	ofDisableAlphaBlending();

	ofSetColor(50,50,50);
	ofFill();
	ofRect(0,480,smallW,smallH);
	ofEnableAlphaBlending();
	ofSetColor(255,35,48,255);
	transSilhouetteImg2.draw(0,480);
	ofSetColor(35,255,25,200);
	transSilhouetteImg.draw(0,480);
	ofDisableAlphaBlending();
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
