/*
* Author      : Joao Carlos jcfgonc@gmail.com
* License     : MIT License
 * started on 04-05-2007 as an way to organize data
 */
#ifndef ENGINE_DATA_HEADER
#define ENGINE_DATA_HEADER

#include "Keyboard.h"
#include "Camera.h"
#include "CameraPathFollower.h"
#include "Light.h"
#include "LightMotion.h"

class EngineData {
public:

	GLuint scene_display_list;

	long timebase; //current frame rendering time (ms)
	long ms_per_frame; //self explanatory
	GLsizei window_width, window_height; //window size
	double internaltime, internaltimedelta; //engine internal time (for physics & motion)

	bool autocamera; //auto camera?
	bool window_resized; //window has been resized

	Keyboard *keyboard; //keyboard class
	Camera *camera; //manual camera class
	CameraPathFollower* campath; //autocamera path class
	Light *light0, *light1; //light 1 & 2 classes
	LightMotion *lightmotion0, *lightmotion1; //light 1 & 2 path classes

	GLint gl_max_texture_size;

	//shadow stuff
	GLuint shadowMapTexture;
	GLsizei shadowMapSize;

	// 3d model for the scene
	GLMmodel *model_scene;

	// texture id for the shadowmap
	GLuint depthTextureId;

	//matrices
	GLfloat* cameraProjectionMatrix;
	GLfloat* cameraViewMatrix;

	GLuint shadow_fboid;
};
#endif
