#pragma once

namespace rtGraphics
{
	//Represents an RGBA color using integers in the range 0-255
	class rtColor
	{
	private:
		int r, g, b, a;
		const static int maxValue = 255;

		//Keep the r, g, b, and a values between 0 and 1
		void clampColors();

	public:
		///Constructors
		rtColor();
		rtColor(int value);
		rtColor(int r, int g, int b, int a = 1);

		///Color Methods
		//Returns the given value 
		static int clampColor(int value);

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
		void setColor(int r, int g, int b, int a = 1);
	};

	///Constructors
	//The default color is an opaque black
	rtColor::rtColor() : rtColor(0, 0, 0) {}
	rtColor::rtColor(int value) : rtColor(value, value, value) {}

	rtColor::rtColor(int r, int g, int b, int a = 1)
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

	inline void rtColor::setColor(int r, int g, int b, int a = 1)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;

		clampColors();
	}
}