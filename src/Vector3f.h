/*
* Author      : Joao Carlos jcfgonc@gmail.com
* License     : MIT License
 * started on 03-04-2007
 * checked on 14-04-2007: OK, clean
 */
#ifndef VECTOR_3F_HEADER
#define VECTOR_3F_HEADER

#include <math.h>
#include<gl/GL.h>

class Vector3f {
private:
	const size_t stringbuf_size = 256;
	char* stringbuf;
	float* returnedArray; // array returned to calling functions
public:

	float x, y, z;

	void initialize(float x0, float y0, float z0) {
		this->stringbuf = new char[stringbuf_size];
		this->returnedArray = new float[4];
		x = x0;
		y = y0;
		z = z0;
	}

	Vector3f(float x0, float y0, float z0) {
		initialize(x0, y0, z0);
	}

	Vector3f() {
		initialize(0, 0, 0);
	}

	Vector3f(Vector3f* org) {
		initialize(org->x, org->y, org->z);
	}

	~Vector3f() {
		delete stringbuf;
		delete returnedArray;
	}

	char* toString() {
		snprintf(stringbuf, stringbuf_size, "%f,%f,%f", x, y, z);
		return stringbuf;
	}

	/* VECTOR <-> VECTOR OPERATIONS */
	void add(Vector3f* b) {
		x = x + b->x;
		y = y + b->y;
		z = z + b->z;
	}

	void subtract(Vector3f* b) {
		x = x - b->x;
		y = y - b->y;
		z = z - b->z;
	}

	/**
	* caller must free the returned data when it is no longer needed
	* UNUSED
	*/
	Vector3f* crossProduct(Vector3f* b) {
		return (new Vector3f(-z * b->y + y * b->z, z * b->x - x * b->z, -y * b->x + x * b->y));
	}

	float dotProduct(Vector3f* a) {
		return (x * a->x + y * a->y + z * a->z);
	}

	/* VECTOR OPERATIONS */
	void negate() {
		x = -x;
		y = -y;
		z = -z;
	}

	float length() {
		return (sqrtf(x * x + y * y + z * z));
	}

	void normalize() {
		float l = length();
		x = x / l;
		y = y / l;
		z = z / l;
	}

	/**
	* caller must free the returned data when it is no longer needed
	* UNUSED
	*/
	Vector3f* createRightVector() {
		Vector3f* right = new Vector3f(z, -x, y);
		float d = dotProduct(right);
		right->addScaledByf(this, -d);
		right->normalize();
		return (right);
	}

	void debug() {
		printf("(%f, %f, %f)\n", x, y, z);
	}

	GLfloat* to3GLfloat() {
		returnedArray[0] = (GLfloat) x;
		returnedArray[1] = (GLfloat) y;
		returnedArray[2] = (GLfloat) z;
		returnedArray[3] = (GLfloat) 0;
		return returnedArray;
	}

	GLfloat* to4GLfloatW1() {
		returnedArray[0] = (GLfloat) x;
		returnedArray[1] = (GLfloat) y;
		returnedArray[2] = (GLfloat) z;
		returnedArray[3] = (GLfloat) 1;
		return returnedArray;
	}

	GLfloat* to4GLfloatW0() {
		returnedArray[0] = (GLfloat) x;
		returnedArray[1] = (GLfloat) y;
		returnedArray[2] = (GLfloat) z;
		returnedArray[3] = (GLfloat) 0;
		return returnedArray;
	}

	/* VECTOR <-> SCALAR OPERATIONS */
	void rotateAxisX(float t) {
		//rotates on X axis
		float cost = cos(t);
		float sint = sin(t);
		y = cost * y - sint * z;
		z = sint * y + cost * z;
	}

	void rotateAxisY(float t) {
		//rotates on Y axis
		float cost = cos(t);
		float sint = sin(t);
		x = cost * x + sint * z;
		z = -sint * x + cost * z;
	}

	void rotateAxisZ(float t) {
		//rotates on Z axis
		float cost = cos(t);
		float sint = sin(t);
		x = cost * x - sint * y;
		y = sint * x + cost * y;
	}

	void rotateVector(float t, float vx, float vy, float vz) {
		//rotates on specified (arbitrary) vector
		float cost = cos(t);
		float sint = sin(t);
		float oldx = x;
		float oldy = y;
		float oldz = z;

		x = (cost + (1 - cost) * vx * vx) * oldx + ((1 - cost) * vx * vy - vz * sint) * oldy
				+ ((1 - cost) * vx * vz + vy * sint) * oldz;

		y = ((1 - cost) * vx * vy + vz * sint) * oldx + (cost + (1 - cost) * vy * vy) * oldy
				+ ((1 - cost) * vy * vz - vx * sint) * oldz;

		z = ((1 - cost) * vx * vz - vy * sint) * oldx + ((1 - cost) * vy * vz + vx * sint) * oldy
				+ (cost + (1 - cost) * vz * vz) * oldz;
	}

	void multiply(float b) {
		x = x * b;
		y = y * b;
		z = z * b;
	}

	void divide(float b) {
		x = x / b;
		y = y / b;
		z = z / b;
	}

	void setup(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}

	/* VECTOR <-> VECTOR OPERATIONS */
	void add(float x0, float y0, float z0) {
		x = x + x0;
		y = y + y0;
		z = z + z0;
	}

	void subtract(float x0, float y0, float z0) {
		x = x - x0;
		y = y - y0;
		z = z - z0;
	}

	/* MULTITYPE OPERATIONS */
	void addScaledByf(Vector3f* scaled, float scale) {
		x = scaled->x * scale + x;
		y = scaled->y * scale + y;
		z = scaled->z * scale + z;
	}

	void rotateVector(float t, Vector3f* base) {
		rotateVector(t, base->x, base->y, base->z);
	}
};
#endif
