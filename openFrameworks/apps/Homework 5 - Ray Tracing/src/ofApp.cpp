#include "ofApp.h"

void ofApp::setup()
{
	//Create the scene and camera
	demoScene = make_shared<rtScene>();
	mainCamera = make_shared<rtCam>(rtVec3f::forward, rtVec3f::up);
	mainCamera->setScene(demoScene);

	//Create a sphere and add it to the scene
	rtMat red(rtColor(50, 50, 50), rtColor(255, 0, 0), rtColor(255, 255, 255));
	rtObject* sphere = new rtSphere(rtVec3f(0.0f, 0.0f, 100.0f), 20.0f, red);
	demoScene->addObject("sphere", sphere);
	
	//Create a mesh and add it to the scene
	rtMat green(rtColor(50, 50, 50), rtColor(0, 255, 0), rtColor(255, 255, 255));

	rtMesh triangleMesh;
	triangleMesh.addVert(rtVec3f(50.0f, 0.0f, 100.0f));
	triangleMesh.addVert(rtVec3f(80.0f, 0.0f, 100.0f));
	triangleMesh.addVert(rtVec3f(65.0f, 30.0f, 100.0f));
	triangleMesh.addFace(0, 1, 2);

	rtObject* triangle = new rtMeshObject(triangleMesh, green);
	demoScene->addObject("triangle", triangle);

	//Create a light and add it to the scene
	rtLight* pointLight = new rtLight(rtColor(50, 50, 50), rtColor(200, 200, 200), rtColor(255, 255, 255));
	demoScene->addLight("main light", pointLight);
}

void ofApp::update()
{

}

void ofApp::draw()
{

}