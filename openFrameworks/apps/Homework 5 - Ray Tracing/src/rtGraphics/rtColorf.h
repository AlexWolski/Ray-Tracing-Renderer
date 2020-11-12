#pragma once

namespace rtGraphics
{
	//Represents an RGBA color using integers in the range 0-1
	class rtColorf
	{
	private:
		float r, g, b, a;
		const static float maxValue;

	public:
		///Constructors
		rtColorf();
		rtColorf(float value);
		rtColorf(float r, float g, float b, float a = maxValue);

		///Color Methods
		//Returns the given value clamped between 0 and the maximum
		static float clampColor(float value);
		//Clamp the r, g, b, and a values between 0 and the maximum
		void clampColors();

		///Getters
		float getR() const;
		float getG() const;
		float getB() const;
		float getA() const;

		///Setters
		void setR(float r);
		void setG(float g);
		void setB(float b);
		void setA(float a);
		void setColor(float r, float g, float b, float a = maxValue);

		///Operators
		rtColorf  operator+ (const rtColorf& rhs) const;
		rtColorf& operator+=(const rtColorf& rhs);
		rtColorf  operator+ (float scalar) const;
		rtColorf& operator+=(float scalar);
		rtColorf  operator- (const rtColorf& rhs) const;
		rtColorf& operator-=(const rtColorf& rhs);
		rtColorf  operator- (float scalar) const;
		rtColorf& operator-=(float scalar);
		rtColorf  operator* (const rtColorf& rhs) const;
		rtColorf& operator*=(const rtColorf& rhs);
		rtColorf  operator* (float scalar) const;
		rtColorf& operator*=(float scalar);
		rtColorf  operator/ (const rtColorf& rhs) const;
		rtColorf& operator/=(const rtColorf& rhs);
		rtColorf  operator/ (float scalar) const;
		rtColorf& operator/=(float scalar);

		///Static colors
		const static rtColorf white, grey, black, red, green, blue, yellow, cyan, magenta;
	};

	///Constructors
	//The default color is an opaque black
	inline rtColorf::rtColorf() : rtColorf(0.0f, 0.0f, 0.0f) {}
	inline rtColorf::rtColorf(float value) : rtColorf(value, value, value) {}

	inline rtColorf::rtColorf(float r, float g, float b, float a)
	{
		setColor(r, g, b, a);
		clampColors();
	}

	///In-line method definitions
	//Color Methods
	inline float rtColorf::clampColor(float value)
	{
		if (value > maxValue)
			return maxValue;

		if (value < 0.0f)
			return 0.0f;

		return value;
	}

	inline void rtColorf::clampColors()
	{
		r = clampColor(r);
		g = clampColor(g);
		b = clampColor(b);
		a = clampColor(a);
	}

	//Getters
	inline float rtColorf::getR() const { return r; }
	inline float rtColorf::getG() const { return g; }
	inline float rtColorf::getB() const { return b; }
	inline float rtColorf::getA() const { return a; }

	//Setters
	inline void rtColorf::setR(float r) { this->r = clampColor(r); }
	inline void rtColorf::setG(float g) { this->g = clampColor(g); }
	inline void rtColorf::setB(float b) { this->b = clampColor(b); }
	inline void rtColorf::setA(float a) { this->a = clampColor(a); }

	inline void rtColorf::setColor(float r, float g, float b, float a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;

		clampColors();
	}

	///Operators
	//Component Addition
	inline rtColorf rtColorf::operator+(const rtColorf& rhs) const
	{
		return rtColorf(r + rhs.r, g + rhs.g, b + rhs.b, a + rhs.a);
	}

	inline rtColorf& rtColorf::operator+=(const rtColorf& rhs)
	{
		r += rhs.r;
		g += rhs.g;
		b += rhs.b;
		a += rhs.a;

		return *this;
	}

	//Scalar Addition
	inline rtColorf rtColorf::operator+(float scalar) const
	{
		return rtColorf(r + scalar, g + scalar, b + scalar, a + scalar);
	}

	inline rtColorf& rtColorf::operator+=(float scalar)
	{
		r += scalar;
		g += scalar;
		b += scalar;
		a += scalar;

		return *this;
	}

	//Component Subtraction
	inline rtColorf rtColorf::operator-(const rtColorf& rhs) const
	{
		return rtColorf(r - rhs.r, g - rhs.g, b - rhs.b, a - rhs.a);
	}

	inline rtColorf& rtColorf::operator-=(const rtColorf& rhs)
	{
		r -= rhs.r;
		g -= rhs.g;
		b -= rhs.b;
		a -= rhs.a;

		return *this;
	}

	//Scalar Subtraction
	inline rtColorf rtColorf::operator-(float scalar) const
	{
		return rtColorf(r - scalar, g - scalar, b - scalar, a - scalar);
	}

	inline rtColorf& rtColorf::operator-=(float scalar)
	{
		r -= scalar;
		g -= scalar;
		b -= scalar;
		a -= scalar;

		return *this;
	}

	//Component Multiplication
	inline rtColorf rtColorf::operator*(const rtColorf& rhs) const
	{
		return rtColorf(r * rhs.r, g * rhs.g, b * rhs.b, a * rhs.a);
	}

	inline rtColorf& rtColorf::operator*=(const rtColorf& rhs)
	{
		r *= rhs.r;
		g *= rhs.g;
		b *= rhs.b;
		a *= rhs.a;

		return *this;
	}

	//Scalar Multiplication
	inline rtColorf rtColorf::operator*(float scalar) const
	{
		return rtColorf(r * scalar, g * scalar, b * scalar, a * scalar);
	}

	inline rtColorf& rtColorf::operator*=(float scalar)
	{
		r *= scalar;
		g *= scalar;
		b *= scalar;
		a *= scalar;

		return *this;
	}

	//Component Division
	inline rtColorf rtColorf::operator/(const rtColorf& rhs) const
	{
		return rtColorf(r / rhs.r, g / rhs.g, b / rhs.b, a / rhs.a);
	}

	inline rtColorf& rtColorf::operator/=(const rtColorf& rhs)
	{
		r /= rhs.r;
		g /= rhs.g;
		b /= rhs.b;
		a /= rhs.a;

		return *this;
	}

	//Scalar Division
	inline rtColorf rtColorf::operator/(float scalar) const
	{
		return rtColorf(r / scalar, g / scalar, b / scalar, a / scalar);
	}

	inline rtColorf& rtColorf::operator/=(float scalar)
	{
		r /= scalar;
		g /= scalar;
		b /= scalar;
		a /= scalar;

		return *this;
	}
}