#pragma once

#include <unordered_set>
#include <memory>
#include "rtCam.h"
#include "ofMain.h"

namespace rtGraphics
{
	typedef unordered_set<ofLight*> lightSet;
	typedef unordered_set<of3dPrimitive*> objectSet;

	//Forward declare rtCam to prevent a cyclic reference
	class rtCam;

	//A data-structure to contain the lights and objects in a scene
	class rtScene
	{
	private:
		//Only one camera can render the scene
		shared_ptr<rtCam> camera;
		//The addresses of the lights and objects are stored in an unordered hash-set
		lightSet lights;
		objectSet objects;

	public:
		///Camera Methods
		void setCamera(shared_ptr<rtCam> camera);
		shared_ptr<rtCam> getCamera();
		///Light Methods
		shared_ptr<lightSet> getLights();
		void addLight(ofLight* lightToAdd);
		void removeLight(ofLight* lightToRemove);
		void clearLights();
		///Object Methods
		shared_ptr<objectSet> getObjects();
		void addObject(of3dPrimitive* objectToAdd);
		void removeObject(of3dPrimitive* objectToRemove);
		void clearObjects();
	};

	///In-line function definitions
	//Camera Methods
	inline shared_ptr<rtCam> rtScene::getCamera()				{ return camera; }
	//Light Methods
	inline shared_ptr<lightSet> rtScene::getLights()			{ return make_shared<lightSet>(lights); }
	inline void rtScene::addLight(ofLight* lightToAdd)			{ lights.insert(lightToAdd); }
	inline void rtScene::removeLight(ofLight* lightToRemove)	{ lights.erase(lightToRemove); }
	inline void rtScene::clearLights()							{ lights.clear(); }
	//Object Methods
	inline shared_ptr<objectSet> rtScene::getObjects()					{ return make_shared<objectSet>(objects); }
	inline void rtScene::addObject(of3dPrimitive* objectToAdd)			{ objects.insert(objectToAdd); }
	inline void rtScene::removeObject(of3dPrimitive* objectToRemove)	{	objects.erase(objectToRemove); }
	inline void rtScene::clearObjects()									{	objects.clear();	}
}