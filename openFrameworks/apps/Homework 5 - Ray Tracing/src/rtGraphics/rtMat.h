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
		rtMat(const rtColorf& ambient, const rtColorf& diffuse, const rtColorf& specular, float smoothness = 1.0f);

		///Getters
		rtColorf& getAmbient();
		rtColorf& getDiffuse();
		rtColorf& getSpecular();
		float getSmoothness();

		///Setters
		void setAmbient(const rtColorf& ambient);
		void setDiffuse(const rtColorf& diffuse);
		void setSpecular(const rtColorf& specular);
		void setColors(const rtColorf& ambient, const rtColorf& diffuse, const rtColorf& specular);
		void setSmoothness(float smoothness);
	};

	///Constructors
	/* 
	 * By default, set the ambient color to a dark grey
	 * And set the diffuse and specular to white
	 */
	inline rtMat::rtMat()
	{
		ambient = rtColorf(0.2f, 0.2f, 0.2f);
		diffuse = rtColorf::white;
		specular = rtColorf::white;
	}

	inline rtMat::rtMat(const rtColorf& ambient, const rtColorf& diffuse, const rtColorf& specular, float smoothness) :
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
	inline void rtMat::setAmbient(const rtColorf& ambient)	{ this->ambient = ambient; }
	inline void rtMat::setDiffuse(const rtColorf& diffuse)	{ this->diffuse = diffuse; }
	inline void rtMat::setSpecular(const rtColorf& specular)	{ this->specular = specular; }
	inline void rtMat::setSmoothness(float smoothness)	{ this->smoothness = smoothness; }

	inline void rtMat::setColors(const rtColorf& ambient, const  rtColorf& diffuse, const rtColorf& specular)
	{
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
	}
}