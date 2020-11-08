#include "rtCam.h"

namespace rtGraphics
{
	///Constructors
	rtCam::rtCam(ofNode transform, bool enabled) : transform(transform), enabled(enabled)
	{
		createFrameBuffer();

		if (enabled)
			enable();
	}

	rtCam::rtCam(ofVec3f position, ofVec3f rotation, bool enabled) : enabled(enabled)
	{
		//Create an ofNode instance given the position and rotation
		ofNode transform = ofNode();
		transform.setPosition(position);
		transform.setOrientation(ofQuaternion(rotation));

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