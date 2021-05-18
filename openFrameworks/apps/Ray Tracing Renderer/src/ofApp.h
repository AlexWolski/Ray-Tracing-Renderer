#pragma once

#include "ofBaseApp.h"
#include "rtGraphics/rtMain.h"

using namespace rtGraphics;

class ofApp : public ofBaseApp
{
private:
	shared_ptr<rtScene> demoScene;
	shared_ptr<rtCam> mainCamera;

public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
};