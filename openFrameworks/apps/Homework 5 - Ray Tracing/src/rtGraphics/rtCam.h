#pragma once

#include <memory>
#include "ofAppRunner.h"
#include "ofImage.h"
#include "ofPixels.h"
#include "ofEventUtils.h"
#include "rtVec3f.h"
#include "rtNode.h"
#include "rtScene.h"
#include "rtRenderer.h"

using namespace std;

namespace rtGraphics
{
	class rtCam
	{
	private:
		//The scene to render
		shared_ptr<rtScene> scene = nullptr;
		//The position and orientation of the camera
		rtNode transform;

		//The camera settings
		bool enabled;
		float fov = 90.0f;
		float nearClip = 0.1f;
		float farClip = 1000.0f;
		//Vectors defining the viewing coordinates
		rtVec3f position;
		rtVec3f pref;	//Look-at point
		rtVec3f n;		//Look vector
		rtVec3f V;		//Imprecise up vector
		rtVec3f v;		//Precise up vector
		rtVec3f u;		//Perpendicular vector

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
		//Calculates the axes of the viewing coordinates
		void calcAxes();

	public:
		///Constructors
		rtCam(bool enabled = true);
		rtCam(const rtVec3f& position, const rtVec3f& lookAtPoint, const rtVec3f& upVector, bool enabled = true);
		///Event Listeners
		void draw(ofEventArgs& event);
		///Camera Methods
		void enable();
		void disable();
		bool isEnabled() const;
		void render();
		void draw();
		///Getters
		float getFov() const;
		float getNearClip() const;
		float getFarClip() const;
		shared_ptr<rtScene> getScene() const;
		ofPixels* getBufferPixels();
		rtVec3f getPosition() const;
		rtVec3f getLookVector() const;
		rtVec3f getUpVector() const;
		rtVec3f getPerpVector() const;
		///Setters
		void setFov(float fov);
		void setNearClip(float nearClip);
		void setFarClip(float farClip);
		void setScene(shared_ptr<rtScene> scene);
		void setPosition(const rtVec3f& position);
		void setLookAtPoint(const rtVec3f& lookAtPoint);
		void setUpVector(const rtVec3f& appoxUpVector);
		void setOrientation(const rtVec3f& lookAtPoint, const rtVec3f& appoxUpVector);
	};

	///Constructors
	//If no look-at point and up-vector are provided, default to the camera facing down the z axis
	inline rtCam::rtCam(bool enabled) : rtCam(rtVec3f::zero, rtVec3f::forward, rtVec3f::up, enabled) {}

	inline rtCam::rtCam(const rtVec3f& position, const rtVec3f& lookAtPoint, const rtVec3f& appoxUpVector, bool enabled) :
		position(position), enabled(enabled)
	{
		setOrientation(lookAtPoint, appoxUpVector);
		createFrameBuffer();

		if (enabled)
			enable();
	}

	///In-line method definitions
	//Event Lister
	inline void rtCam::draw(ofEventArgs& event)
	{
		render();
		draw();
	}

	//Getters
	inline bool  rtCam::isEnabled() const				{ return enabled; }
	inline float rtCam::getFov() const					{ return fov; }
	inline float rtCam::getNearClip() const				{ return nearClip; }
	inline float rtCam::getFarClip() const				{ return farClip; }
	inline shared_ptr<rtScene> rtCam::getScene() const	{ return scene; }
	inline ofPixels* rtCam::getBufferPixels()			{ return bufferPixels; }
	inline rtVec3f rtCam::getPosition() const			{ return position; }
	inline rtVec3f rtCam::getLookVector() const			{ return n; }
	inline rtVec3f rtCam::getUpVector() const			{ return V; }
	inline rtVec3f rtCam::getPerpVector() const			{ return u; }

	//Setters
	inline void rtCam::setFov(float fov)							{ this->fov = fov; }
	inline void rtCam::setNearClip(float nearClip)					{ this->nearClip = nearClip; }
	inline void rtCam::setFarClip(float farClip)					{ this->farClip = farClip; }
	inline void rtCam::setScene(const shared_ptr<rtScene> scene)	{ this->scene = scene; }
	inline void rtCam::setPosition(const rtVec3f& position)			{ this->position = position; }
	inline void rtCam::setLookAtPoint(const rtVec3f& lookAtPoint)	{ pref = lookAtPoint; calcAxes(); }
	inline void rtCam::setUpVector(const rtVec3f& appoxUpVector)	{ V = appoxUpVector; calcAxes(); }

	inline void rtCam::setOrientation(const rtVec3f& lookAtPoint, const rtVec3f& appoxUpVector)
	{
		pref = lookAtPoint;
		V = appoxUpVector;
		calcAxes();
	}

	//Camera Methods
	inline void rtCam::enable()
	{
		ofAddListener(ofEvents().draw, this, &rtCam::draw);
		enabled = true;
	}

	inline void rtCam::disable()
	{
		ofRemoveListener(ofEvents().draw, this, &rtCam::draw);
		enabled = false;
	}

	//Render the scene using ray tracing
	inline void rtCam::render()
	{
		rtRenderer::rayTraceScene(scene, position, u, v, n, fov, nearClip, farClip, bufferPixels);
	}

	//Draw the rendered image
	inline void rtCam::draw()
	{
		frameBuffer->update();
		frameBuffer->draw(0, 0);
	}

	//Calculates the axes of the viewing coordinates
	inline void rtCam::calcAxes()
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
	//Creates a 3D image buffer array based on the window size
	inline void rtCam::createFrameBuffer()
	{
		createFrameBuffer(ofGetWindowWidth(), ofGetWindowHeight());
	}

	//Creates a 3D image buffer array
	inline void rtCam::createFrameBuffer(int width, int height)
	{
		//Save the dimensions of the buffer
		bufferWidth = width;
		bufferHeight = height;
		//Create a frame buffer with the given dimensions
		frameBuffer = make_shared<ofImage>();
		frameBuffer->allocate(width, height, OF_IMAGE_COLOR);
		//Store a reference to the pixel data
		bufferPixels = &frameBuffer->getPixels();
	}
}