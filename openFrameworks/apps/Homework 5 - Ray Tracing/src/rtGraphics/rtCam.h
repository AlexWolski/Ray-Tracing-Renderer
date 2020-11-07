#pragma once

#include <memory>
#include "rtScene.h"
#include "ofMain.h"

namespace rtGraphics
{
	//Forward declare rtScene to prevent a cyclic reference
	class rtScene;

	class rtCam
	{
	private:
		//The scene to render
		shared_ptr<rtScene> scene = nullptr;
		//The position and orientation of the camera
		ofNode transform;

		//The camera settings
		bool enabled;
		float fov;

		//An data structure containing the pixels to be drawn to the screen
		shared_ptr<ofPixels> frameBuffer;
		//The texture object used to draw the pixels
		shared_ptr<ofTexture> renderTexture;
		//The dimensions of the frame buffer
		int bufferWidth, bufferHeight;

		///Buffer methods
		//Instantiates the frame buffer. If no dimensions are given, the window size is used
		void createFrameBuffer();
		void createFrameBuffer(int width, int height);

	public:
		///Constructors
		rtCam(ofNode transform, bool enabled = true);
		rtCam(ofVec3f position, ofVec3f rotation, bool enabled = true);
		///Event Listeners
		void draw(ofEventArgs& event);
		///Scene methods
		shared_ptr<rtScene> getScene();
		void setScene(shared_ptr<rtScene> scene);
		///Camera Methods
		void enable();
		void disable();
		bool isEnabled();
		void setFov(float fov);
		float getFov();
		void render();
	};
}