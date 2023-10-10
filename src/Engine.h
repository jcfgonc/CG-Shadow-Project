/*
* Author      : Joao Carlos jcfgonc@gmail.com
* License     : MIT License
 * 02-04-2007
 */
#include <fstream>
extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>
	//GL header file, including extensions
#include "GLee.h"
#include <GL/glut.h>
//glm supports the loading of wavefront objects and their materials
#include "glm.h"
}

//CLASSES
#include "Vector3f.h"
#include "Color4f.h"
#include "Light.h"
#include "Camera.h"
#include "CameraPathFollower.h"
#include "Vector3fSplineInterpolator.h"
#include "LightMotion.h"
#include "Keyboard.h"
#include "EngineData.h"

//Definitions
#include "EngineDefinitions.h"

void checkForErrors();

/* light which illuminates shadows */
void shadowingLight(bool status);

/* light which casts shadows */
void shadowCastingLight(bool status);

/* Creates and calls opengl lighting calls for all lights */
void fullLighting();

/* sets up only shadowing light */
void shadowLighting();
