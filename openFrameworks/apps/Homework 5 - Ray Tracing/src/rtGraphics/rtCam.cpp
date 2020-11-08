#include "rtCam.h"

namespace rtGraphics
{
	///Constructors
	//If no look-at point and up-vector are provided, default to the camera facing down the z axis
	rtCam::rtCam(bool enabled) : rtCam(ofVec3f(0.0f, 0.0f, 1.0f), ofVec3f(0.0f, 1.0f, 0.0f), enabled)
	{
	}

	rtCam::rtCam(ofVec3f lookAtPoint, ofVec3f appoxUpVector, bool enabled) : enabled(enabled)
	{
		setOrientation(lookAtPoint, appoxUpVector);
		createFrameBuffer();

		if (enabled)
			enable();
	}

	///Event Listeners
	//Render the scene and draw it to the screen
	void rtCam::draw(ofEventArgs& event)
	{
		render();
		draw();
	}

	///Buffer Methods
	//Creates a 3D image buffer array based on the window size
	void rtCam::createFrameBuffer()
	{
		createFrameBuffer(ofGetWindowWidth(), ofGetWindowHeight());
	}

	//Creates a 3D image buffer array
	void rtCam::createFrameBuffer(int width, int height)
	{
		//Save the dimensions of the buffer
		bufferWidth = width;
		bufferHeight = height;
		//Create a frame buffer with the given dimensions
		frameBuffer = make_shared<ofImage>();
		frameBuffer->allocate(width, height, OF_IMAGE_COLOR);
		//Set the default color to black
		frameBuffer->setColor(ofColor(0.0f, 0.0f, 0.0f));
		//Store a reference to the pixel data
		bufferPixels = &frameBuffer->getPixels();
	}

	///Camera Methods
	/*
	 * Calculates the axes of the viewing coordinates
	 * (normalized look-vector, up-vector, and perpendicular-vector)
	 * using the look-at point and approximate up vector
	 */
	void rtCam::calcAxes()
	{
		//TO-DO
	}
}