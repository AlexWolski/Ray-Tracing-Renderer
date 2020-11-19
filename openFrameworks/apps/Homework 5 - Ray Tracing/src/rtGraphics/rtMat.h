#pragma once

#include "rtColorf.h"

namespace rtGraphics
{
	//Stores the properties of a material
	class rtMat
	{
	private:
		//The colors for the three different types of lighting.
		rtColorf ambient;
		rtColorf diffuse;
		rtColorf specular;
		//Controls the size and intensity of the specular highlight. Between 0 and infinity.
		float smoothness;
		//Controls how much light bounces off of the object. Between 0 and 1.
		float reflectivity;

	public:
		///Constructors
		rtMat();
		rtMat(const rtColorf& ambient, const rtColorf& diffuse, const rtColorf& specular, float smoothness = 20.0f, float reflectivity = 0.0f);

		///Getters
		rtColorf& getAmbient();
		rtColorf& getDiffuse();
		rtColorf& getSpecular();
		float getSmoothness();
		float getReflectivity();

		///Setters
		void setAmbient(const rtColorf& ambient);
		void setDiffuse(const rtColorf& diffuse);
		void setSpecular(const rtColorf& specular);
		void setColors(const rtColorf& ambient, const rtColorf& diffuse, const rtColorf& specular);
		void setSmoothness(float smoothness);
		void setReflectivity(float smoothness);
	};

	///Constructors
	/* 
	 * By default, set the ambient color to a dark grey,
	 * set the diffuse color to white,
	 * and set the specular color to a neutral grey.
	 */
	inline rtMat::rtMat() : rtMat(rtColorf(0.2f), rtColorf::white, rtColorf(0.4f)) {
	}

	inline rtMat::rtMat(const rtColorf& ambient, const rtColorf& diffuse, const rtColorf& specular, float smoothness, float refelctivity) :
		smoothness(smoothness),
		ambient(ambient),
		diffuse(diffuse),
		specular(specular),
		reflectivity(reflectivity) {
	}

	///In-line method definitions
	//Getters
	inline rtColorf& rtMat::getAmbient()	{ return ambient; }
	inline rtColorf& rtMat::getDiffuse()	{ return diffuse; }
	inline rtColorf& rtMat::getSpecular()	{ return specular; }
	inline float rtMat::getSmoothness()		{ return smoothness; }
	inline float rtMat::getReflectivity()	{ return reflectivity; }

	//Setters
	inline void rtMat::setAmbient(const rtColorf& ambient)		{ this->ambient = ambient; }
	inline void rtMat::setDiffuse(const rtColorf& diffuse)		{ this->diffuse = diffuse; }
	inline void rtMat::setSpecular(const rtColorf& specular)	{ this->specular = specular; }
	inline void rtMat::setSmoothness(float smoothness)			{ this->smoothness = smoothness; }
	inline void rtMat::setReflectivity(float reflectivity)		{ this->reflectivity = reflectivity; }

	inline void rtMat::setColors(const rtColorf& ambient, const  rtColorf& diffuse, const rtColorf& specular)
	{
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
	}
}