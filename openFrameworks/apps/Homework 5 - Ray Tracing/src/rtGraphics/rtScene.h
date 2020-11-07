#pragma once

#include <unordered_set>
#include <memory>
#include "rtCam.h"
#include "ofMain.h"

#define lightSet unordered_set<ofLight*>
#define objectSet unordered_set<of3dPrimitive*>

namespace rtGraphics
{
	//Forward declare rtCam to prevent a cyclic reference
	class rtCam;

	//A data-structure to contain the lights and objects in a scene
	class rtScene
	{
	private:
		//Only one camera can render the scene
		shared_ptr<rtCam> camera;
		//The addresses of the lights and objects are stored in an unordered hash-set
		unordered_set<ofLight*> lights;
		unordered_set<of3dPrimitive*> objects;

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
}