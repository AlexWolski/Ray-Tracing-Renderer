#pragma once

namespace rtGraphics
{
	class rtVec3f
	{
	private:
		float x, y, z;

	public:
		///Constructors
		rtVec3f(float value = 0) : x(value), y(value), z(value) {}
		rtVec3f(float x, float y, float z) : x(x), y(y), z(z) {}

		///Static Methods
		static rtVec3f one();
		static rtVec3f zero();

		///Vector Methods
		rtVec3f& normalize();
		rtVec3f  getNormalized();
		rtVec3f& cross(rtVec3f& rhs);
		rtVec3f  getCrossed(rtVec3f& rhs);
		rtVec3f& dot(rtVec3f& rhs);
		rtVec3f  getDotted(rtVec3f& rhs);


		///Operators
		//Scalar Addition
		rtVec3f  operator+ (float rhs) const;
		rtVec3f& operator+=(float rhs) const;
		//Vector Addition
		rtVec3f  operator+ (const rtVec3f rhs) const;
		rtVec3f& operator+=(const rtVec3f rhs) const;
		//Scalar Subtraction
		rtVec3f  operator- (float rhs) const;
		rtVec3f& operator-=(float rhs) const;
		//Vector Subtraction
		rtVec3f  operator- (const rtVec3f rhs) const;
		rtVec3f& operator-=(const rtVec3f rhs) const;
		//Scalar Multiplication
		rtVec3f  operator* (float rhs) const;
		rtVec3f& operator*=(float rhs) const;
		//Vector Multiplication
		rtVec3f  operator* (const rtVec3f rhs) const;
		rtVec3f& operator*=(const rtVec3f rhs) const;
		//Scalar Division
		rtVec3f  operator/ (float rhs) const;
		rtVec3f& operator/=(float rhs) const;
		//Vector Division
		rtVec3f  operator/ (const rtVec3f rhs) const;
		rtVec3f& operator/=(const rtVec3f rhs) const;
		//Unary Negation
		rtVec3f  operator-() const;
		rtVec3f  operator-() const;
	};
}