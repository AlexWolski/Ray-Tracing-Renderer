#include "ofApp.h"

void ofApp::setup()
{
	//Create the scene and camera
	demoScene = make_shared<rtScene>();
	mainCamera = make_shared<rtCam>(rtVec3f::forward, rtVec3f::up);
	mainCamera->setScene(demoScene);

	//Create a sphere and add it to the scene
	rtMat red(rtColorf(0.2f, 0.0f, 0.0f), rtColorf(1.0f, 0.0f, 0.0f), rtColorf(0.5f, 0.5f, 0.5f), 200.0f);
	rtObject* sphere = new rtSphere(rtVec3f(0.0f, 0.0f, 100.0f), 20.0f, red);
	demoScene->addObject("sphere", sphere);
	
	//Create a mesh and add it to the scene
	rtMat green(rtColorf(0.2f, 0.2f, 0.2f), rtColorf(0.0f, 1.0f, 0.0f), rtColorf(0.8f, 0.8f, 0.8f));

	rtMesh triangleMesh;
	triangleMesh.addVert(rtVec3f(50.0f, 0.0f, 100.0f));
	triangleMesh.addVert(rtVec3f(80.0f, 0.0f, 100.0f));
	triangleMesh.addVert(rtVec3f(65.0f, 30.0f, 100.0f));
	triangleMesh.addFace(0, 1, 2);

	rtObject* triangle = new rtMeshObject(triangleMesh, green);
	demoScene->addObject("triangle", triangle);

	//Create a light and add it to the scene
	rtLight* pointLight = new rtLight(rtVec3f(40.0f, 40.0f, 50.0f), rtColorf(0.2f, 0.2f, 0.2f), rtColorf(1.0f, 1.0f, 1.0f), rtColorf(0.5f, 0.5f, 0.5f));
	demoScene->addLight("main light", pointLight);
}

void ofApp::update()
{

}

void ofApp::draw()
{

}