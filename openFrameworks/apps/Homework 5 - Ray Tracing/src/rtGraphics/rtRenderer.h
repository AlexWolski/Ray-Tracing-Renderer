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
		static void rayTrace(shared_ptr<rtScene> scene, rtVec3f camPos, rtVec3f u, rtVec3f v, rtVec3f n,
			float hFov, float nearClip, float farClip, ofPixels* bufferPixels)
		{
			//Cache the pixel buffer dimensions as floats
			float widthf = bufferPixels->getWidth();
			float heightf = bufferPixels->getHeight();

			//The width and height of the near clip plane based on the FOV and distance to the clip plane
			float halfClipWidth = cos(degToRad(hFov / 2)) * nearClip;
			float halfClipHeight = halfClipWidth * (heightf / widthf);

			//Calculate the point in the center of the near clip plane using the camera position and look vector
			rtVec3f clipCenter = camPos + (-n * nearClip);
			//Calculate the horizontal and vertical edges of the clip plane using the perpendicular and up vectors
			rtVec3f clipEdgeH = clipCenter + (u * halfClipWidth);
			rtVec3f clipEdgeV = clipCenter + (v * halfClipHeight);
			//The amount to traverse horizontally and vertically each iteration
			rtVec3f hStep = ((clipEdgeH - clipCenter) * 2) / widthf;
			rtVec3f vStep = ((clipEdgeV - clipCenter) * 2) / heightf;

			//The position to reset to after a row is completed
			rtVec3f edge = clipCenter + (u * halfClipWidth) + (v * halfClipHeight);
			//The current point to cast a ray through
			rtVec3f r = edge;
			//The direction vector from the camera to the current point
			rtVec3f d;

			//Iterate over all the pixels of the pixel buffer
			for (int row = 0; row < bufferPixels->getHeight(); row++)
			{
				for (int col = 0; col < bufferPixels->getWidth(); col++)
				{
					//Find new direction vector
					d = (r - camPos).normalize();
					//Calculate the color of the pixel
					rtColor pixelColor = rayTraceScene(scene, camPos, d, nearClip, farClip);
					//Write the color to the pixel buffer
					bufferPixels->setColor(col, row, ofColor(pixelColor.getR(), pixelColor.getG(), pixelColor.getB()));
				}

				//After a row is completed, update the edge point and reset the current point
				edge += vStep;
				r = edge;
			}
		}

		//Ray trace a single ray
		static rtColor rayTraceScene(shared_ptr<rtScene> scene, rtVec3f p, rtVec3f d, float nearClip, float farClip)
		{
			//Cache the objects and lights in the scene
			objectSet objects = scene->getObjects();
			lightSet lights = scene->getLights();

			//The closest object and the distance to the intersection
			float minT = INFINITY;
			rtObject* hitTarget;

			//Iterate over the all the objects
			for (auto objectPtr = objects->begin(); objectPtr != objects->end(); objectPtr++)
			{
				//Get a pointer to the current object
				rtObject* currObject = objectPtr->second;
				//Determine if the ray intersects the object
				float t = currObject->rayIntersect(p, d);

				//If the ray hit and the object is not obscured, save the ray parameter and object address
				if (t < minT && t > 0.0f)
				{
					hitTarget = currObject;
					minT = t;
				}
			}

			//If the ray didn't intersect any objects, return a black pixel
			//TO-DO: Return the background color of the camera
			if (minT < 0.0f && minT > farClip)
				return rtColor(0.0f, 0.0f, 0.0f);

			//Iterate over the all the lights
			for (auto lightPtr = lights->begin(); lightPtr != lights->end(); lightPtr++)
			{

			}
		}
	};
}