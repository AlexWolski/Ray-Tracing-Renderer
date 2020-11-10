#pragma once

#include <unordered_set>
#include <memory>
#include "rtCam.h"
#include "rtLight.h"
#include "rtObject.h"
#include "ofMain.h"

using namespace std;

namespace rtGraphics
{
	typedef unordered_set<rtLight*> lightSet;
	typedef unordered_set<rtObject*> objectSet;

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
		shared_ptr<rtCam> getCamera() const;
		///Light Methods
		shared_ptr<lightSet> getLights() const;
		void addLight(rtLight* lightToAdd);
		void removeLight(rtLight* lightToRemove);
		void clearLights();
		///Object Methods
		shared_ptr<objectSet> getObjects() const;
		void addObject(rtObject* objectToAdd);
		void removeObject(rtObject* objectToRemove);
		void clearObjects();
	};

	///In-line method definitions
	//Camera Methods
	inline shared_ptr<rtCam> rtScene::getCamera() const			{ return camera; }
	//Light Methods
	inline shared_ptr<lightSet> rtScene::getLights() const		{ return make_shared<lightSet>(lights); }
	inline void rtScene::addLight(rtLight* lightToAdd)			{ lights.insert(lightToAdd); }
	inline void rtScene::removeLight(rtLight* lightToRemove)	{ lights.erase(lightToRemove); }
	inline void rtScene::clearLights()							{ lights.clear(); }
	//Object Methods
	inline shared_ptr<objectSet> rtScene::getObjects() const	{ return make_shared<objectSet>(objects); }
	inline void rtScene::addObject(rtObject* objectToAdd)		{ objects.insert(objectToAdd); }
	inline void rtScene::removeObject(rtObject* objectToRemove)	{ objects.erase(objectToRemove); }
	inline void rtScene::clearObjects()							{ objects.clear(); }
}