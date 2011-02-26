#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"

#define OF_ADDON_USING_OFXOSC
#include "ofAddons.h"

#define INIT_H 9
#define INIT_W 13

// #define INIT_H 40
// #define INIT_W 80

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

class testApp : public ofBaseApp{

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
        void sendImage(uint* data, int w, int h);

        // display texture
        ofTexture	tex;

        // audio "forced I/O" hack
        void audioReceived 	(float * input, int bufferSize, int nChannels);
        int psnFlag;
        ProcessSerialNumber psn;

        // misc debug
		int 	bufferCounter, drawCounter, updateCounter;

};

#endif
