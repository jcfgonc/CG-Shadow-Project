/*
* Author      : Joao Carlos jcfgonc@gmail.com
* License     : MIT License
* started on 02-04-2007
* checked on 14-04-2007: OK, clean
*/
#ifndef CAMERA_HEADER
#define CAMERA_HEADER

#include <math.h>
#include "Vector3f.h"
#include <GL/glut.h>

class Camera {
public:
	Vector3f* position;
	Vector3f* direction;
	Vector3f* up;
	Vector3f* right;	//optional, can be calculated using cross product
	int debuggedn;

	Camera(bool uninitalized) {
		//number of debugged times
		debuggedn = 0;
		if(!uninitalized)
		{
			// camera position
			position = new Vector3f();
			// camera direction
			direction = new Vector3f();
			// camera up vector
			up = new Vector3f();
			// camera right vector
			right = new Vector3f();
		}
	}

	Camera(Camera* org)	{
		//copy constructor
		position = new Vector3f(org->position);
		direction = new Vector3f(org->direction);
		up = new Vector3f(org->up);
		right = new Vector3f(org->right);
		debuggedn = org->debuggedn;
	}

	~Camera() {
		// destroy internal data
		delete position;
		delete direction;
		delete up;
		delete right;
	}

	void lookAt() {
		//sets opengl's projection matrix according to this camera
		gluLookAt(
			position->x,
			position->y,
			position->z,
			position->x+direction->x,
			position->y+direction->y,
			position->z+direction->z,
			up->x,
			up->y,
			up->z);
	}

	void rotateViewOnAxisX(float t) {
		//rotates on scene's X axis
		direction->rotateAxisX(t);
		right->rotateAxisX(t);
		up->rotateAxisX(t);
	}

	void rotateViewOnAxisY(float t) {
		//rotates on scene's Y axis
		direction->rotateAxisY(t);
		right->rotateAxisY(t);
		up->rotateAxisY(t);
	}

	void rotateViewOnAxisZ(float t) {
		//rotates on scene's Z axis
		direction->rotateAxisZ(t);
		right->rotateAxisZ(t);
		up->rotateAxisZ(t);
	}

	void rotateViewVirtualX(float t) {
		//rotates on camera's X axis
		direction->rotateVector(t,right);
		up->rotateVector(t,right);
	}

	void rotateViewVirtualY(float t) {
		//rotates on camera's Y axis
		direction->rotateVector(t,up);
		right->rotateVector(t,up);
	}

	void rotateViewVirtualZ(float t) {
		//rotates on camera's Z axis
		right->rotateVector(t,direction);
		up->rotateVector(t,direction);
	}

	void rotateViewOnVector(float t, Vector3f* v) {
		//rotates on specified vector
		direction->rotateVector(t,v);
	}

	void rotateViewOnVector(float t, float x, float y, float z) {
		//rotates on specified vector
		direction->rotateVector(t,x,y,z);
	}

	void moveForwardBackward(float scale) {
		//Moves forward the direction vector
		position->addScaledByf(direction, scale);
	}

	void moveLeftRight(float scale) {
		position->addScaledByf(right, scale);
	}

	void moveUpDown(float scale) {
		position->addScaledByf(up, scale);
	}

	void debug() {
		//outputs important camera stuff
		printf("Vector3f* position%d = new Vector3f%s;\n",
			debuggedn,
			position->toString());
		printf("Vector3f* direction%d = new Vector3f%s;\n",
			debuggedn,
			direction->toString());
		printf("Vector3f* up%d = new Vector3f%s;\n",
			debuggedn,
			up->toString());
		//just got debugged one more time, mate :D
		debuggedn++;
	}

	void debugSimple() {
		//outputs important camera stuff
		printf("%s\n",
			position->toString());
		printf("%s\n",
			direction->toString());
		printf("%s\n",
			up->toString());
	}
};
#endif
