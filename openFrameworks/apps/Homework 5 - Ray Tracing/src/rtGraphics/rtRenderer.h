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
		static void rayTraceScene(shared_ptr<rtScene> scene, rtVec3f& camPos, rtVec3f& u, rtVec3f& v, rtVec3f& n,
			float hFov, float nearClip, float farClip, ofPixels* bufferPixels)
		{
			//Cache the objects and lights in the scene
			objectSet objects = scene->getObjects();
			lightSet lights = scene->getLights();

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
					rtColorf pixelColor = rayTrace(objects, lights, camPos, D, v, n, nearClip, farClip);
					//Write the color to the pixel buffer
					(*bufferPixels)[bufferIndex++] = (int)(pixelColor.getR() * 255.0f);
					(*bufferPixels)[bufferIndex++] = (int)(pixelColor.getG() * 255.0f);
					(*bufferPixels)[bufferIndex++] = (int)(pixelColor.getB() * 255.0f);

					//Iterate to the next column
					R += hStep;
				}

				//After a row is completed, move to the next row
				rowPos += vStep;
				R = rowPos;
			}
		}

		//Ray trace a single ray
		static rtColorf rayTrace(objectSet& objects, lightSet& lights, rtVec3f& P, rtVec3f& D, rtVec3f& v, rtVec3f& n, float nearClip, float farClip)
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
				float t = currObject->rayIntersect(P, D, currHitPos, currHitNormal);

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
			if (minT < 0.0f || minT > farClip)
				return rtColorf(0.0f, 0.0f, 0.0f);
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

		static rtColorf ambientColor(rtColorf& ambientLight, rtColorf& ambientMaterial, float ambientIntensity)
		{
			//The ambient color is calculated using a component-wise multiplication
			rtColorf ambientColor = ambientLight * ambientMaterial;
			
			return ambientColor * ambientIntensity;
		}

		static rtColorf diffuseColor(rtVec3f& lightVector, rtVec3f& normal, rtColorf& diffuseLight, rtColorf& diffuseMaterial, float incidentIntensity)
		{
			float dotProd = normal.dot(lightVector);
			rtColorf diffuseColor = (diffuseLight * diffuseMaterial) * dotProd;

			return diffuseColor * incidentIntensity;
		}

		static rtColorf specularColor(rtVec3f& lightVector, rtVec3f& lookVector, rtVec3f& normal, rtColorf& specularLight, rtColorf& specularMaterial, float smoothness, float incidentIntensity)
		{
			//Calculate the half-way vector
			rtVec3f halfWay = lightVector + lookVector;
			halfWay.normalize();
			//Calculate (N*H)^n, where n is the smoothness parameter
			float dotProd = pow(normal.dot(halfWay), smoothness);
			//Calculate the specular color
			rtColorf specularColor = (specularLight * specularMaterial) * dotProd;

			return specularColor * incidentIntensity;
		}
	};
}