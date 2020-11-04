#pragma once

#include "ofMain.h"
#include "./rtGraphics/rtCam.h"
#include "./rtGraphics/rtScene.h"

using namespace rtGraphics;

class ofApp : public ofBaseApp
{
	public:
		void setup();
		void update();
		void draw();
};