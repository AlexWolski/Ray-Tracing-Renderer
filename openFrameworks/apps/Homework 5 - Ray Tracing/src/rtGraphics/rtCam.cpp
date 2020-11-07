#include "rtCam.h"

namespace rtGraphics
{
	///Constructors
	rtCam::rtCam(ofNode transform) : transform(transform)
	{
	}

	rtCam::rtCam(ofVec3f position, ofVec3f rotation)
	{
		transform.setPosition(position);
		transform.setOrientation(ofQuaternion(rotation));
	}

	///Scene methods
	shared_ptr<rtScene> rtCam::getScene()
	{
		return scene;
	}

	void rtCam::setScene(shared_ptr<rtScene> scene)
	{
		this->scene = scene;
	}
}