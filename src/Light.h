/*
* Author      : Joao Carlos jcfgonc@gmail.com
* License     : MIT License
 * started on 21-04-2007
 *
 * HAS MEMORY LEAKS
 */
#ifndef LIGHT_HEADER
#define LIGHT_HEADER

// I dont like this inclusion
#include <gl/glut.h>

#include "Color4f.h"
#include "Vector3f.h"
#include "EngineDefinitions.h"

class Light {
private:
	Color4f* diffuse;
	Color4f* ambient;
	Color4f* specular;

	bool directional;

	float spot_exponent;
	float spot_angle;

	float attenuation_constant;
	float attenuation_linear;
	float attenuation_quadratic;

	GLenum light_number;


public:
	//light local axis
	Vector3f* position;
	Vector3f* direction;
	Vector3f* up;
	
	//matrices
	GLfloat* lightProjectionMatrix;
	GLfloat* lightViewMatrix;
	
	Light(GLenum _light_number, float r, float g, float b, float position_x, float position_y, float position_z,
			float direction_x, float direction_y, float direction_z, bool _directional, float _spot_exponent, float _spot_angle,
			float _attenuation_constant, float _attenuation_linear, float _attenuation_quadratic) {
		diffuse = new Color4f(r, g, b, 1.0f);
		ambient = new Color4f(0.0f, 0.0f, 0.0f, 1.0f);
		specular = new Color4f(r, g, b, 1.0f);

		// this is set in LightMotion
	//	position = new Vector3f(position_x, position_y, position_z);
	//	direction = new Vector3f(direction_x, direction_y, direction_z);
	//	up = new Vector3f(0, 0, 0);

		directional = _directional;
		spot_exponent = _spot_exponent;
		spot_angle = _spot_angle;

		attenuation_constant = _attenuation_constant;
		attenuation_linear = _attenuation_linear;
		attenuation_quadratic = _attenuation_quadratic;

		light_number = _light_number;

		lightProjectionMatrix = new GLfloat[16];
		lightViewMatrix = new GLfloat[16];
	}

	~Light() {
		delete diffuse;
		delete ambient;
		delete specular;

		// this is deleted in LightMotion
		//delete position;
		//delete direction;
		//delete up;

		delete lightProjectionMatrix;
		delete lightViewMatrix;
	}

	void turnState(bool status) {
		if (status)
			glEnable(light_number);
		else
			glDisable(light_number);
	}

	void setupLightOpenGL() {
		// COLORS: AMBIENT, DIFFUSE, SPECULAR
		glLightfv(light_number, GL_AMBIENT, ambient->to4GLfloat());
		glLightfv(light_number, GL_DIFFUSE, diffuse->to4GLfloat());
		glLightfv(light_number, GL_SPECULAR, specular->to4GLfloat());
		// VECTORS: POSITION, DIRECTION
		if (directional)
			glLightfv(light_number, GL_POSITION, position->to4GLfloatW0());
		else
			glLightfv(light_number, GL_POSITION, position->to4GLfloatW1());
		glLightfv(light_number, GL_SPOT_DIRECTION, direction->to3GLfloat());
		// SPOTLIGHT: EXPONENT, CUTOFF
		glLightf(light_number, GL_SPOT_EXPONENT, spot_exponent);
		glLightf(light_number, GL_SPOT_CUTOFF, spot_angle);
		// ATTENUATION: CONSTANT, LINEAR, QUADRATIC
		glLightf(light_number, GL_CONSTANT_ATTENUATION, attenuation_constant);
		glLightf(light_number, GL_LINEAR_ATTENUATION, attenuation_linear);
		glLightf(light_number, GL_QUADRATIC_ATTENUATION, attenuation_quadratic);
	}

	void cameraAtLight() {
		//set projection according to spotlight angle
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluPerspective(45, 1.0, ZBUFFER_MIN, ZBUFFER_MAX);

		glGetFloatv(GL_PROJECTION_MATRIX, lightProjectionMatrix);
		glPopMatrix();

		//set world relative position of light
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		gluLookAt(position->x, position->y, position->z, position->x + direction->x, position->y + direction->y,
				position->z + direction->z, up->x, up->y, up->z);
		glGetFloatv(GL_MODELVIEW_MATRIX, lightViewMatrix);
		glPopMatrix();
	}
};
#endif
