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
		//Vectors defining the viewing coordinates
		ofVec3f position;
		ofVec3f pref;	//Look-at point
		ofVec3f n;		//Look vector
		ofVec3f V;		//Imprecise up vector
		ofVec3f v;		//Precise up vector
		ofVec3f u;		//Perpendicular vector

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
		rtCam(const ofVec3f& lookAtPoint, const ofVec3f& upVector, bool enabled = true);
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
		ofVec3f getPosition() const;
		ofVec3f getLookVector() const;
		ofVec3f getUpVector() const;
		ofVec3f getPerpVector() const;
		///Setters
		void setFov(float fov);
		void setNearClip(float nearClip);
		void setFarClip(float farClip);
		void setScene(shared_ptr<rtScene> scene);
		void setPosition(const ofVec3f& position);
		void setLookAtPoint(const ofVec3f& lookAtPoint);
		void setUpVector(const ofVec3f& appoxUpVector);
		void setOrientation(const ofVec3f& lookAtPoint, const ofVec3f& appoxUpVector);
	};

	///In-line method definitions
	//Getters
	inline bool  rtCam::isEnabled() const				{ return enabled; }
	inline float rtCam::getFov() const					{ return fov; }
	inline float rtCam::getNearClip() const				{ return nearClip; }
	inline float rtCam::getFarClip() const				{ return farClip; }
	inline shared_ptr<rtScene> rtCam::getScene() const	{ return scene; }
	inline ofVec3f rtCam::getPosition() const			{ return position; }
	inline ofVec3f rtCam::getLookVector() const			{ return pref; }
	inline ofVec3f rtCam::getUpVector() const			{ return V; }
	inline ofVec3f rtCam::getPerpVector() const			{ return u; }

	//Setters
	inline void rtCam::setFov(float fov)							{ this->fov = fov; }
	inline void rtCam::setNearClip(float nearClip)					{ this->nearClip = nearClip; }
	inline void rtCam::setFarClip(float farClip)					{ this->farClip = farClip; }
	inline void rtCam::setScene(const shared_ptr<rtScene> scene)	{ this->scene = scene; }
	inline void rtCam::setPosition(const ofVec3f& position)			{ this->position = position; }
	inline void rtCam::setLookAtPoint(const ofVec3f& lookAtPoint)	{ pref = lookAtPoint; calcAxes(); }
	inline void rtCam::setUpVector(const ofVec3f& appoxUpVector)	{ V = appoxUpVector; calcAxes(); }

	inline void rtCam::setOrientation(const ofVec3f& lookAtPoint, const ofVec3f& appoxUpVector)
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
		//TO-DO
	}

	//Draw the rendered image
	inline void rtCam::draw()
	{
		frameBuffer->update();
		frameBuffer->draw(0, 0);
	}
}