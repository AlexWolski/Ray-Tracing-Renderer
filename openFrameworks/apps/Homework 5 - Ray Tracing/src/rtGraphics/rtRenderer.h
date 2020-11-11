#pragma once

#include <memory>
#include <math.h>
#include "rtScene.h"

#define PI 3.14159265
#define degToRad(angleDegrees) ((angleDegrees) * PI / 180.0f)

namespace rtGraphics
{
	static class rtRenderer
	{
	public:
		//Ray trace an entire scene
		static void rayTraceScene(shared_ptr<rtScene> scene, rtVec3f camPos, rtVec3f u, rtVec3f v, rtVec3f n,
			float hFov, float nearClip, float farClip, ofPixels* bufferPixels)
		{
			//Cache the pixel buffer dimensions as floats
			float widthf = bufferPixels->getWidth();
			float heightf = bufferPixels->getHeight();
			//The current 
			int bufferIndex = 0;

			//The width and height of the near clip plane based on the FOV and distance to the clip plane
			float halfClipWidth = cos(degToRad(hFov / 2)) * nearClip;
			float halfClipHeight = halfClipWidth * (heightf / widthf);

			//Calculate the point in the center of the near clip plane using the camera position and look vector
			rtVec3f clipCenter = camPos + (-n * nearClip);
			//Get the vectors pointing from the center of the screen to the left edge and top edge of the near clip plane
			rtVec3f widthVector = u * halfClipWidth;
			rtVec3f heightVector = v * halfClipHeight;
			//The amount to traverse horizontally and vertically in each iteration
			rtVec3f hStep = (widthVector * -2) / widthf;
			rtVec3f vStep = (heightVector * -2) / heightf;

			//The starting position of the current row. This vector only moves down the screen
			rtVec3f rowPos = clipCenter + widthVector + heightVector;
			//The current point to cast a ray through. This vector is in the same row as rowPos and moves to the right
			rtVec3f R = rowPos;
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
					rtColor pixelColor = rayTrace(scene, camPos, D, v, n, nearClip, farClip);
					//Write the color to the pixel buffer
					(*bufferPixels)[bufferIndex++] = pixelColor.getR();
					(*bufferPixels)[bufferIndex++] = pixelColor.getG();
					(*bufferPixels)[bufferIndex++] = pixelColor.getB();

					//Iterate to the next column
					R += hStep;
				}

				//After a row is completed, move to the next row
				rowPos += vStep;
				R = rowPos;
			}
		}

		//Ray trace a single ray
		static rtColor rayTrace(shared_ptr<rtScene> scene, rtVec3f P, rtVec3f D, rtVec3f v, rtVec3f n, float nearClip, float farClip)
		{
			//Cache the objects and lights in the scene
			objectSet objects = scene->getObjects();
			lightSet lights = scene->getLights();
			
			//The distance from the camera to the intersection point
			float minT = INFINITY;
			//The closest object that the ray intersects
			rtObject* hitObject;
			//The point of intersection and the normal at that point
			shared_ptr<rtVec3f> hitPos = make_shared<rtVec3f>();
			shared_ptr<rtVec3f> hitNormal = make_shared<rtVec3f>();

			//Iterate over the all the objects
			for (auto objectPtr = objects->begin(); objectPtr != objects->end(); objectPtr++)
			{
				//Get a pointer to the current object
				rtObject* currObject = objectPtr->second;
				//Determine if the ray intersects the object
				float t = currObject->rayIntersect(P, D, hitPos, hitNormal);

				//If the ray hit and the object is not obscured, save the ray parameter and object address
				if (t < minT && t > 0.0f)
				{
					hitObject = currObject;
					minT = t;
				}
			}

			//If the ray didn't intersect any objects, return a black pixel
			//TO-DO: Return the background color of the camera
			if (minT < 0.0f || minT > farClip)
				return rtColor(0, 0, 0);
			//Otherwise calculate the lighting of the pixel
			else
			{
				return rtColor(255, 0, 0);

				//The final color to the drawn to the pixel
				rtColor finalColor = rtColor();
				//The material properties of the object
				rtMat objectMaterial = hitObject->getMat();

				//Iterate over the all the lights
				for (auto lightPtr = lights->begin(); lightPtr != lights->end(); lightPtr++)
				{
					//Get a pointer to the current light
					rtLight* currLight = lightPtr->second;
					//Calculate the light vector, used in both diffuse and specular lighting
					rtVec3f lightVector = (currLight->getPosition() - *hitPos).getNormalized();

					//Add the three types of lighting from this light to the final color
					finalColor += ambientColor(objectMaterial.getAmbient(), (*currLight).getAmbient());
					finalColor += diffuseColor(lightVector, *hitNormal, objectMaterial.getDiffuse(), (*currLight).getDiffuse());
					finalColor += specularColor(lightVector, n, v, objectMaterial.getDiffuse(), (*currLight).getDiffuse());
				}
			}
		}

		static rtColor ambientColor(rtColor ambientLight, rtColor ambientMaterial)
		{
			//The ambient color is calculated using a component-wise multiplication
			return ambientLight * ambientMaterial;
		}

		static rtColor diffuseColor(rtVec3f lightVector, rtVec3f normal, rtColor diffuseLight, rtColor diffuseMaterial)
		{
			float dotProd = normal.dot(lightVector);
			return (diffuseLight * diffuseMaterial) * dotProd;
		}

		static rtColor specularColor(rtVec3f lightVector, rtVec3f lookVector, rtVec3f upVector, rtColor specularLight, rtColor specularMaterial)
		{
			rtVec3f halfWay = (lightVector + lookVector).getNormalized();
			float dotProd = upVector.dot(lightVector);
			return (specularLight * specularMaterial) * dotProd;
		}
	};
}