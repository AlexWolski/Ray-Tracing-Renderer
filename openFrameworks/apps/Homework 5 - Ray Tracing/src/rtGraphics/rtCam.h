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
		rtCam(ofVec3f lookAtPoint, ofVec3f upVector, bool enabled = true);
		///Event Listeners
		void draw(ofEventArgs& event);
		///Camera Methods
		void enable();
		void disable();
		bool isEnabled();
		void render();
		void draw();
		///Setters
		void setFov(float fov);
		void setNearClip(float nearClip);
		void setFarClip(float farClip);
		void setScene(shared_ptr<rtScene> scene);
		void setPosition(ofVec3f position);
		void setLookAtPoint(ofVec3f lookAtPoint);
		void setUpVector(ofVec3f appoxUpVector);
		void setOrientation(ofVec3f lookAtPoint, ofVec3f appoxUpVector);
		///Getters
		float getFov();
		float getNearClip();
		float getFarClip();
		shared_ptr<rtScene> getScene();
		ofVec3f getPosition();
		ofVec3f getLookVector();
		ofVec3f getUpVector();
		ofVec3f getPerpVector();
	};

	///In-line function definitions
	//Setters
	inline void rtCam::setFov(float fov)					{ this->fov = fov; }
	inline void rtCam::setNearClip(float nearClip)			{ this->nearClip = nearClip; }
	inline void rtCam::setFarClip(float farClip)			{ this->farClip = farClip; }
	inline void rtCam::setPosition(ofVec3f position)		{ this->position = position; }
	inline void rtCam::setScene(shared_ptr<rtScene> scene)	{ this->scene = scene; }
	inline void rtCam::setLookAtPoint(ofVec3f lookAtPoint)							{ pref = lookAtPoint; calcAxes(); }
	inline void rtCam::setUpVector(ofVec3f appoxUpVector)							{ V = appoxUpVector; calcAxes(); }
	inline void rtCam::setOrientation(ofVec3f lookAtPoint, ofVec3f appoxUpVector)	{ pref = lookAtPoint; V = appoxUpVector; calcAxes(); }
	//Getters
	inline bool  rtCam::isEnabled()					{ return enabled; }
	inline float rtCam::getFov()					{ return fov; }
	inline float rtCam::getNearClip()				{ return nearClip; }
	inline float rtCam::getFarClip()				{ return farClip; }
	inline shared_ptr<rtScene> rtCam::getScene()	{ return scene; }
	inline ofVec3f rtCam::getPosition()				{ return position; }
	inline ofVec3f rtCam::getLookVector()			{ return pref; }
	inline ofVec3f rtCam::getUpVector()				{ return V; }
	inline ofVec3f rtCam::getPerpVector()			{ return u; }

	///Camera Methods
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