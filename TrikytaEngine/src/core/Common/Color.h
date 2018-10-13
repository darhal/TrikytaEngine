#pragma once

typedef unsigned char uint8;

struct Color {
	uint8 r, g, b, a;
	Color(uint8 r, uint8 g, uint8 b, uint8 a=0) :
		r(r), g(g), b(b), a(a)
	{}
	bool isVisible(){
		return  a != 0;
	}
};