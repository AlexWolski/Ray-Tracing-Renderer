#include "ofMain.h"
#include "ofApp.h"

int main()
{
	//Create a windowed application running in OpenGL 3.2
	ofGLFWWindowSettings settings;
	settings.setGLVersion(3, 2);
	settings.setSize(512, 512);
	ofCreateWindow(settings);

	//Start the application
	ofRunApp(new ofApp());
}