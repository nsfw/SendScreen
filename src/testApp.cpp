#include "testApp.h"

extern "C" {
#include "macGlutfix.h"
}

#include <GLUT/glut.h>

int t=0;

void idle(){
    t++;
}

void idle2(int i){
    t+=i;
}


//--------------------------------------------------------------
void testApp::setup(){
	
    ofSetWindowTitle("SendScreen Window");

	//CGContextRef cgctx = NULL;
	// ofSetVerticalSync(true);
	tex.allocate(300,300, GL_RGBA);

	ofSetFrameRate(60);

	
	bufferCounter = 0;
	drawCounter = 0;
    updateCounter = 0;
    psnFlag = 0;

    // OK - let's listen to some audio so we get called more often
	ofSoundStreamSetup(0,2,this, 44100, 256, 4);	
}

int now=0,then=0; int ms=0;

void testApp::update(){
    now = ofGetElapsedTimeMillis();
    ms = now-then;
    then = now;

    updateCounter++;
}

//--------------------------------------------------------------

void testApp::draw(){

	int w = 300;
	int h = 300;

    static int i=0;
    	
	uint32 * data = pixelsBelowWindow(ofGetWindowPositionX()+(i++%300),ofGetWindowPositionY(),w,h);
    // convert to GL_RGBA format
    if(data!=NULL){
        for (int i = 0; i < w*h; i++){
            // GL_RGBA = (uint32) AABBGGRR
            // NSImage = (uint32) BBGGRRAA
            data[i] = (data[i]>>8) | 0xff000000; 	// scoot down 8 bits - full alpha
        }
        tex.loadData((unsigned char *) data, 300, 300, GL_RGBA);
    }

    // cover the whole screen area
	tex.draw(0,0, ofGetWidth(), ofGetHeight());
    drawCounter++;

	// ofSetColor(0x0);
	// char reportString[255];
	// sprintf(reportString, "MS: %05i,%05i,%05i,%05i", ms, bufferCounter,drawCounter,updateCounter);
	// ofDrawBitmapString(reportString,40,40);
	// ofSetColor(0xffffffff);

}

void testApp::audioReceived 	(float * input, int bufferSize, int nChannels){	
    // not actually receiving audio, just forcing some I/O
	bufferCounter++;

    // hackery! - works! But really?
    if(!psnFlag){
        GetFrontProcess(&psn);
        psnFlag=1;
    }
    CGEventRef e = CGEventCreateKeyboardEvent (NULL, (CGKeyCode)83, true);
    CGEventPostToPSN (&psn,e);
    CFRelease(e);
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){

	
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

