#pragma once

typedef unsigned char uint8;

struct Color {
	uint8 r, g, b, a;
	Color(uint8 r, uint8 g, uint8 b, uint8 a=0) :
		r(r), g(g), b(b), a(a)
	{}
	Color(): 
		r(0), g(0), b(0), a(255)
	{}
	bool isVisible(){
		return  a != 0;
	}

	void interpolate(const Color& color, int fraction) {
		r = (color.r - r) * fraction + r;
		g = (color.g - g) * fraction + g;
		b = (color.b - b) * fraction + b;
		a = (color.a - a) * fraction + a;
	}
};