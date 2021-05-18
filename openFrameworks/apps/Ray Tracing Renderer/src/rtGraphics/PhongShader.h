#include "Data Classes/rtVec3f.h"
#include "Data Classes/rtColorf.h"

namespace rtGraphics
{
	class PhongShader
	{
	public:
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

		static rtColorf  specularColor(rtVec3f& lightVector, rtVec3f& rayDirection, rtVec3f& normal, rtColorf& specularLight, rtColorf& specularMaterial, float smoothness, float incidentIntensity)
		{
			//Calculate the half-way vector
			rtVec3f halfWay = lightVector + -rayDirection;
			halfWay.normalize();
			//Calculate (N*H)^n, where n is the smoothness parameter
			float dotProd = pow(normal.dot(halfWay), smoothness);
			//Calculate the specular color
			rtColorf specularColor = (specularLight * specularMaterial) * dotProd;

			return specularColor * incidentIntensity;
		}
	};
}