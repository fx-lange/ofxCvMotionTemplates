#include "ofxCvMotionBlob.h"

void ofxCvMotionBlob::draw(int x,int y,int _width,int _height){
	float scalex = 0.0f;
	float scaley = 0.0f;

	if( _width != 0 ) { scalex = w/_width; } else { scalex = 1.0f; }
	if( _height != 0 ) { scaley = h/_height; } else { scaley = 1.0f; }

	ofPushMatrix();
	ofTranslate(x,y);
	ofScale(scalex, scaley, 0.0 );
	ofNoFill();
	ofSetColor(10,25,255);
	ofBeginShape();
	for(int i=0;i<nPts;i++){
		ofVertex(pts[i].x,pts[i].y);
	}
	ofEndShape(true);
	double magnitude =  (count/area/5);
	ofSetColor(10,255,25);
	ofEllipse(centroid.x,centroid.y,round(magnitude*1.2),round(magnitude*1.2));
	ofLine(centroid.x,centroid.y,
			round(centroid.x + magnitude*cos(angle*CV_PI/180)),
			round(centroid.y + magnitude*sin(angle*CV_PI/180)));
	ofPopMatrix();
}

