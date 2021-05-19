#pragma once

#include <memory>
#include <chrono>
#include "ofAppRunner.h"
#include "ofImage.h"
#include "ofPixels.h"
#include "ofEventUtils.h"
#include "rtNode.h"
#include "Data Classes/rtScene.h"
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
		int maxBounces = 3;
		renderMode RenderMode = renderMode::rayTrace;
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
		//An rtRenderer instance for this camera
		rtRenderer renderer;

		///Fps methods
		//The time when the frame counting began
		chrono::steady_clock::time_point fpsTimer;
		//Number of frames rendered during this interval
		int frameCounter;
		//Number of framed rendered in the last second interval
		int fps;

		///Buffer methods
		//Instantiates the frame buffer. If no dimensions are given, the window size is used
		void createFrameBuffer();
		void createFrameBuffer(int width, int height);
		///Camera methods
		//Calculates the axes of the viewing coordinates
		void calcAxes();

		///FPS methods
		void startFpsTimer();
		void updateFps();

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
		void render(bool waitForRender);
		void draw();
		void clearBuffer();
		///Getters
		float getFov() const;
		float getNearClip() const;
		float getFarClip() const;
		int getMaxBounces() const;
		renderMode getRenderMode() const;
		int getFps() const;
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
		void setMaxBounces(int maxBounces);
		void setRenderMode(renderMode RenderMode);
		void setScene(shared_ptr<rtScene> scene);
		void setPosition(const rtVec3f& position);
		void setLookAtPoint(const rtVec3f& lookAtPoint);
		void setUpVector(const rtVec3f& appoxUpVector);
		void setOrientation(const rtVec3f& lookAtPoint, const rtVec3f& appoxUpVector);
	};
}
