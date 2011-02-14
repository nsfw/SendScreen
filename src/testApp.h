#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"

#define INIT_H 90
#define INIT_W 300

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

        int capW, capH;

        // audio "forced I/O" hack
        void audioReceived 	(float * input, int bufferSize, int nChannels);
        int psnFlag;
        ProcessSerialNumber psn;

        // misc
		int 	bufferCounter, drawCounter, updateCounter;

        // display texture
        ofTexture	tex;
};

#endif
