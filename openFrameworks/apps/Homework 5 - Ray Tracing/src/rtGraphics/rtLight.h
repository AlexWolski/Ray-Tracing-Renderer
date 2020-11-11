#pragma once

#include "rtNode.h"
#include "rtVec3f.h"
#include "rtColorf.h"

namespace rtGraphics
{
	/*
	 * A light object that defines the light position, color, and intensity
	 * TO-DO: Extend the class to support point, area, directional, and spot lights
	 */
	class rtLight : public rtNode
	{
	private:
		//Light colors
		rtColorf ambient;
		rtColorf diffuse;
		rtColorf specular;
		//Light properties
		rtVec3f position;
		float intensity;
		bool attenuate;

	public:
		///Constructors
		rtLight(float intensity = 1.0f, bool attenuate = true);
		rtLight(rtVec3f& position, float intensity = 1.0f, bool attenuate = true);
		rtLight(rtColorf& ambient, rtColorf& diffuse, rtColorf& specular, float intensity = 1.0f, bool attenuate = true);
		rtLight(rtVec3f& position, rtColorf& ambient, rtColorf& diffuse, rtColorf& specular, float intensity = 1.0f, bool attenuate = true);

		///Getters
		rtVec3f getPosition() const;
		rtColorf& getAmbient();
		rtColorf& getDiffuse();
		rtColorf& getSpecular();
		float getIntensity() const;
		bool attenuateEnabled() const;

		///Setters
		void setPosition(rtVec3f& position);
		void setAmbient(rtColorf& ambient);
		void setDiffuse(rtColorf& diffuse);
		void setSpecular(rtColorf& specular);
		void setColors(rtColorf& ambient, rtColorf& diffuse, rtColorf& specular);
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
		ambient = rtColorf(50, 50, 50);
		diffuse = rtColorf(255, 255, 255);
		specular = rtColorf(255, 255, 255);
	}

	inline rtLight::rtLight(rtVec3f& position, float intensity, bool attenuate) :
		position(position), intensity(intensity), attenuate(attenuate)
	{
		ambient = rtColorf(50, 50, 50);
		diffuse = rtColorf(255, 255, 255);
		specular = rtColorf(255, 255, 255);
	}

	inline rtLight::rtLight(rtColorf& ambient, rtColorf& diffuse, rtColorf& specular, float intensity, bool attenuate) :
		ambient(ambient),
		diffuse(diffuse),
		specular(specular),
		intensity(intensity),
		attenuate(attenuate) {
	}

	inline rtLight::rtLight(rtVec3f& position, rtColorf& ambient, rtColorf& diffuse, rtColorf& specular, float intensity, bool attenuate) :
		position(position),
		ambient(ambient),
		diffuse(diffuse),
		specular(specular),
		intensity(intensity),
		attenuate(attenuate) {
	}

	///In-line method definitions
	//Getters
	inline rtVec3f rtLight::getPosition() const		{ return position; }
	inline rtColorf& rtLight::getAmbient()			{ return ambient; }
	inline rtColorf& rtLight::getDiffuse()			{ return diffuse; }
	inline rtColorf& rtLight::getSpecular()			{ return specular; }
	inline float rtLight::getIntensity() const		{ return intensity; }
	inline bool rtLight::attenuateEnabled() const	{ return attenuate; }

	//Setters
	inline void rtLight::setPosition(rtVec3f& position)	{ this->position = position; }
	inline void rtLight::setAmbient(rtColorf& ambient)	{ this->ambient = ambient; }
	inline void rtLight::setDiffuse(rtColorf& diffuse)	{ this->diffuse = diffuse; }
	inline void rtLight::setSpecular(rtColorf& specular)	{ this->specular = specular; }

	inline void rtLight::setColors(rtColorf& ambient, rtColorf& diffuse, rtColorf& specular)
	{
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
	}

	inline void rtLight::setIntensity(float intensity)	{ this->intensity = intensity; }
	inline void rtLight::setAttenuate(bool attenuate)	{ this->attenuate = attenuate; }
}