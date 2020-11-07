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
		shared_ptr<rtScene> scene = nullptr;
		ofNode transform;
		float fov;

	public:
		///Constructors
		rtCam(ofNode transform);
		rtCam(ofVec3f position, ofVec3f rotation);
		///Scene methods
		shared_ptr<rtScene> getScene();
		void setScene(shared_ptr<rtScene> scene);
	};
}