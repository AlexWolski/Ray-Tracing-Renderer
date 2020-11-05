#pragma once

#include <unordered_set>
#include <memory>
#include "ofMain.h"

#define lightSet unordered_set<ofLight*>
#define objectSet unordered_set<of3dPrimitive*>

namespace rtGraphics
{
	//A data-structure to contain the lights and objects in a scene
	class rtScene
	{
	private:
		//The addresses of the lights and objects are stored in an unordered hash-set
		unordered_set<ofLight*> lights;
		unordered_set<of3dPrimitive*> objects;

	public:
		///Light Methods
		shared_ptr<lightSet> getLights()
		{
			return make_shared<lightSet>(lights);
		}

		void addLight(ofLight* lightToAdd)
		{
			lights.insert(lightToAdd);
		}

		void removeLight(ofLight* lightToRemove)
		{
			lights.erase(lightToRemove);
		}

		void clearLights()
		{
			lights.clear();
		}

		///Object Methods
		shared_ptr<objectSet> getObjects()
		{
			return make_shared<objectSet>(objects);
		}

		void addObject(of3dPrimitive* objectToAdd)
		{
			objects.insert(objectToAdd);
		}

		void removeObject(of3dPrimitive* objectToRemove)
		{
			objects.erase(objectToRemove);
		}

		void clearObjects()
		{
			objects.clear();
		}
	};
}