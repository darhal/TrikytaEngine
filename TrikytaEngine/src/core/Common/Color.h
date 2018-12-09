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

	void interpolate(const Color& color, float fraction) {
		r = int((color.r - r) * fraction) + r;
		g = int((color.g - g) * fraction) + g;
		b = int((color.b - b) * fraction) + b;
		a = int((color.a - a) * fraction) + a;
	}

	int difference(const Color& color) {
		return (r - color.r) + (g - color.g) + (b - color.b) + (a - color.a);
	}
};