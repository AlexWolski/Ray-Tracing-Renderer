#include "ofApp.h"

void ofApp::setup()
{
	///Create the scene and camera
	demoScene = make_shared<rtScene>();
	mainCamera = make_shared<rtCam>(rtVec3f(0.0f, 0.0f, -100.0f), rtVec3f::forward, rtVec3f::up);
	mainCamera->setFov(90.0f);
	mainCamera->setScene(demoScene);

	///Create a red, matte sphere and add it to the scene
	rtMat shinyRed(rtColorf(0.2f, 0.0f, 0.0f), rtColorf::red, rtColorf::white, 200.0f);
	rtObject* redSphere = new rtSphere(rtVec3f(25.0f, -25.0f, 0.0f), 20.0f, shinyRed);
	demoScene->addObject("red sphere", redSphere);

	///Create a blue, reflective sphere and add it to the scene
	rtMat reflectiveBlue(rtColorf(0.0f, 0.0f, 0.2f), rtColorf::blue, rtColorf::white, 200.0f, 0.3f);
	rtObject* blueSphere = new rtSphere(rtVec3f(-25.0f, -25.0f, 0.0f), 20.0f, reflectiveBlue);
	demoScene->addObject("blue sphere", blueSphere);
	
	///Create a mesh and add it to the scene
	rtMesh triangleMesh;
	triangleMesh.addVert(rtVec3f(-50.0f, 0.0f, 0.0f));
	triangleMesh.addVert(rtVec3f(0.0f, 50.0f, 0.0f));
	triangleMesh.addVert(rtVec3f(50.0f, 0.0f, 0.0f));
	triangleMesh.addFace(0, 1, 2);

	rtMat matteGreen(rtColorf(0.0f, 0.2f, 0.0f), rtColorf::green, rtColorf(0.6f), 20.0f);
	rtObject* triangle = new rtMeshObject(triangleMesh, matteGreen);
	demoScene->addObject("triangle", triangle);

	///Create a box surrounding the scene
	rtMesh boxMesh;
	boxMesh.addVert(rtVec3f(-200.0f, -200.0f, -200.0f));
	boxMesh.addVert(rtVec3f(-200.0f, -200.0f, 200.0f));
	boxMesh.addVert(rtVec3f(-200.0f, 200.0f, -200.0f));
	boxMesh.addVert(rtVec3f(-200.0f, 200.0f, 200.0f));
	boxMesh.addVert(rtVec3f(200.0f, -200.0f, -200.0f));
	boxMesh.addVert(rtVec3f(200.0f, -200.0f, 200.0f));
	boxMesh.addVert(rtVec3f(200.0f, 200.0f, -200.0f));
	boxMesh.addVert(rtVec3f(200.0f, 200.0f, 200.0f));

	boxMesh.addFace(1, 0, 2); boxMesh.addFace(2, 3, 1);		//Left Wall
	boxMesh.addFace(4, 5, 7); boxMesh.addFace(7, 6, 4);		//Right Wall
	boxMesh.addFace(0, 4, 6); boxMesh.addFace(6, 2, 0);		//Back Wall
	boxMesh.addFace(5, 1, 3); boxMesh.addFace(3, 7, 5);		//Front Wall
	boxMesh.addFace(2, 6, 7); boxMesh.addFace(7, 3, 2);		//Top Wall
	boxMesh.addFace(4, 0, 1); boxMesh.addFace(1, 5, 4);		//Bottom Wall

	rtMat matteWhite(rtColorf(0.2f), rtColorf::white, rtColorf(0.4f), 20.0f);
	rtObject* box = new rtMeshObject(boxMesh, matteWhite);
	demoScene->addObject("box", box);

	///Create a light and add it to the scene
	rtLight* pointLight = new rtLight(rtVec3f(-50.0f, 40.0f, -60.0f), rtColorf(0.3f), rtColorf(0.9f), rtColorf(0.8f));
	pointLight->setAmbientIntensity(1.0f);
	pointLight->setIncidentIntensity(0.5f);
	demoScene->addLight("left light", pointLight);

	///Create an additional light and add it to the scene
	pointLight = new rtLight(rtVec3f(50.0f, 40.0f, -60.0f), rtColorf(0.3f), rtColorf(0.9f), rtColorf(0.8f));
	pointLight->setAmbientIntensity(1.0f);
	pointLight->setIncidentIntensity(0.5f);
	demoScene->addLight("right light", pointLight);
}

void ofApp::update()
{

}

void ofApp::draw()
{

}