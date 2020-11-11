#pragma once

#include "rtColorf.h"

namespace rtGraphics
{
	//Stores the properties of a material
	class rtMat
	{
	private:
		rtColorf ambient;
		rtColorf diffuse;
		rtColorf specular;
		float smoothness;

	public:
		///Constructors
		rtMat();
		rtMat(rtColorf& ambient, rtColorf& diffuse, rtColorf& specular, float smoothness = 1.0f);

		///Getters
		rtColorf& getAmbient();
		rtColorf& getDiffuse();
		rtColorf& getSpecular();
		float getSmoothness();

		///Setters
		void setAmbient(rtColorf& ambient);
		void setDiffuse(rtColorf& diffuse);
		void setSpecular(rtColorf& specular);
		void setColors(rtColorf& ambient, rtColorf& diffuse, rtColorf& specular);
		void setSmoothness(float smoothness);
	};

	///Constructors
	/* 
	 * By default, set the ambient color to a dark grey
	 * And set the diffuse and specular to white
	 */
	inline rtMat::rtMat()
	{
		ambient = rtColorf(50, 50, 50);
		diffuse = rtColorf(255, 255, 255);
		specular = rtColorf(255, 255, 255);
	}

	inline rtMat::rtMat(rtColorf& ambient, rtColorf& diffuse, rtColorf& specular, float smoothness) :
		smoothness(smoothness),
		ambient(ambient),
		diffuse(diffuse),
		specular(specular) {
	}

	///In-line method definitions
	//Getters
	inline rtColorf& rtMat::getAmbient()	{ return ambient; }
	inline rtColorf& rtMat::getDiffuse()	{ return diffuse; }
	inline rtColorf& rtMat::getSpecular()	{ return specular; }
	inline float rtMat::getSmoothness()		{ return smoothness; }

	//Setters
	inline void rtMat::setAmbient(rtColorf& ambient)	{ this->ambient = ambient; }
	inline void rtMat::setDiffuse(rtColorf& diffuse)	{ this->diffuse = diffuse; }
	inline void rtMat::setSpecular(rtColorf& specular)	{ this->specular = specular; }
	inline void rtMat::setSmoothness(float smoothness)	{ this->smoothness = smoothness; }

	inline void rtMat::setColors(rtColorf& ambient, rtColorf& diffuse, rtColorf& specular)
	{
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
	}
}