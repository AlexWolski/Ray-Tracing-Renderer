#pragma once

#include <memory>
#include <math.h>
#include <vector>
#include "ofPixels.h"
#include "ofThread.h"
#include "rtScene.h"

#define PI 3.14159265
#define degToRad(angleDegrees) ((angleDegrees) * PI / 180.0f)

namespace rtGraphics
{
	static class rtRenderer
	{
	private:
		//A threadable nested class that renders a portion of the final image
		class rayTraceThread : public ofThread
		{
		private:
			///Shared Data
			//Data for the scene, camera orientation, and output buffer
			static shared_ptr<rtScene> scene;
			static rtVec3f& camPos, u, v, n;
			static int nearClip, farClip;
			static ofPixels* bufferPixels;
			static float bufferWidth, bufferHeight;
			//The distance between each grid point of the near clip plane in world space
			static rtVec3f hStep, vStep;

			///Instance Data
			//Variables that define the start and end of the section to render
			int startRow, startCol, endRow, endCol;
			rtVec3f currRowStart;

			//Renders a section of the buffer
			void threadedFunction();

		public:
			rayTraceThread(int startRow, int startCol, int endRow, int endCol);
		};

		//A pool the threads rendering the image
		vector<rayTraceThread> threadPool;

	public:
		//Ray trace an entire scene
		static void rayTraceScene(shared_ptr<rtScene> scene, rtVec3f& camPos, rtVec3f& u, rtVec3f& v, rtVec3f& n,
			float hFov, float nearClip, float farClip, ofPixels* bufferPixels);

		//Ray trace a single ray
		static rtColorf rayTrace(objectSet& objects, lightSet& lights, rtVec3f& P, rtVec3f& D, rtVec3f& v, rtVec3f& n, float nearClip, float farClip);

		//Color calculation methods
		static rtColorf ambientColor(rtColorf& ambientLight, rtColorf& ambientMaterial, float ambientIntensity);
		static rtColorf diffuseColor(rtVec3f& lightVector, rtVec3f& normal, rtColorf& diffuseLight, rtColorf& diffuseMaterial, float incidentIntensity);
		static rtColorf specularColor(rtVec3f& lightVector, rtVec3f& lookVector, rtVec3f& normal, rtColorf& specularLight, rtColorf& specularMaterial, float smoothness, float incidentIntensity);
	};

	///In-line method definitions
	inline rtColorf rtRenderer::ambientColor(rtColorf& ambientLight, rtColorf& ambientMaterial, float ambientIntensity)
	{
		//The ambient color is calculated using a component-wise multiplication
		rtColorf ambientColor = ambientLight * ambientMaterial;

		return ambientColor * ambientIntensity;
	}

	inline rtColorf  rtRenderer::diffuseColor(rtVec3f& lightVector, rtVec3f& normal, rtColorf& diffuseLight, rtColorf& diffuseMaterial, float incidentIntensity)
	{
		float dotProd = normal.dot(lightVector);
		rtColorf diffuseColor = (diffuseLight * diffuseMaterial) * dotProd;

		return diffuseColor * incidentIntensity;
	}

	inline rtColorf  rtRenderer::specularColor(rtVec3f& lightVector, rtVec3f& lookVector, rtVec3f& normal, rtColorf& specularLight, rtColorf& specularMaterial, float smoothness, float incidentIntensity)
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
}