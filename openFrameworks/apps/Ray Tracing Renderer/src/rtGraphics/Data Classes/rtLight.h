#pragma once

#include "../rtNode.h"
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
		rtLight(const rtVec3f& position, float incidentIntensity = 1.0f, float ambientIntensity = 1.0f, bool attenuate = true);
		rtLight(const rtColorf& ambient, const rtColorf& diffuse, const rtColorf& specular, float incidentIntensity = 1.0f, float ambientIntensity = 1.0f, bool attenuate = true);
		rtLight(const rtVec3f& position, const rtColorf& ambient, const rtColorf& diffuse, const rtColorf& specular, float incidentIntensity = 1.0f, float ambientIntensity = 1.0f, bool attenuate = true);

		///Getters
		rtVec3f getPosition() const;
		rtColorf& getAmbient();
		rtColorf& getDiffuse();
		rtColorf& getSpecular();
		float getIncidentIntensity() const;
		float getAmbientIntensity() const;
		bool attenuateEnabled() const;

		///Setters
		void setPosition(const rtVec3f& position);
		void setAmbient(const rtColorf& ambient);
		void setDiffuse(const rtColorf& diffuse);
		void setSpecular(const rtColorf& specular);
		void setColors(const rtColorf& ambient, const rtColorf& diffuse, const rtColorf& specular);
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
		rtLight(rtVec3f::zero, incidentIntensity, ambientIntensity, attenuate) {
	}

	inline rtLight::rtLight(const rtVec3f& position, float incidentIntensity, float ambientIntensity, bool attenuate) :
		position(position), incidentIntensity(incidentIntensity), ambientIntensity(ambientIntensity), attenuate(attenuate)
	{
		ambient = rtColorf(0.2f, 0.2f, 0.2f);
		diffuse = rtColorf::white;
		specular = rtColorf::white;
	}

	inline rtLight::rtLight(const rtColorf& ambient, const rtColorf& diffuse, const rtColorf& specular, float incidentIntensity, float ambientIntensity, bool attenuate) :
		rtLight(rtVec3f::zero, ambient, diffuse, specular, incidentIntensity, ambientIntensity, attenuate) {
	}

	inline rtLight::rtLight(const rtVec3f& position, const rtColorf& ambient, const rtColorf& diffuse, const rtColorf& specular, float incidentIntensity, float ambientIntensity, bool attenuate) :
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
	inline float rtLight::getIncidentIntensity() const	{ return incidentIntensity; }
	inline float rtLight::getAmbientIntensity() const	{ return ambientIntensity; }
	inline bool rtLight::attenuateEnabled() const		{ return attenuate; }

	//Setters
	inline void rtLight::setPosition(const rtVec3f& position)	{ this->position = position; }
	inline void rtLight::setAmbient(const rtColorf& ambient)	{ this->ambient = ambient; }
	inline void rtLight::setDiffuse(const rtColorf& diffuse)	{ this->diffuse = diffuse; }
	inline void rtLight::setSpecular(const rtColorf& specular)	{ this->specular = specular; }

	inline void rtLight::setColors(const rtColorf& ambient, const rtColorf& diffuse, const rtColorf& specular)
	{
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
	}

	inline void rtLight::setIncidentIntensity(float incidentIntensity)	{ this->incidentIntensity = incidentIntensity; }
	inline void rtLight::setAmbientIntensity(float ambientIntensity)	{ this->ambientIntensity = ambientIntensity; }
	inline void rtLight::setAttenuate(bool attenuate)					{ this->attenuate = attenuate; }
}