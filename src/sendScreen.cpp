#define RVIP	// use the actual installed IP address

#include "sendScreen.h"
extern "C" {
#include "macGlutfix.h"
}

#include <GLUT/glut.h>


// Allow a portion of the MAC screen to be output as a pixmap via OSC
// Input window can be any size but will be SCALED DOWN via SCALE MODE to OUTPUT SIZE

// NOTE: Should support dynamic resizing

// Supports replicating output horizontally to drive both sides of RV
// i.e: [image#1][image#2]
// set replicateCol to desired replication column

// Process:
// Capture Window -> Texture 
// Render Texture at OUTPUT RESOLUTION
// Read rendered pixels and send

int multiple=15;	// ratio of output pixels to capture window size
int forceKey=1;		// Key Event Hack toggle
int snapshotMode=0;	// snapshot mode - just send one picture
int snapshot=0;		// trigger a snapshot
int dumpShotFlag=0;		// dump the image out as a structure

int interpModes[] = { CV_INTER_NN,
                      CV_INTER_LINEAR,
                      CV_INTER_AREA,
                      CV_INTER_CUBIC};

int interpIdx=0;
char interpStr[] = "NLAC";

void sendScreenApp::setup(){
    ofSetWindowTitle("SendScreen");

    capW = OUT_W*multiple;
    capH = OUT_H*multiple;

    // Capture and Output images
    capImg.allocate(capW, capH);
    outImg.allocate(OUT_W, OUT_H);

	// open an outbound OSC 
	sender.setup( OUT_HOST, OUT_PORT );
	cout << "Sending to " << OUT_HOST << "\n";

	ofBackground(255,255,255);
	ofSetFrameRate(30);
    AlwaysOnTop();

	/////// H / A / C / K /////////////////////////////////////////////////////////
    // Let's listen to some audio so we get called more often -- hackery!
    // toggle with '*'
    psnFlag = 0;
	ofSoundStreamSetup(0,2,this, 44100, 256, 4);	
	/////// H / A / C / K /////////////////////////////////////////////////////////
}

void setFromGL_RGBA(ofxCvColorImage *capImg, char* data, int capW, int capH){
    char* base = capImg->getCvImage()->imageData;
    for(int y=0; y<capH; y++){
        char *d = base + (y * capImg->getCvImage()->widthStep);
        for(int x=0; x<capW; x++){
#if 0
            // use intensity to scale
            int r=*data++;
            int g=*data++;
            int b=*data++;
            data++;
            //
            int m=max(r,max(g,b));
            float f = m/255.0;	// what is the brightest componant as a fraction of MAX
            *d++ = min(r*(1/f),255);
            *d++ = min(g*(1/f),255);
            *d++ = min(b*(1/f),255);
            *d++ = max(f*255, 255);	// and then scale it back using intensity
#else
            *d++=*data++;
            *d++=*data++;
            *d++=*data++;
            data++;
#endif
        }
    }
    capImg->flagImageChanged();
}

void copyToRGBA(ofxCvColorImage *img, char* data){
    // data points to uint32[w*h]

    int w=img->width;
    int h=img->height;

    char* base = img->getCvImage()->imageData;
    for(int y=0; y<h; y++){
        char *d = base + (y * img->getCvImage()->widthStep);
        for(int x=0; x<w; x++){
            *data++ = *d++;
            *data++ = *d++;
            *data++ = *d++;
            *data++ = 0xff;
        }
    }
}

void dumpShot(ofxCvColorImage *img){
    // output image a data structure
	cout << "//\n// Image\n//\n";

}

int now=0,then=0; int ms=0;

void sendScreenApp::update(){
    now = ofGetElapsedTimeMillis();
    ms = now-then;
    then = now;
}

void sendScreenApp::draw(){

    static int i=0;

    // i++; 	// will let us see update rate
	uint32 * data = pixelsBelowWindow(ofGetWindowPositionX()+(i%300),ofGetWindowPositionY(),capW,capH);
    
    // convert to GL_RGBA format
    if(data!=NULL){
        for (int i = 0; i < capW*capH; i++){
            // GL_RGBA = (uint32) AABBGGRR
            // NSImage = (uint32) BBGGRRAA
            data[i] = (data[i]>>8) | 0xff000000; 	// scoot down 8 bits - full alpha
        }
    }

    setFromGL_RGBA(&capImg, (char *) data, capW, capH);
    // draw what we captured
    capImg.draw(0,0);

    // Scale w/ chosen interpolation method
    // 
    // CV_INTER_NN     - nearest-neigbor interpolation,
    // CV_INTER_LINEAR - bilinear interpolation (used by default)
    // CV_INTER_AREA   - resampling using pixel area relation. It is preferred method
    //                   for image decimation that gives moire-free results. In case of
    //                   zooming it is similar to CV_INTER_NN method.
    // CV_INTER_CUBIC  - bicubic interpolation.

    outImg.scaleIntoMe( capImg, interpModes[interpIdx]);
    outImg.draw(0,0);	// draw this scaled down in a corner

    // send this image via OSC
    if(data!=NULL){
        if(snapshotMode && snapshot){
            sendImage(&outImg);
            snapshot=0;
        }
        if(!snapshotMode) sendImage(&outImg);
        if(dumpShotFlag){
            dumpShot(&outImg);
            dumpShotFlag=0;
        }
    }

    // draw # of ms since last update
	ofSetColor(0x00);
	char reportString[255];
	sprintf(reportString, "%c%c%2d", interpStr[interpIdx], forceKey?'*':' ',ms);
	ofDrawBitmapString(reportString,1,ofGetHeight()-10);
    ofDrawBitmapString(snapshotMode?
                       "SNAPSHOT MODE - press SPACE to Capture and Send - \"C\" key for continuous":
                       "CONTINUOUS MODE - press \"S\" key for SNAPSHOT", 1, ofGetHeight()-25);
	ofSetColor(0xffffffff);

}

void sendScreenApp::audioReceived 	(float * input, int bufferSize, int nChannels){	
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

void sendScreenApp::sendImage(ofxCvColorImage *img){
    // output 4 channels 
    uint32 outbuf[OUT_H][OUT_W];
    copyToRGBA(img,(char *)outbuf);

    ofxOscMessage m;
    m.setAddress( "/screen" );
    m.addIntArg( OUT_H );
    m.addIntArg( OUT_W );
    m.addBlobArg(outbuf, sizeof(outbuf));
    sender.sendRawMessage( m );
}

//--------------------------------------------------------------
void sendScreenApp::keyPressed(int key){
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

    if(key=='i' || key=='I'){
        interpIdx = ++interpIdx % 4;
    }

    if(key=='c' || key=='C') snapshotMode=0;
    if(key=='s' || key=='S') snapshotMode=1;
    if(key=='d' || key=='D') dumpShotFlag=1;
    if(key=='+' || key=='=') {
        if(++multiple > 100) multiple=100;
        glutReshapeWindow(OUT_W*multiple,OUT_H*multiple);
    }
    if(key=='-' || key=='_') {
        if(--multiple < 1) multiple=1;
        glutReshapeWindow(OUT_W*multiple,OUT_H*multiple);
    }
    if(key==' ') snapshot=1;
}

//--------------------------------------------------------------
void sendScreenApp::keyReleased(int key){

}

//--------------------------------------------------------------
void sendScreenApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void sendScreenApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void sendScreenApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void sendScreenApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void sendScreenApp::windowResized(int w, int h){
    // Force Aspect Ratio to be constant
    // test - see what happens if we keep W constant
	// glutReshapeWindow(INIT_W, h);
}

