#include "ofMain.h"
#include "ofApp.h"

int main()
{
	//Set up the GL context
	ofSetupOpenGL(1024,768,OF_WINDOW);
	//Start the application
	ofRunApp(new ofApp());
}