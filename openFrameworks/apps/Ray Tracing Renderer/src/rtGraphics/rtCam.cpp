#include "rtCam.h"

namespace rtGraphics
{
	///Constructors
	//If no look-at point and up-vector are provided, default to the camera facing down the z axis
	rtCam::rtCam(bool enabled) : rtCam(rtVec3f::zero, rtVec3f::forward, rtVec3f::up, enabled) {}

	rtCam::rtCam(const rtVec3f& position, const rtVec3f& lookAtPoint, const rtVec3f& appoxUpVector, bool enabled) :
		position(position), enabled(enabled)
	{
		setOrientation(lookAtPoint, appoxUpVector);
		createFrameBuffer();

		if (enabled)
			enable();
	}

	///In-line method definitions
	//Event Lister
	void rtCam::draw(ofEventArgs& event)
	{
		render();
		draw();
	}

	//Getters
	bool  rtCam::isEnabled() const { return enabled; }
	float rtCam::getFov() const { return fov; }
	float rtCam::getNearClip() const { return nearClip; }
	float rtCam::getFarClip() const { return farClip; }
	int rtCam::getMaxBounces() const { return maxBounces; }
	shared_ptr<rtScene> rtCam::getScene() const { return scene; }
	ofPixels* rtCam::getBufferPixels() { return bufferPixels; }
	rtVec3f rtCam::getPosition() const { return position; }
	rtVec3f rtCam::getLookVector() const { return n; }
	rtVec3f rtCam::getUpVector() const { return V; }
	rtVec3f rtCam::getPerpVector() const { return u; }

	//Setters
	void rtCam::setFov(float fov) { this->fov = fov; }
	void rtCam::setNearClip(float nearClip) { this->nearClip = nearClip; }
	void rtCam::setFarClip(float farClip) { this->farClip = farClip; }
	void rtCam::setMaxBounces(int maxBounces) { this->maxBounces = maxBounces; }
	void rtCam::setScene(const shared_ptr<rtScene> scene) { this->scene = scene; }
	void rtCam::setPosition(const rtVec3f& position) { this->position = position; }
	void rtCam::setLookAtPoint(const rtVec3f& lookAtPoint) { pref = lookAtPoint; calcAxes(); }
	void rtCam::setUpVector(const rtVec3f& appoxUpVector) { V = appoxUpVector; calcAxes(); }

	void rtCam::setOrientation(const rtVec3f& lookAtPoint, const rtVec3f& appoxUpVector)
	{
		pref = lookAtPoint;
		V = appoxUpVector;
		calcAxes();
	}

	//Camera Methods
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

	//Render the scene using ray tracing
	void rtCam::render()
	{
		renderer.rayTraceScene(scene, position, u, v, n, fov, nearClip, farClip, maxBounces, bufferPixels);
	}

	//Draw the rendered image
	void rtCam::draw()
	{
		frameBuffer->update();
		frameBuffer->draw(0, 0);
	}

	//Reset the image buffer to black
	void rtCam::clearBuffer()
	{
		frameBuffer->setColor(ofColor::black);
	}

	//Calculates the axes of the viewing coordinates
	void rtCam::calcAxes()
	{
		//Calculate the normalized look-vector
		n = position - pref;
		n.normalize();
		//Cross the look-vector with the imprecise up-vector to get the perpendicular vector
		u = V.getCrossed(n);
		u.normalize();
		//Cross the perpendicular vector with the look-vector to get a precise look-vector
		v = n.getCrossed(u);
		v.normalize();
	}

	///Buffer Methods
	//Creates a 2D image buffer array based on the window size
	void rtCam::createFrameBuffer()
	{
		createFrameBuffer(ofGetWindowWidth(), ofGetWindowHeight());
	}

	//Creates a 2D image buffer array
	void rtCam::createFrameBuffer(int width, int height)
	{
		//Save the dimensions of the buffer
		bufferWidth = width;
		bufferHeight = height;
		//Create a frame buffer with the given dimensions
		frameBuffer = make_shared<ofImage>();
		frameBuffer->allocate(width, height, OF_IMAGE_COLOR);
		//Store a reference to the pixel data
		bufferPixels = &frameBuffer->getPixels();
		//Set a the image to black by default
		clearBuffer();
	}
}