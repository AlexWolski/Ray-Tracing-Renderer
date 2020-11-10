#pragma once

#include <math.h>

namespace rtGraphics
{
	class rtVec3f
	{
	private:
		float x, y, z;

	public:
		///Static Members
		static const rtVec3f one, zero, up, down, forward, back, left, right;

		///Constructors
		rtVec3f() : x(0.0f), y(0.0f), z(0.0f) { };
		explicit rtVec3f(float value) : x(value), y(value), z(value) {}
		rtVec3f(float x, float y, float z) : x(x), y(y), z(z) {}

		///Setters
		void set(float value);
		void set(float x, float y, float z);
		void setX(float x);
		void setY(float y);
		void setZ(float z);

		///Getters
		float getX() const;
		float getY() const;
		float getZ() const;

		///Vector Methods
		float magnitude() const;
		float magnitudeSquared() const;
		rtVec3f& normalize();
		rtVec3f  getNormalized() const;
		rtVec3f& cross(const rtVec3f& rhs);
		rtVec3f  getCrossed(const rtVec3f& rhs) const;
		float dot(const rtVec3f& rhs) const;

		///Operators
		//Copy assignment
		rtVec3f& operator=(const rtVec3f& rhs);
		//Comparison
		bool operator==(const rtVec3f& rhs);
		bool operator!=(const rtVec3f& rhs);
		//Scalar Addition
		rtVec3f  operator+ (float value) const;
		rtVec3f& operator+=(float value);
		//Vector Addition
		rtVec3f  operator+ (const rtVec3f& rhs) const;
		rtVec3f& operator+=(const rtVec3f& rhs);
		//Scalar Subtraction
		rtVec3f  operator- (float value) const;
		rtVec3f& operator-=(float value);
		//Vector Subtraction
		rtVec3f  operator- (const rtVec3f& rhs) const;
		rtVec3f& operator-=(const rtVec3f& rhs);
		//Scalar Multiplication
		rtVec3f  operator* (float value) const;
		rtVec3f& operator*=(float value);
		//Vector Multiplication
		rtVec3f  operator* (const rtVec3f& rhs) const;
		rtVec3f& operator*=(const rtVec3f& rhs);
		//Scalar Division
		rtVec3f  operator/ (float value) const;
		rtVec3f& operator/=(float value);
		//Vector Division
		rtVec3f  operator/ (const rtVec3f& rhs) const;
		rtVec3f& operator/=(const rtVec3f& rhs);
		//Unary Negation
		rtVec3f  operator-() const;
	};

	///In-line method definitions
	//Setters
	inline void rtVec3f::set(float value)
	{
		x = value;
		y = value;
		z = value;
	}

	inline void rtVec3f::set(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	inline void rtVec3f::setX(float x)
	{
		this->x = x;
	}

	inline  void rtVec3f::setY(float y)
	{
		this->y = y;
	}

	inline void rtVec3f::setZ(float z)
	{
		this->z = z;
	}

	//Getters
	inline float rtVec3f::getX() const
	{
		return x;
	}

	inline float rtVec3f::getY() const
	{
		return y;
	}

	inline float rtVec3f::getZ() const
	{
		return z;
	}

	//Vector Methods
	inline float rtVec3f::magnitude() const
	{
		return sqrt(x*x + y*y + z*z);
	}

	inline float rtVec3f::magnitudeSquared() const
	{
		return (x*x + y*y + z*z);
	}

	inline rtVec3f& rtVec3f::normalize()
	{
		*this /= magnitude();
		return *this;
	}

	inline rtVec3f rtVec3f::getNormalized() const
	{
		rtVec3f normalized = *this;
		normalized /= magnitude();
		return normalized;
	}

	inline rtVec3f& rtVec3f::cross(const rtVec3f& rhs)
	{
		///Calculate the x and y components of the cross products and store them in temporary variables
		float crossX = (y * rhs.z) - (z * rhs.y);
		//Simplified version of -((x * rhs.z) - (z * rhs.x))
		float crossY = (z * rhs.x) - (x * rhs.z);

		///Calculate the z component of the cross product and assign the x, y, z
		///values of the vector to the cross product result
		//We can omit the temporary variable for z to avoid an extra allocation
		z = (x * rhs.y) - (y * rhs.x);
		//Assign x and y to the calculated cross product components
		x = crossX;
		y = crossY;

		return *this;
	}

	inline rtVec3f rtVec3f::getCrossed(const rtVec3f& rhs) const
	{
		float crossX = (y * rhs.z) - (z * rhs.y);
		//Simplified version of -((x * rhs.z) - (z * rhs.x))
		float crossY = (z * rhs.x) - (x * rhs.z);
		float crossZ = (x * rhs.y) - (y * rhs.x);

		return rtVec3f(crossX, crossY, crossZ);
	}

	inline float rtVec3f::dot(const rtVec3f& rhs) const
	{
		return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
	}

	//Operators
	//Copy Assignment
	inline rtVec3f& rtVec3f::operator=(const rtVec3f& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;

		return *this;
	}

	//Comparison
	inline bool rtVec3f::operator==(const rtVec3f& rhs)
	{
		return ((x == rhs.x) && (y == rhs.y) && (z == rhs.z));
	}

	inline bool rtVec3f::operator!=(const rtVec3f& rhs)
	{
		return ((x != rhs.x) || (y != rhs.y) || (z != rhs.z));
	}


	//Scalar Addition
	inline rtVec3f rtVec3f::operator+(float value) const
	{
		return rtVec3f(x + value, y + value, z + value);
	}

	inline rtVec3f& rtVec3f::operator+=(float value)
	{
		x += value;
		y += value;
		z += value;

		return *this;
	}

	//Vector Addition
	inline rtVec3f rtVec3f::operator+(const rtVec3f& rhs) const
	{
		return rtVec3f(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	inline rtVec3f& rtVec3f::operator+=(const rtVec3f& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;

		return *this;
	}

	//Scalar Subtraction
	inline rtVec3f rtVec3f::operator-(float value) const
	{
		return rtVec3f(x - value, y - value, z - value);
	}

	inline rtVec3f& rtVec3f::operator-=(float value)
	{
		x -= value;
		y -= value;
		z -= value;

		return *this;
	}

	//Vector Subtraction
	inline rtVec3f rtVec3f::operator-(const rtVec3f& rhs) const
	{
		return rtVec3f(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	inline rtVec3f& rtVec3f::operator-=(const rtVec3f& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;

		return *this;
	}

	//Scalar Multiplication
	inline rtVec3f rtVec3f::operator*(float value) const
	{
		return rtVec3f(x * value, y * value, z * value);
	}

	inline rtVec3f& rtVec3f::operator*=(float value)
	{
		x *= value;
		y *= value;
		z *= value;

		return *this;
	}

	//Vector Multiplication
	inline rtVec3f rtVec3f::operator*(const rtVec3f& rhs) const
	{
		return rtVec3f(x * rhs.x, y * rhs.y, z * rhs.z);
	}

	inline rtVec3f& rtVec3f::operator*=(const rtVec3f& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;

		return *this;
	}

	//Scalar Division
	inline rtVec3f rtVec3f::operator/(float value) const
	{
		if (value == 0)
			return rtVec3f(NAN, NAN, NAN);

		return rtVec3f(x * value, y * value, z * value);
	}

	inline rtVec3f& rtVec3f::operator/=(float value)
	{
		if (value == 0)
		{
			set(NAN);
		}
		else
		{
			x /= value;
			y /= value;
			z /= value;
		}

		return *this;
	}

	//Vector Division
	inline rtVec3f rtVec3f::operator/(const rtVec3f& rhs) const
	{
		float divX = (rhs.x == 0) ? NAN : x / rhs.x;
		float divY = (rhs.y == 0) ? NAN : y / rhs.y;
		float divZ = (rhs.z == 0) ? NAN : z / rhs.z;

		return rtVec3f(divX, divY, divZ);
	}

	inline rtVec3f& rtVec3f::operator/=(const rtVec3f& rhs)
	{
		x = (rhs.x == 0) ? NAN : x / rhs.x;
		y = (rhs.y == 0) ? NAN : y / rhs.y;
		z = (rhs.z == 0) ? NAN : z / rhs.z;

		return *this;
	}

	//Unary Negation
	inline rtVec3f rtVec3f::operator-() const
	{
		return rtVec3f(-x, -y, -z);
	}
}