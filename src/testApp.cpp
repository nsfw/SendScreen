#define RVIP	// use the actual installed IP address

#include "testApp.h"

extern "C" {
#include "macGlutfix.h"
}

#include <GLUT/glut.h>

int forceKey=1;

void testApp::setup(){
	
    ofSetWindowTitle("SendScreen");
	tex.allocate(INIT_W, INIT_H, GL_RGBA);
	cout << "Sending to " << OUT_HOST << "\n";
	
	// open an outbound OSC 
	sender.setup( OUT_HOST, OUT_PORT );

    // stuff that would be in constructor
    capW = INIT_W;
    capH = INIT_H;

    // dbg stuff
	bufferCounter = 0;
	drawCounter = 0;
    updateCounter = 0;

    AlwaysOnTop();

    // Let's listen to some audio so we get called more often -- hackery!
    // toggle with '*'
    psnFlag = 0;
	ofSoundStreamSetup(0,2,this, 44100, 256, 4);	

	ofBackground(255,255,255);
	ofSetFrameRate(30);
}

int now=0,then=0; int ms=0;

void testApp::update(){
    now = ofGetElapsedTimeMillis();
    ms = now-then;
    then = now;

    updateCounter++;
}

void testApp::draw(){

    static int i=0;

    // i=0; // don't animate
	uint32 * data = pixelsBelowWindow(ofGetWindowPositionX()+(i++%300),ofGetWindowPositionY(),capW,capH);
    
    // convert to GL_RGBA format
    if(data!=NULL){
        for (int i = 0; i < capW*capH; i++){
            // GL_RGBA = (uint32) AABBGGRR
            // NSImage = (uint32) BBGGRRAA
            data[i] = (data[i]>>8) | 0xff000000; 	// scoot down 8 bits - full alpha
        }
        tex.loadData((unsigned char *) data, capW, capH, GL_RGBA);
    }

    // send this image via OSC
    static int justonce=0;
    if(data!=NULL && !justonce) sendImage(data, capH, capW);
    justonce=1;

    // draw what we captured
	tex.draw(0,0, ofGetWidth(), ofGetHeight());
    drawCounter++;
	
    // draw # of ms since last update
	ofSetColor(0x00);
	char reportString[255];
	sprintf(reportString, "%c%2d", forceKey?'*':' ',ms);
	ofDrawBitmapString(reportString,1,8);
	ofSetColor(0xffffffff);

}

void testApp::audioReceived 	(float * input, int bufferSize, int nChannels){	
    // not actually receiving audio, just forcing some I/O
	bufferCounter++;

    // hackery! - works! But really?
    if(!psnFlag){
        GetFrontProcess(&psn);
        psnFlag=1;
    }
    if(forceKey){
        CGEventRef e = CGEventCreateKeyboardEvent (NULL, (CGKeyCode)0, true);
        CGEventPostToPSN (&psn,e);
        CFRelease(e);
    }
}

void testApp::sendImage(uint* data, int h, int w){
    ofxOscMessage m;
    m.setAddress( "/screen" );
    m.addIntArg( h );
    m.addIntArg( w );
    m.addBlobArg(data, w*h*4 );		// send blob in BYTES
    sender.sendRawMessage( m );
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key>=OF_KEY_LEFT && key <=OF_KEY_DOWN){
        // OF_KEY_LEFT, OF_KEY_UP, OF_KEY_RIGHT, OF_KEY_DOWN
        int deltas[][2]={{-1,0},{0,-1},{1,0},{0,1}};
        int k = key-OF_KEY_LEFT;
        ofSetWindowPosition(ofGetWindowPositionX()+deltas[k][0],
                            ofGetWindowPositionY()+deltas[k][1]);
    }
    if(key=='*'){
        // toggle keyboard hack
        forceKey = !forceKey;
    }
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

