#pragma once

#include <memory>
#include <map>
#include "rtLight.h"
#include "rtObject.h"
#include "ofMain.h"

using namespace std;

namespace rtGraphics
{
	typedef shared_ptr<map<string, rtLight*>> lightSet;
	typedef shared_ptr<map<string, rtObject*>> objectSet;

	//A data-structure to contain the lights and objects in a scene
	class rtScene
	{
	private:
		//The addresses of the lights and objects are stored in an unordered hash-set
		lightSet lights;
		objectSet objects;

	public:
		///Constructor
		rtScene();
		///Light Methods
		lightSet getLights() const;
		void addLight(string lightName, rtLight* lightToAdd);
		rtLight* getLight(string lightName);
		void removeLight(string lightName);
		void clearLights();
		///Object Methods
		objectSet getObjects() const;
		void addObject(string objectName, rtObject* objectToAdd);
		rtObject* getObject(string objectName);
		void removeObject(string objectName);
		void clearObjects();
	};

	///Constructor
	inline rtScene::rtScene()
	{
		lights = make_shared<map<string, rtLight*>>();
		objects = make_shared<map<string, rtObject*>>();
	}

	///In-line method definitions
	//Light Methods
	inline lightSet rtScene::getLights() const { return lights; }

	inline void rtScene::addLight(string objectName, rtLight* lightToAdd)
	{
		lights->insert(pair<string, rtLight*>(objectName, lightToAdd));
	}

	inline rtLight* rtScene::getLight(string lightName)		{ return lights->operator[](lightName); }
	inline void rtScene::removeLight(string lightName)		{ lights->erase(lightName); }
	inline void rtScene::clearLights()						{ lights->clear(); }

	//Object Methods
	inline objectSet rtScene::getObjects() const			{ return objects; }

	inline void rtScene::addObject(string objectName, rtObject* objectToAdd)
	{
		objects->insert(pair<string, rtObject*>(objectName, objectToAdd));
	}

	inline rtObject* rtScene::getObject(string objectName)	{ return objects->operator[](objectName); }
	inline void rtScene::removeObject(string objectName)	{ objects->erase(objectName); }
	inline void rtScene::clearObjects()						{ objects->clear(); }
}