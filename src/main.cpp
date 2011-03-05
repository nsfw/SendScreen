#include "ofMain.h"
#include "sendScreen.h"
#include "ofAppGlutWindow.h"

int main( ){

    ofAppGlutWindow window;

    // for output echo of screen capture area
	ofSetupOpenGL( &window, OUT_W*INIT_MULT, OUT_H*INIT_MULT, OF_WINDOW);

	// pass in width and height too:
	ofRunApp( new sendScreenApp());
}
