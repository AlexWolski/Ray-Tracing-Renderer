#include "rtCam.h"

namespace rtGraphics
{
	///Constructors
	//If no look-at point and up-vector are provided, default to the camera facing down the z axis
	rtCam::rtCam(bool enabled) : rtCam(ofVec3f(0.0f, 0.0f, 1.0f), ofVec3f(0.0f, 1.0f, 0.0f), enabled)
	{
	}

	rtCam::rtCam(ofVec3f lookAtPoint, ofVec3f upVector, bool enabled) : enabled(enabled)
	{
		calculateOrientation(lookAtPoint, upVector);
		createFrameBuffer();

		if (enabled)
			enable();
	}

	///Event Listeners
	void rtCam::draw(ofEventArgs& event)
	{
		render();
	}

	///Buffer Methods
	//Creates a 3D image buffer array based on the window size
	void rtCam::createFrameBuffer()
	{
		createFrameBuffer(ofGetWindowWidth(), ofGetWindowHeight());
	}

	///Camera Methods
	//Calculates the normalized up-vector, look-vector, and perpendicular-vector
	//Inputs: a look-at point and approximate up-vector
	void rtCam::calculateOrientation(ofVec3f lookAtPoint, ofVec3f appoxUpVector)
	{

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

	///Scene Methods
	shared_ptr<rtScene> rtCam::getScene()
	{
		return scene;
	}

	void rtCam::setScene(shared_ptr<rtScene> scene)
	{
		this->scene = scene;
	}

	///Camera Methods
	void rtCam::enable()
	{
		ofAddListener(ofEvents().draw, this, &rtCam::draw);
		enabled = true;
	}

	void rtCam::disable()
	{
		ofRemoveListener(ofEvents().draw, this, &rtCam::draw);
		enabled = false;
	}

	bool rtCam::isEnabled()
	{
		return enabled;
	}

	void rtCam::setFov(float fov)
	{
		this->fov = fov;
	}

	float rtCam::getFov()
	{
		return fov;
	}

	void rtCam::render()
	{
		frameBuffer->update();
		frameBuffer->draw(0, 0);
	}

}