#include "rtCam.h"

namespace rtGraphics
{
	///Constructors
	//If no look-at point and up-vector are provided, default to the camera facing down the z axis
	rtCam::rtCam(bool enabled) :
		rtCam(ofVec3f(0.0f, 0.0f, 1.0f), ofVec3f(0.0f, 1.0f, 0.0f), enabled)
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
	//Calculates the normalized up-vector, look-vector, and perpendicular-vector
	//Inputs: a look-at point and approximate up-vector
	void rtCam::calculateOrientation(ofVec3f lookAtPoint, ofVec3f appoxUpVector)
	{

	}

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

	//Render the scene using ray tracing
	void rtCam::render()
	{

	}

	//Draw the rendered image
	void rtCam::draw()
	{
		frameBuffer->update();
		frameBuffer->draw(0, 0);
	}

	///Setters
	void rtCam::setFov(float fov)					{ this->fov = fov; }
	void rtCam::setNearClip(float nearClip)			{ this->nearClip = nearClip; }
	void rtCam::setFarClip(float farClip)			{ this->farClip = farClip; }
	void rtCam::setPosition(ofVec3f position)		{ this->position = position; }
	void rtCam::setScene(shared_ptr<rtScene> scene)	{ this->scene = scene; }

	void rtCam::setLookAtPoint(ofVec3f lookAtPoint)
	{
		calculateOrientation(lookAtPoint, upVector);
	}

	void rtCam::setUpVector(ofVec3f appoxUpVector)
	{
		calculateOrientation(lookAtPoint, appoxUpVector);
	}

	void rtCam::setOrientation(ofVec3f lookAtPoint, ofVec3f appoxUpVector)
	{
		calculateOrientation(lookAtPoint, appoxUpVector);
	}

	///Getters
	float rtCam::getFov()					{ return fov; }
	float rtCam::getNearClip()				{ return nearClip; }
	float rtCam::getFarClip()				{ return farClip; }
	shared_ptr<rtScene> rtCam::getScene()	{ return scene; }
	ofVec3f rtCam::getPosition()			{ return position; }
	ofVec3f rtCam::getLookAtVector()		{ return lookAtPoint; }
	ofVec3f rtCam::getUpVector()			{ return upVector; }
	ofVec3f rtCam::getPerpVector()			{ return perpVector; }
}