#pragma once

namespace rtGraphics
{
	//Represents an RGBA color using integers in the range 0-255
	class rtColor
	{
	private:
		int r, g, b, a;
		const static int maxValue = 255;

	public:
		///Constructors
		rtColor();
		rtColor(int value);
		rtColor(int r, int g, int b, int a = maxValue);

		///Color Methods
		//Returns the given value clamped between 0 and the maximum
		static int clampColor(int value);
		//Clamp the r, g, b, and a values between 0 and the maximum
		void clampColors();

		///Getters
		int getR() const;
		int getG() const;
		int getB() const;
		int getA() const;

		///Setters
		void setR(int r);
		void setG(int g);
		void setB(int b);
		void setA(int a);
		void setColor(int r, int g, int b, int a = maxValue);

		///Operators
		rtColor  operator+ (const rtColor& rhs) const;
		rtColor& operator+=(const rtColor& rhs);
		rtColor  operator- (const rtColor& rhs) const;
		rtColor& operator-=(const rtColor& rhs);
		rtColor  operator* (const rtColor& rhs) const;
		rtColor& operator*=(const rtColor& rhs);
		rtColor  operator/ (const rtColor& rhs) const;
		rtColor& operator/=(const rtColor& rhs);
	};

	///Constructors
	//The default color is an opaque black
	inline rtColor::rtColor() : rtColor(0, 0, 0) {}
	inline rtColor::rtColor(int value) : rtColor(value, value, value) {}

	inline rtColor::rtColor(int r, int g, int b, int a)
	{
		setColor(r, g, b, a);
		clampColors();
	}

	///In-line method definitions
	//Color Methods
	inline int rtColor::clampColor(int value)
	{
		if (value > maxValue)
			return maxValue;

		if (value < 0)
			return 0;

		return value;
	}

	inline void rtColor::clampColors()
	{
		r = clampColor(r);
		g = clampColor(g);
		b = clampColor(b);
		a = clampColor(a);
	}

	//Getters
	inline int rtColor::getR() const { return r; }
	inline int rtColor::getG() const { return g; }
	inline int rtColor::getB() const { return b; }
	inline int rtColor::getA() const { return a; }

	//Setters
	inline void rtColor::setR(int r) { this->r = clampColor(r); }
	inline void rtColor::setG(int g) { this->g = clampColor(g); }
	inline void rtColor::setB(int b) { this->b = clampColor(b); }
	inline void rtColor::setA(int a) { this->a = clampColor(a); }

	inline void rtColor::setColor(int r, int g, int b, int a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;

		clampColors();
	}

	///Operators
	//Addition
	inline rtColor rtColor::operator+(const rtColor& rhs) const
	{
		return rtColor(r + rhs.r, g + rhs.g, b + rhs.b, a + rhs.a);
	}

	inline rtColor& rtColor::operator+=(const rtColor& rhs)
	{
		r += rhs.r;
		g += rhs.g;
		b += rhs.b;
		a += rhs.a;

		return *this;
	}

	//Subtraction
	inline rtColor rtColor::operator-(const rtColor& rhs) const
	{
		return rtColor(r - rhs.r, g - rhs.g, b - rhs.b, a - rhs.a);
	}

	inline rtColor& rtColor::operator-=(const rtColor& rhs)
	{
		r -= rhs.r;
		g -= rhs.g;
		b -= rhs.b;
		a -= rhs.a;

		return *this;
	}

	//Multiplication
	inline rtColor rtColor::operator*(const rtColor& rhs) const
	{
		return rtColor(r * rhs.r, g * rhs.g, b * rhs.b, a * rhs.a);
	}

	inline rtColor& rtColor::operator*=(const rtColor& rhs)
	{
		r *= rhs.r;
		g *= rhs.g;
		b *= rhs.b;
		a *= rhs.a;

		return *this;
	}

	//Division
	inline rtColor rtColor::operator/(const rtColor& rhs) const
	{
		return rtColor(r / rhs.r, g / rhs.g, b / rhs.b, a / rhs.a);
	}

	inline rtColor& rtColor::operator/=(const rtColor& rhs)
	{
		r /= rhs.r;
		g /= rhs.g;
		b /= rhs.b;
		a /= rhs.a;

		return *this;
	}
}