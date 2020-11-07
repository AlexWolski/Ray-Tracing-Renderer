#include "rtCam.h"

namespace rtGraphics
{
	///Constructors
	rtCam::rtCam(ofNode transform, bool enabled) : transform(transform), enabled(enabled)
	{
	}

	rtCam::rtCam(ofVec3f position, ofVec3f rotation, bool enabled) : enabled(enabled)
	{
		transform.setPosition(position);
		transform.setOrientation(ofQuaternion(rotation));
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
		frameBuffer = make_shared<ofPixels>();
		frameBuffer->allocate(width, height, OF_IMAGE_COLOR);
		//Create a render texture where the pixel data is taken from frameBuffer
		renderTexture = make_shared<ofTexture>();
		renderTexture->allocate(*frameBuffer.get());
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
		renderTexture->draw(0, 0);
	}

}