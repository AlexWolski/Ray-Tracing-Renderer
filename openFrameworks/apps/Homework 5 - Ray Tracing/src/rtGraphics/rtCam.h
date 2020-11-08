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
		float nearClip;
		float farClip;
		//Vectors defining the camera position and orientation
		ofVec3f position;
		ofVec3f lookAtPoint;
		ofVec3f lookVector;
		ofVec3f upVector;
		ofVec3f perpVector;

		//An image to temporarily store the render before being drawn to the screen
		shared_ptr<ofImage> frameBuffer;
		//A reference to the pixel data of the frame buffer
		ofPixels* bufferPixels;
		//The dimensions of the frame buffer
		int bufferWidth, bufferHeight;

		///Buffer methods
		//Instantiates the frame buffer. If no dimensions are given, the window size is used
		void createFrameBuffer();
		void createFrameBuffer(int width, int height);
		///Camera methods
		//Calculates the normalized up-vector, look-vector, and perpendicular-vector
		//Inputs: a look-at point and approximate up-vector
		void calculateOrientation(ofVec3f lookAtPoint, ofVec3f appoxUpVector);

	public:
		///Constructors
		rtCam(bool enabled = true);
		rtCam(ofVec3f lookAtPoint, ofVec3f upVector, bool enabled = true);
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