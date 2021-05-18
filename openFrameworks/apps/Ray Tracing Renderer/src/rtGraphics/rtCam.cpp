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
		//Start the render and wait for it to complete
		render(true);
		//Draw the rendered image to the screen
		draw();
	}

	//Getters
	bool  rtCam::isEnabled() const { return enabled; }
	float rtCam::getFov() const { return fov; }
	float rtCam::getNearClip() const { return nearClip; }
	float rtCam::getFarClip() const { return farClip; }
	int rtCam::getMaxBounces() const { return maxBounces; }
	int rtCam::getFps() const { return fps; }
	renderMode rtCam::getRenderMode() const { return RenderMode; }
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
	void rtCam::setRenderMode(renderMode RenderMode) { this->RenderMode = RenderMode; }
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
		startFpsTimer();
		enabled = true;
	}

	void rtCam::disable()
	{
		ofRemoveListener(ofEvents().draw, this, &rtCam::draw);
		enabled = false;
	}

	//Render the scene
	void rtCam::render(bool waitForRender)
	{
		renderer.render(RenderMode, scene, position, u, v, n, fov, nearClip, farClip, maxBounces, bufferPixels);

		if (waitForRender)
			renderer.waitForRender();
	}

	//Draw the rendered image
	void rtCam::draw()
	{
		frameBuffer->update();
		frameBuffer->draw(0, 0);
		updateFps();
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

	//Start counting the number of rendered frames
	void rtCam::startFpsTimer()
	{
		fpsTimer = chrono::steady_clock::now();
		frameCounter = 0;
	}

	//Increments the number of frames rendered this interval and updates the fps
	void rtCam::updateFps()
	{
		frameCounter++;

		//Get the elapsed time since the frame counting began
		chrono::steady_clock::time_point currentTime = chrono::steady_clock::now();
		chrono::duration<double> ellapsedTime = chrono::duration_cast<chrono::duration<double>>(currentTime - fpsTimer);

		//If a second has passed, update the fps and reset the timer
		if (ellapsedTime.count() > 1.0f)
		{
			fpsTimer = currentTime;
			fps = frameCounter;
			frameCounter = 0;
		}
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