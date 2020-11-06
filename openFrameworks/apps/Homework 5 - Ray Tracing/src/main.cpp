#include "ofMain.h"
#include "ofApp.h"

int main()
{
	//Create a windowed application running in OpenGL 3.2
	ofGLFWWindowSettings settings;
	settings.setGLVersion(3, 2);
	settings.setSize(1024, 768);
	ofCreateWindow(settings);

	//Start the application
	ofRunApp(new ofApp());
}