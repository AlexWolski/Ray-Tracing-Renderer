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
			//Scene data
			static objectSet objects;
			static lightSet lights;
			//Camera data
			static rtVec3f camPos, u, v, n;
			static float nearClip, farClip;
			//Output buffer data
			static ofPixels* bufferPixels;
			static float bufferWidth, bufferHeight;
			//Grid data
			static rtVec3f firstRow, hStep, vStep;

			//Set the shared data
			static void setData(shared_ptr<rtScene>scene, rtVec3f& camPos, rtVec3f& u, rtVec3f& v, rtVec3f& n,
				float nearClip, float farClip, ofPixels* bufferPixels, rtVec3f& firstRow, rtVec3f& hStep, rtVec3f& vStep);

			//Renders a section of the frame buffer
			void threadedFunction(int startRow, int endRow);
		};

		//TO-DO: Set the number of threads based on the CPU cores and hyper threads
		static int numThreads;
		//A pool of threads to render the image
		static rayTraceThread* threadPool;

		//Instantiate the thread pool
		static rayTraceThread* makeThreads();

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