#include "ofMain.h"
#include "sendScreen.h"
#include "ofAppGlutWindow.h"

int main( ){

    ofAppGlutWindow window;

    // for output echo of screen capture area
	ofSetupOpenGL(&window, INIT_W,INIT_H, OF_WINDOW);			// <-------- setup the GL context

	// pass in width and height too:
	ofRunApp( new sendScreenApp());
}
