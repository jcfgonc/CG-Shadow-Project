/*
* Author      : Joao Carlos jcfgonc@gmail.com
* License     : MIT License
* 03-04-2007
* checked on 14-04-2007: OK, clean
*/
#ifndef COLOR_4F_HEADER
#define COLOR_4F_HEADER

#ifndef GLfloat
typedef float GLfloat;
#endif

class Color4f {
public:
	float red, green, blue, alpha;
	float* array;

	Color4f() {
		initialize(0, 0, 0, 1);
	}

	Color4f(float red_, float green_, float blue_, float alpha_) {
		initialize(red_, green_, blue_, alpha_);
	}

	void initialize(float red_, float green_, float blue_, float alpha_) {
		red = red_;
		green = green_;
		blue = blue_;
		alpha = alpha_;
		array = new float[4];
	}

	~Color4f() {
		delete[] array;
	}

	void scale(float sr, float sg, float sb, float sa) {
		red *= sr;
		green *= sg;
		blue *= sb;
		alpha *= sa;
	}

	void scale(float sr, float sg, float sb) {
		scale(sr, sg, sb, 1.0);
	}

	void scale(float ss, float sa) {
		scale(ss, ss, ss, sa);
	}

	void scale(float ss) {
		scale(ss, ss, ss, 1.0);
	}

	void bias(float sr, float sg, float sb, float sa) {
		red += sr;
		green += sg;
		blue += sb;
		alpha += sa;
	}

	void bias(float sr, float sg, float sb) {
		bias(sr, sg, sb, 1.0);
	}

	void bias(float ss, float sa) {
		bias(ss, ss, ss, sa);
	}

	void bias(float ss) {
		bias(ss, ss, ss, 1.0);
	}

	GLfloat* to4GLfloat() {
		array[0] = (GLfloat) red;
		array[1] = (GLfloat) green;
		array[2] = (GLfloat) blue;
		array[3] = (GLfloat) alpha;
		return array;
	}
};
#endif
