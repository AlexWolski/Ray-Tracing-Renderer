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
		float incidentIntensity;
		float ambientIntensity;
		bool attenuate;

	public:
		///Constructors
		rtLight(float incidentIntensity = 1.0f, float ambientIntensity = 1.0f, bool attenuate = true);
		rtLight(rtVec3f& position, float incidentIntensity = 1.0f, float ambientIntensity = 1.0f, bool attenuate = true);
		rtLight(rtColorf& ambient, rtColorf& diffuse, rtColorf& specular, float incidentIntensity = 1.0f, float ambientIntensity = 1.0f, bool attenuate = true);
		rtLight(rtVec3f& position, rtColorf& ambient, rtColorf& diffuse, rtColorf& specular, float incidentIntensity = 1.0f, float ambientIntensity = 1.0f, bool attenuate = true);

		///Getters
		rtVec3f getPosition() const;
		rtColorf& getAmbient();
		rtColorf& getDiffuse();
		rtColorf& getSpecular();
		float getIncidentIntensity() const;
		float getAmbientIntensity() const;
		bool attenuateEnabled() const;

		///Setters
		void setPosition(rtVec3f& position);
		void setAmbient(rtColorf& ambient);
		void setDiffuse(rtColorf& diffuse);
		void setSpecular(rtColorf& specular);
		void setColors(rtColorf& ambient, rtColorf& diffuse, rtColorf& specular);
		void setIncidentIntensity(float incidentIntensity);
		void setAmbientIntensity(float ambientIntensity);
		void setAttenuate(bool attenuate);
	};


	///Constructors
	/*
	 * By default, set the ambient color to a dark grey
	 * And set the diffuse and specular to white
	 */
	inline rtLight::rtLight(float incidentIntensity, float ambientIntensity, bool attenuate) :
		incidentIntensity(incidentIntensity), ambientIntensity(ambientIntensity), attenuate(attenuate)
	{
		position = rtVec3f::zero;
		ambient = rtColorf(50.0f, 50.0f, 50.0f);
		diffuse = rtColorf(255.0f, 255.0f, 255.0f);
		specular = rtColorf(255.0f, 255.0f, 255.0f);
	}

	inline rtLight::rtLight(rtVec3f& position, float incidentIntensity, float ambientIntensity, bool attenuate) :
		position(position), incidentIntensity(incidentIntensity), ambientIntensity(ambientIntensity), attenuate(attenuate)
	{
		ambient = rtColorf(50.0f, 50.0f, 50.0f);
		diffuse = rtColorf(255.0f, 255.0f, 255.0f);
		specular = rtColorf(255.0f, 255.0f, 255.0f);
	}

	inline rtLight::rtLight(rtColorf& ambient, rtColorf& diffuse, rtColorf& specular, float incidentIntensity, float ambientIntensity, bool attenuate) :
		ambient(ambient),
		diffuse(diffuse),
		specular(specular),
		incidentIntensity(incidentIntensity),
		ambientIntensity(ambientIntensity),
		attenuate(attenuate) {
	}

	inline rtLight::rtLight(rtVec3f& position, rtColorf& ambient, rtColorf& diffuse, rtColorf& specular, float incidentIntensity, float ambientIntensity, bool attenuate) :
		position(position),
		ambient(ambient),
		diffuse(diffuse),
		specular(specular),
		incidentIntensity(incidentIntensity),
		ambientIntensity(ambientIntensity),
		attenuate(attenuate) {
	}

	///In-line method definitions
	//Getters
	inline rtVec3f rtLight::getPosition() const			{ return position; }
	inline rtColorf& rtLight::getAmbient()				{ return ambient; }
	inline rtColorf& rtLight::getDiffuse()				{ return diffuse; }
	inline rtColorf& rtLight::getSpecular()				{ return specular; }
	inline float rtLight::getIncidentIntensity() const		{ return incidentIntensity; }
	inline float rtLight::getAmbientIntensity() const	{ return ambientIntensity; }
	inline bool rtLight::attenuateEnabled() const		{ return attenuate; }

	//Setters
	inline void rtLight::setPosition(rtVec3f& position)		{ this->position = position; }
	inline void rtLight::setAmbient(rtColorf& ambient)		{ this->ambient = ambient; }
	inline void rtLight::setDiffuse(rtColorf& diffuse)		{ this->diffuse = diffuse; }
	inline void rtLight::setSpecular(rtColorf& specular)	{ this->specular = specular; }

	inline void rtLight::setColors(rtColorf& ambient, rtColorf& diffuse, rtColorf& specular)
	{
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
	}

	inline void rtLight::setIncidentIntensity(float incidentIntensity)	{ this->incidentIntensity = incidentIntensity; }
	inline void rtLight::setAmbientIntensity(float ambientIntensity)	{ this->ambientIntensity = ambientIntensity; }
	inline void rtLight::setAttenuate(bool attenuate)					{ this->attenuate = attenuate; }
}