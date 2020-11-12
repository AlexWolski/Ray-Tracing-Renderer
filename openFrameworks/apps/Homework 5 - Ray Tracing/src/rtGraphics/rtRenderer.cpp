#include "rtRenderer.h"

namespace rtGraphics
{
	//Ray trace an entire scene
	void rtRenderer::rayTraceScene(shared_ptr<rtScene> scene, rtVec3f& camPos, rtVec3f& u, rtVec3f& v, rtVec3f& n,
		float hFov, float nearClip, float farClip, ofPixels* bufferPixels)
	{
		//Cache the objects and lights in the scene
		objectSet objects = scene->getObjects();
		lightSet lights = scene->getLights();

		//Cache the pixel buffer dimensions as floats
		float bufferWidth = bufferPixels->getWidth();
		float bufferHeight = bufferPixels->getHeight();
		//The current index in the buffer pixels array
		int bufferIndex = 0;

		//The width and height of the near clip plane based on the FOV and distance to the clip plane
		float halfClipWidth = cos(degToRad(hFov / 2)) * nearClip;
		float halfClipHeight = halfClipWidth * (bufferHeight / bufferWidth);

		//Calculate the point in the center of the near clip plane using the camera position and look vector
		rtVec3f clipCenter = camPos + (-n * nearClip);
		//Get the vectors pointing from the center of the screen to the left edge and top edge of the near clip plane
		rtVec3f widthVector = u * halfClipWidth;
		rtVec3f heightVector = v * halfClipHeight;
		//The distance between each grid point of the near clip plane in world space
		rtVec3f hStep = (widthVector * -2) / bufferWidth;
		rtVec3f vStep = (heightVector * -2) / bufferHeight;

		//The first grid point of the current row. This vector stays on the left edge of the near clip plane and moves downwards
		rtVec3f currRowStart = clipCenter + widthVector + heightVector;
		//The current grid point. This vector is in the same row as currRowStart and moves to the right
		rtVec3f R = currRowStart;
		//The direction vector from the camera to the current point
		rtVec3f D;

		//Iterate over all the pixels of the pixel buffer
		for (int row = 0; row < bufferPixels->getHeight(); row++)
		{
			for (int col = 0; col < bufferPixels->getWidth(); col++)
			{
				//Find new direction vector
				D = (R - camPos).normalize();
				//Calculate the color of the pixel
				rtColorf pixelColor = rayTrace(objects, lights, camPos, D, v, n, nearClip, farClip);
				//Write the color to the pixel buffer
				(*bufferPixels)[bufferIndex++] = (int)(pixelColor.getR() * 255.0f);
				(*bufferPixels)[bufferIndex++] = (int)(pixelColor.getG() * 255.0f);
				(*bufferPixels)[bufferIndex++] = (int)(pixelColor.getB() * 255.0f);

				//Iterate to the next column
				R += hStep;
			}

			//After a row is completed, move to the next row
			currRowStart += vStep;
			R = currRowStart;
		}
	}

	//Ray trace a single ray
	rtColorf rtRenderer::rayTrace(objectSet& objects, lightSet& lights, rtVec3f& P, rtVec3f& D, rtVec3f& v, rtVec3f& n, float nearClip, float farClip)
	{
		//The distance from the camera to the intersection point
		float minT = INFINITY;
		//The closest object that the ray intersects
		rtObject* hitObject;
		//The closest intersection point and its normal
		rtVec3f hitPos;
		rtVec3f hitNormal;
		//The position and normal of the current intersection point
		shared_ptr<rtVec3f> currHitPos = make_shared<rtVec3f>();
		shared_ptr<rtVec3f> currHitNormal = make_shared<rtVec3f>();

		//Iterate over the all the objects
		for (auto objectPtr = objects->begin(); objectPtr != objects->end(); objectPtr++)
		{
			//Get a pointer to the current object
			rtObject* currObject = objectPtr->second;
			//Determine if the ray intersects the object
			float t = currObject->rayIntersect(P, D, currHitPos, currHitNormal, nearClip, farClip);

			//If the ray hit and the object is not obscured, save the ray parameter and object address
			if (t < minT && t > 0.0f)
			{
				hitObject = currObject;
				hitPos = *currHitPos;
				hitNormal = *currHitNormal;
				minT = t;
			}
		}

		//If the ray didn't intersect any objects, return a black pixel
		//TO-DO: Return the background color of the camera
		if (minT < nearClip || minT > farClip)
			return rtColorf::black;
		//Otherwise calculate the lighting of the pixel
		else
		{
			//The final color to the drawn to the pixel
			rtColorf finalColor = rtColorf();
			//The material properties of the object
			rtMat objectMat = hitObject->getMat();

			//Iterate over the all the lights
			for (auto lightPtr = lights->begin(); lightPtr != lights->end(); lightPtr++)
			{
				//Get a pointer to the current light
				rtLight* currLight = lightPtr->second;
				//Cache the ambient and light intensity
				float incidentIntensity = currLight->getIncidentIntensity();
				float ambientIntensity = currLight->getAmbientIntensity();

				//Calculate the light vector, which is used in both diffuse and specular lighting
				rtVec3f lightVector = (currLight->getPosition() - hitPos);
				lightVector.normalize();

				//Add the three types of lighting from this light to the final color
				finalColor += ambientColor((*currLight).getAmbient(), objectMat.getAmbient(), ambientIntensity);
				finalColor += diffuseColor(lightVector, hitNormal, (*currLight).getDiffuse(), objectMat.getDiffuse(), incidentIntensity);
				finalColor += specularColor(lightVector, n, hitNormal, (*currLight).getSpecular(), objectMat.getSpecular(), objectMat.getSmoothness(), incidentIntensity);
				//Clamp the values of the color
				finalColor.clampColors();

				return finalColor;
			}
		}
	}
}