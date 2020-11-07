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
		ofNode transform;
		float fov;
		//A pointer to a 3D array containing the RGB values for each pixel on the screen
		shared_ptr<char***> frameBuffer = NULL;
		//The dimensions of the frame buffer
		int bufferWidth, bufferHeight;

		///Buffer methods
		//Instantiates the frame buffer. If no dimensions are given, the window size is used
		void createFrameBuffer();
		void createFrameBuffer(int width, int height);
		//Deallocates the memory of the 3D array
		void deleteFrameBuffer();

	public:
		///Constructors
		rtCam(ofNode transform);
		rtCam(ofVec3f position, ofVec3f rotation);
		///Destructor
		~rtCam();
		///Scene methods
		shared_ptr<rtScene> getScene();
		void setScene(shared_ptr<rtScene> scene);
		///Camera Methods
		void setFov(float fov);
		float getFov();
		void render();
	};
}