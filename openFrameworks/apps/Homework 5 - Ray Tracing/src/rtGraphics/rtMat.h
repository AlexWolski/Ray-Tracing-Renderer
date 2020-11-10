#pragma once

#include "rtColor.h"

namespace rtGraphics
{
	//Stores the properties of a material
	class rtMat
	{
	private:
		rtColor ambient;
		rtColor diffuse;
		rtColor specular;

	public:
		///Constructors
		rtMat();
		rtMat(rtColor& ambient, rtColor& diffuse, rtColor& specular);

		///Getters
		rtColor& getAmbient();
		rtColor& getDiffuse();
		rtColor& getSpecular();

		///Setters
		void setAmbient(rtColor& ambient);
		void setDiffuse(rtColor& diffuse);
		void setSpecular(rtColor& specular);
		void setColors(rtColor& ambient, rtColor& diffuse, rtColor& specular);
	};

	///Constructors
	/* 
	 * By default, set the ambient color to a dark grey
	 * And set the diffuse and specular to white
	 */
	inline rtMat::rtMat()
	{
		ambient = rtColor(50, 50, 50);
		diffuse = rtColor(255, 255, 255);
		specular = rtColor(255, 255, 255);
	}

	inline rtMat::rtMat(rtColor& ambient, rtColor& diffuse, rtColor& specular) :
		ambient(ambient),
		diffuse(diffuse),
		specular(specular) {
	}

	///In-line method definitions
	//Getters
	inline rtColor& rtMat::getAmbient()	{ return ambient; }
	inline rtColor& rtMat::getDiffuse()	{ return diffuse; }
	inline rtColor& rtMat::getSpecular()	{ return specular; }

	//Setters
	inline void rtMat::setAmbient(rtColor& ambient)	{ this->ambient = ambient; }
	inline void rtMat::setDiffuse(rtColor& diffuse)	{ this->diffuse = diffuse; }
	inline void rtMat::setSpecular(rtColor& specular)	{ this->specular = specular; }

	inline void rtMat::setColors(rtColor& ambient, rtColor& diffuse, rtColor& specular)
	{
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
	}
}