#include "rtScene.h"

namespace rtGraphics
{
	///Camera Methods
	void rtScene::setCamera(shared_ptr<rtCam> camera)
	{
		//Save the camera in the scene
		this->camera = camera;

		//Get a pointer to this scene object and set the camera's scene reference
		shared_ptr<rtScene> selfPointer = make_shared<rtScene>(*this);
		this->camera->setScene(selfPointer);
	}

	shared_ptr<rtCam> rtScene::getCamera()
	{
		return camera;
	}

	///Light Methods
	shared_ptr<lightSet> rtScene::getLights()
	{
		return make_shared<lightSet>(lights);
	}

	void rtScene::addLight(ofLight* lightToAdd)
	{
		lights.insert(lightToAdd);
	}

	void rtScene::removeLight(ofLight* lightToRemove)
	{
		lights.erase(lightToRemove);
	}

	void rtScene::clearLights()
	{
		lights.clear();
	}

	///Object Methods
	shared_ptr<objectSet> rtScene::getObjects()
	{
		return make_shared<objectSet>(objects);
	}

	void rtScene::addObject(of3dPrimitive* objectToAdd)
	{
		objects.insert(objectToAdd);
	}

	void rtScene::removeObject(of3dPrimitive* objectToRemove)
	{
		objects.erase(objectToRemove);
	}

	void rtScene::clearObjects()
	{
		objects.clear();
	}
}