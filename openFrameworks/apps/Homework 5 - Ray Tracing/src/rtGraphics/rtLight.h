#pragma once

#include "rtNode.h"
#include "rtVec3f.h"
#include "rtColor.h"

namespace rtGraphics
{
	/*
	 * A light object that defines the light position, color, and intensity
	 * TO-DO: Extend the class to support point, area, directional, and spot lights
	 */
	class rtLight : rtNode
	{
	private:
		//Light colors
		rtColor ambient;
		rtColor diffuse;
		rtColor specular;
		//Light properties
		rtVec3f position;
		float intensity;
		bool attenuate;

	public:
		///Constructors
		rtLight(float intensity = 1.0f, bool attenuate = true);
		rtLight(rtVec3f& position, float intensity = 1.0f, bool attenuate = true);
		rtLight(rtColor& ambient, rtColor& diffuse, rtColor& specular, float intensity = 1.0f, bool attenuate = true);

		///Getters
		rtColor& getAmbient();
		rtColor& getDiffuse();
		rtColor& getSpecular();
		float getIntensity() const;
		bool attenuateEnabled() const;

		///Setters
		void setAmbient(rtColor& ambient);
		void setDiffuse(rtColor& diffuse);
		void setSpecular(rtColor& specular);
		void setColors(rtColor& ambient, rtColor& diffuse, rtColor& specular);
		void setIntensity(float intensity);
		void setAttenuate(bool attenuate);
	};


	///Constructors
	/*
	 * By default, set the ambient color to a dark grey
	 * And set the diffuse and specular to white
	 */
	inline rtLight::rtLight(float intensity, bool attenuate) : intensity(intensity), attenuate(attenuate)
	{
		position = rtVec3f::zero;
		ambient = rtColor(50, 50, 50);
		diffuse = rtColor(255, 255, 255);
		specular = rtColor(255, 255, 255);
	}

	inline rtLight::rtLight(rtVec3f& position, float intensity, bool attenuate) :
		position(position), intensity(intensity), attenuate(attenuate)
	{
		ambient = rtColor(50, 50, 50);
		diffuse = rtColor(255, 255, 255);
		specular = rtColor(255, 255, 255);
	}

	inline rtLight::rtLight(rtColor& ambient, rtColor& diffuse, rtColor& specular, float intensity, bool attenuate) :
		ambient(ambient),
		diffuse(diffuse),
		specular(specular),
		intensity(intensity),
		attenuate(attenuate) {
	}

	///In-line method definitions
	//Getters
	inline rtColor& rtLight::getAmbient()			{ return ambient; }
	inline inline rtColor& rtLight::getDiffuse()	{ return diffuse; }
	inline rtColor& rtLight::getSpecular()			{ return specular; }
	inline float rtLight::getIntensity() const		{ return intensity; }
	inline bool rtLight::attenuateEnabled() const	{ return attenuate; }

	//Setters
	inline void rtLight::setAmbient(rtColor& ambient)	{ this->ambient = ambient; }
	inline void rtLight::setDiffuse(rtColor& diffuse)	{ this->diffuse = diffuse; }
	inline void rtLight::setSpecular(rtColor& specular)	{ this->specular = specular; }

	inline void rtLight::setColors(rtColor& ambient, rtColor& diffuse, rtColor& specular)
	{
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
	}

	inline void rtLight::setIntensity(float intensity)	{ this->intensity = intensity; }
	inline void rtLight::setAttenuate(bool attenuate)	{ this->attenuate = attenuate; }
}