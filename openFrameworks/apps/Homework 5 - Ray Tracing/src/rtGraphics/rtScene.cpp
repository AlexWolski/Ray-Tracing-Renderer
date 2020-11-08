#include "rtScene.h"

namespace rtGraphics
{
	///This method cannot be in-line and must be in a separate cpp file because
	///it references the method 'setScene' in the forward-declared class rtScene
	void rtScene::setCamera(shared_ptr<rtCam> camera)
	{
		//Save the camera in the scene
		this->camera = camera;

		//Get a pointer to this scene object and set the camera's scene reference
		shared_ptr<rtScene> selfPointer = make_shared<rtScene>(*this);
		this->camera->setScene(selfPointer);
	}
}