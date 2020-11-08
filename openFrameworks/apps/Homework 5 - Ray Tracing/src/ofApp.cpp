#include "ofApp.h"

void ofApp::setup()
{
	demoScene = make_shared<rtScene>();
	mainCamera = make_shared<rtCam>(ofVec3f(0.0f, 0.0f, 0.0f), ofVec3f(90.0f, 0.0f, 0.0f));
	demoScene->setCamera(mainCamera);
}

void ofApp::update()
{

}

void ofApp::draw()
{

}