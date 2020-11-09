#include "ofApp.h"

void ofApp::setup()
{
	demoScene = make_shared<rtScene>();
	mainCamera = make_shared<rtCam>(rtVec3f::forward(), rtVec3f::up());
	demoScene->setCamera(mainCamera);
}

void ofApp::update()
{

}

void ofApp::draw()
{

}