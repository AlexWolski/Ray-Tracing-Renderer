#include "ofApp.h"
#include <graphics/ofGraphics.cpp>

void ofApp::setup()
{
	//Enable depth test to allow for overlay graphics
	ofEnableDepthTest();
	//Show the FPS
	showFps = true;

	///Create the scene and camera
	demoScene = make_shared<rtScene>();
	mainCamera = make_shared<rtCam>(rtVec3f(0.0f, 0.0f, -100.0f), rtVec3f(0.0f, -25.0f, 0.0f), rtVec3f::up);
	mainCamera->setFov(150.0f);
	mainCamera->setScene(demoScene);

	///Create a red, matte sphere and add it to the scene
	rtMat shinyRed(rtColorf(0.2f, 0.0f, 0.0f), rtColorf::red, rtColorf::white, 200.0f);
	rtObject* redSphere = new rtSphereObject(rtVec3f(25.0f, -25.0f, 0.0f), 20.0f, shinyRed);
	demoScene->addObject("red sphere", redSphere);

	///Create a blue, reflective sphere and add it to the scene
	rtMat reflectiveBlue(rtColorf(0.0f, 0.0f, 0.2f), rtColorf::blue, rtColorf::white, 500.0f, 0.3f);
	rtObject* blueSphere = new rtSphereObject(rtVec3f(-25.0f, -25.0f, 0.0f), 20.0f, reflectiveBlue);
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

	//Create a torus and add it to the scene
	rtTorusObject* greenTorus = new rtTorusObject(rtVec3f(0.0f, -60.0f, -20.0f), 20.0f, 5.0f, matteGreen);
	demoScene->addObject("green torus", greenTorus);


	//Create a blue cylinder and add it to the scene
	rtCylinderObject* blueCylinder = new rtCylinderObject(rtVec3f(50.0f, 80.0f, 0.0f), 10.0f, reflectiveBlue);
	demoScene->addObject("blue cylinder", blueCylinder);

	//Create a box surrounding the scene
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

	//Create a ground plane under the scene
	rtPlaneObject* whitePlane = new rtPlaneObject(rtVec3f(0.0f, -70.0f, 0.0f), rtVec3f(0.0f, 1.0f, 0.0f), matteWhite);
	demoScene->addObject("white plane", whitePlane);

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
	// If the camera isn't in real-time mode, draw the buffer each frame
	if (!mainCamera->isEnabled())
		mainCamera->draw();

	if (showFps)
		drawFps();
}

void ofApp::drawFps()
{
	int fps = mainCamera->getFps();
	string fpsString = "FPS: " + to_string(fps);
	ofDrawBitmapString(fpsString, 10, 20, -1);
}

void ofApp::keyPressed(int key)
{
	//When the 't' key is pressed, render the scene using ray tracing
	if (key == 't' || key == 'T')
	{
		//Run the camera in real-time
		mainCamera->enable();
		//Set the rendering mode to ray tracing
		mainCamera->setRenderMode(renderMode::rayTrace);
	}
	//When the 'm' key is pressed, render the scene using ray marching
	else if (key == 'm' || key == 'M')
	{
		//Prevent the camera from rendering a new image each frame
		mainCamera->disable();
		//Set the rendering mode to ray marching
		mainCamera->setRenderMode(renderMode::rayMarch);
		//Set the image to black
		mainCamera->clearBuffer();
		//Start rendering the scene without waiting for it to complete
		mainCamera->render(false);
	}
}
