#ifndef _SENDSCREEN_APP
#define _SENDSCREEN_APP

#include "ofMain.h"

// OUTPUT RESOLUTION
#define OUT_W 30
#define OUT_H 9
//// Initial window size
#define INIT_MULT 15
//#define INIT_W (OUT_W*15)
//#define INIT_H (OUT_H*15)

// deprecated...
// #define OF_ADDON_USING_OFXOSC
// #define OF_ADDON_USING_OFXOPENCV
// #include "ofAddons.h"

#include "ofxOsc.h"
#include "ofxOpenCv.h"

#ifdef MC
#define OUT_HOST "239.192.192.192"	// multicast
#define OUT_PORT 9192
#endif
#ifdef DIRECT_CONNECT
#define OUT_HOST "198.178.187.122"
#define OUT_PORT 9999
#endif
#ifdef RVIP
#define OUT_HOST "192.168.69.69"
#define OUT_PORT 9999
#endif

class sendScreenApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

        int capW, capH;		// capture width and height in pixels

        // OSC
		ofxOscReceiver	receiver;
		ofxOscSender	sender;

        void sendImage(ofxCvColorImage*);

        ofxCvColorImage		capImg;
        ofxCvColorImage		outImg;

        // audio "forced I/O" hack
        void audioReceived 	(float * input, int bufferSize, int nChannels);
        int psnFlag;
        ProcessSerialNumber psn;

};

#endif
