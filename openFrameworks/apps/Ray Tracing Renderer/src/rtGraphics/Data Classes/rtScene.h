#pragma once

#include <memory>
#include <vector>
#include "rtLight.h"
#include "../Objects/rtObject.h"

using namespace std;

namespace rtGraphics
{
	//A data-structure to contain the lights and objects in a scene
	class rtScene
	{
	public:
		typedef shared_ptr<vector<rtLight*>> lightSet;
		typedef shared_ptr<vector<rtObject*>> objectSet;

	private:
		//The addresses of the lights and objects are stored in an unordered hash-set
		lightSet lights;
		objectSet objects;

	public:
		///Constructor
		rtScene();
		///Light Methods
		lightSet getLights() const;
		void addLight(rtLight* lightToAdd);
		rtLight* getLight(int lightIndex);
		void removeLight(int lightIndex);
		void clearLights();
		///Object Methods
		objectSet getObjects() const;
		void addObject(rtObject* objectToAdd);
		rtObject* getObject(int objectIndex);
		void removeObject(int objectIndex);
		void clearObjects();
	};

	///Constructor
	inline rtScene::rtScene()
	{
		lights = make_shared<vector<rtLight*>>();
		objects = make_shared<vector<rtObject*>>();
	}

	///In-line method definitions
	//Light Methods
	inline rtScene::lightSet rtScene::getLights() const { return lights; }

	inline void rtScene::addLight(rtLight* lightToAdd)
	{
		lights->push_back(lightToAdd);
	}

	inline rtLight* rtScene::getLight(int lightIndex)		{ return lights->operator[](lightIndex); }
	inline void rtScene::removeLight(int lightIndex)		{ lights->erase(lights->begin() + lightIndex); }
	inline void rtScene::clearLights()						{ lights->clear(); }

	//Object Methods
	inline rtScene::objectSet rtScene::getObjects() const			{ return objects; }

	inline void rtScene::addObject(rtObject* objectToAdd)
	{
		objects->push_back(objectToAdd);
	}

	inline rtObject* rtScene::getObject(int objectIndex)	{ return objects->operator[](objectIndex); }
	inline void rtScene::removeObject(int objectIndex)		{ objects->erase(objects->begin() + objectIndex); }
	inline void rtScene::clearObjects()						{ objects->clear(); }
}