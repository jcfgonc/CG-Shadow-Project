/*
* Author      : Joao Carlos jcfgonc@gmail.com
* License     : MIT License
 * 02-04-2007
 */

#include "Engine.h"

EngineData* data;

//ENGINE
GLfloat* matrix_bias;
GLfloat* matrix_product;

bool fullscreen;

/* Sets up an sphere and a line at a ligh's location */
void createLightBulb(Light* _l, GLfloat radius, GLuint sections) {
	GLfloat mat_1[] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_1);

	glPushMatrix();
	glTranslatef(_l->position->x, _l->position->y, _l->position->z);
	// sphere on cams position
	glutSolidSphere(radius, sections, sections);
	glLineWidth(2.0);
	glBegin(GL_LINES);
	// line showing its direction
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(_l->direction->x, _l->direction->y, _l->direction->z);
	glEnd();
	glPopMatrix();
}

void createShadowTexture() {

	glActiveTextureARB(GL_TEXTURE1_ARB);
	//generate texture
	glGenTextures(1, &(data->shadowMapTexture));
	//associate current texture unit
	glBindTexture(GL_TEXTURE_2D, data->shadowMapTexture);
	//setup it (must have been bind before)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, data->shadowMapSize, data->shadowMapSize, 0, GL_DEPTH_COMPONENT,
			GL_UNSIGNED_BYTE, NULL);
	//minification/magnification parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	//Tell OpenGL we want to compare on a per-pixel basis
	//configure the depth-map texture object to perform the depth comparison
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	//Tell OpenGL what sort of texture generation to use
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	//Shadow comparison should be true (ie not in shadow) if r<=texture
	//After comparing the generated r value with the stored depth-map value,
	//tell opengl what to do with the boolean value
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);

	// create a framebuffer object
	glGenFramebuffersEXT(1, &(data->shadow_fboid));
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, data->shadow_fboid);

	// tell opengl the fbo won't have color info
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// connect the texture to the fbo's depth buffer
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, data->shadowMapTexture, 0);

	// check if fbo is binded
	GLenum fbostat = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	checkForErrors();
	if (fbostat != GL_FRAMEBUFFER_COMPLETE_EXT) {
		printf("GL_FRAMEBUFFER_COMPLETE_EXT failed, can't use FBO\n");
		exit(-1);
	} else {
		printf("created frame buffer object (FBO)\n");
	}

	// switch back to window-system-provided framebuffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

/**
 * gets the row (at position) of the given opengl matrix
 */
void getMatrixRow(GLfloat* row4f, GLfloat* matrix4x4, int position) {
	if (position == 0) {
		row4f[0] = matrix4x4[0];
		row4f[1] = matrix4x4[4];
		row4f[2] = matrix4x4[8];
		row4f[3] = matrix4x4[12];
	} else if (position == 1) {
		row4f[0] = matrix4x4[1];
		row4f[1] = matrix4x4[5];
		row4f[2] = matrix4x4[9];
		row4f[3] = matrix4x4[13];
	} else if (position == 2) {
		row4f[0] = matrix4x4[2];
		row4f[1] = matrix4x4[6];
		row4f[2] = matrix4x4[10];
		row4f[3] = matrix4x4[14];
	} else if (position == 3) {
		row4f[0] = matrix4x4[3];
		row4f[1] = matrix4x4[7];
		row4f[2] = matrix4x4[11];
		row4f[3] = matrix4x4[15];
	}
}

void createTextureProjectionMatrix() {
	//Calculate texture matrix for projection from camera to light
	//Scale*LighProjection*LightView
	//do it in with opengl transforms
	//likely it's worse than using the CPU but it depends on the opengl driver
	//work in the model view matrix
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadMatrixf(matrix_bias);
	LightMotion* lm = data->lightmotion0;
	Light* l = lm->getLight();
	glMultMatrixf(l->lightProjectionMatrix);
	glMultMatrixf(l->lightViewMatrix);
	//save product matrix
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix_product);
	//and restore previous matrix
	glPopMatrix();

	//for your eye's viewpoint, setup your texture planes
	GLfloat row4f_0[4];
	GLfloat row4f_1[4];
	GLfloat row4f_2[4];
	GLfloat row4f_3[4];
	getMatrixRow(row4f_0, matrix_product, 0);
	getMatrixRow(row4f_1, matrix_product, 1);
	getMatrixRow(row4f_2, matrix_product, 2);
	getMatrixRow(row4f_3, matrix_product, 3);
	glTexGenfv(GL_S, GL_EYE_PLANE, row4f_0);
	glTexGenfv(GL_T, GL_EYE_PLANE, row4f_1);
	glTexGenfv(GL_R, GL_EYE_PLANE, row4f_2);
	glTexGenfv(GL_Q, GL_EYE_PLANE, row4f_3);
}

void positionCameraAtLight() {
	LightMotion* lm = data->lightmotion0;
	Light* l = lm->getLight();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((GLfloat*) l->lightProjectionMatrix);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((GLfloat*) l->lightViewMatrix);
}

void restoreOriginalCamera() {
	//setup original camera matrices
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((GLfloat*) data->cameraProjectionMatrix);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((GLfloat*) data->cameraViewMatrix);
}

/* Only sets up light which casts shadows */
void positionCameraAtShadowCaster() {
	LightMotion* lm = data->lightmotion0;
	Light* l = lm->getLight();
	//light's flyby
	lm->interpolateMotion((float) data->internaltime);
	//setup light's projection & view matrices
	l->cameraAtLight();
}

/* Draws the scene */
void staticScene(GLuint mode) {
	glmDraw(data->model_scene, mode);
	puts("Scene created");
}

void shadowFirstPass() {
	//First step: Render from the light POV to a FBO, story depth values only
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, data->shadow_fboid);

	//Use view port the same size as the shadow map
	glViewport(0, 0, data->shadowMapSize, data->shadowMapSize);

	// Clear previous frame values
	glClear(GL_DEPTH_BUFFER_BIT);

	//disable color writes, texturing & lighting for fast rendering
	glColorMask(0, 0, 0, 0);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	//z fighting, prevent auto shadowing
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(SHADOW_OFFSET_FACTOR, SHADOW_OFFSET_UNITS);

	//Draw back faces into the shadow map
	glCullFace(GL_FRONT);

	//setup camera at shadow caster light position
	positionCameraAtShadowCaster();

	//load matrices and position camera @ light
	positionCameraAtLight();

	//Draw the scene (flat shading)
	glCallList(data->scene_display_list);

	//Read the depth buffer into the shadow map texture
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D, data->shadowMapTexture);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, data->shadowMapSize, data->shadowMapSize);

	//restore back faces removal
	glCullFace(GL_BACK);

	//enable color writes
	glColorMask(1, 1, 1, 1);
	glDisable(GL_POLYGON_OFFSET_FILL);

	// Now rendering from the camera POV, using the FBO to generate shadows
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	glViewport(0, 0, data->window_width, data->window_height);
}

void shadowSecondPass() {
	//we need an new and clean depth buffer
	glClear(GL_DEPTH_BUFFER_BIT);

	//full view port
	glViewport(0, 0, data->window_width, data->window_height);
	restoreOriginalCamera();

	//enable lighting effects
	glEnable(GL_LIGHTING);

	//Bind & enable shadow map texture
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D, data->shadowMapTexture);
	glEnable(GL_TEXTURE_2D);

	//Enable texture generation for all four texture coordinates
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_GEN_Q);

	//create texture matrix & restore camera
	createTextureProjectionMatrix();
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//OpenGL outputs a Boolean value to indicate the result of the comparison
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	//scene with shadow illumination
	fullLighting();
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glCallList(data->scene_display_list);

	//Disable texture generation
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_Q);
}

void shadowThirdPass() {
	//we need an new and clean depth buffer
	glClear(GL_DEPTH_BUFFER_BIT);

	//full view port
	glViewport(0, 0, data->window_width, data->window_height);
	restoreOriginalCamera();

	//enable lighting effects
	glEnable(GL_LIGHTING);

	//Bind & enable shadow map texture
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D, data->shadowMapTexture);
	glEnable(GL_TEXTURE_2D);

	//Enable texture generation for all four texture coordinates
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_GEN_Q);

	//create texture matrix & restore camera
	createTextureProjectionMatrix();
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//OpenGL outputs a Boolean value to indicate the result of the comparison
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_GEQUAL);

	//scene with shadow illumination
	shadowLighting();
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glCallList(data->scene_display_list);

	//Disable texture generation
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_Q);
}

void displayScene() {
	//working on back buffer
	glReadBuffer(GL_BACK);
	shadowFirstPass();
	//draw lit
	shadowSecondPass();
	//add current to the accumulator
	glAccum(GL_ACCUM, 1.0);
	//draw shadowed
	shadowThirdPass();
	//add current to the accumulator
	glAccum(GL_ACCUM, 1.0);
	//copy accumulator to back buffer
	glAccum(GL_RETURN, 1.0);
}

/* Creates and calls opengl lighting calls for all lights */
void fullLighting() {
	shadowCastingLight(true);
	shadowingLight(true);
}

/* sets up only shadowing light */
void shadowLighting() {
	shadowCastingLight(false);
	shadowingLight(true);
}

void initMaterials(GLfloat ambient_r, GLfloat ambient_g, GLfloat ambient_b, GLfloat ambient_a, GLfloat diffuse_r,
		GLfloat diffuse_g, GLfloat diffuse_b, GLfloat diffuse_a, GLfloat specular_r, GLfloat specular_g, GLfloat specular_b,
		GLfloat specular_a, GLfloat emission_r, GLfloat emission_g, GLfloat emission_b, GLfloat emission_a, GLfloat shininess) {
	GLfloat mat_ambient[] = { ambient_r, ambient_g, ambient_b, ambient_a };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	GLfloat mat_diffuse[] = { diffuse_r, diffuse_g, diffuse_b, diffuse_a };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	GLfloat mat_specular[] = { specular_r, specular_g, specular_b, specular_a };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	GLfloat mat_emission[] = { emission_r, emission_g, emission_b, emission_a };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);
	GLfloat mat_shininess = { shininess };
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

void initLights() {
	//light which casts shadows
	data->light0 = new Light(GL_LIGHT0, 0.99f, 0.99f, 0.99f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, false, 0.0f, 60.0f, 0.5f, 0.005f, 0.0f);
	//light movement structure
	data->lightmotion0 = new LightMotion(data->light0, "shadow_caster.txt");

	//light which illuminates shadows
	data->light1 = new Light(GL_LIGHT1, 0.25f, 0.25f, 0.25f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, false, 1.0f, 90.0f, 0.25f, 0.0f, 0.0f);
	//light movement structure
	data->lightmotion1 = new LightMotion(data->light1, "shadow_light.txt");
}

/* light which illuminates shadows */
void shadowingLight(bool status) {
	initMaterials(0.0f, 0.0f, 0.0f, 0.0f, COLOR_YELLOW, 1.0f, COLOR_YELLOW, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 8.0f);
	LightMotion* lm1 = data->lightmotion1;
	Light* l1 = lm1->getLight();
	//light's flyby
	lm1->interpolateMotion((float) data->internaltime);
	//call opengl lighting config
	l1->setupLightOpenGL();
	//enable light
	l1->turnState(status);
	//light bulb
	createLightBulb(l1, 2.0, 8);
}

/* light which casts shadows */
void shadowCastingLight(bool status) {
	initMaterials(0.0f, 0.0f, 0.0f, 0.0f, COLOR_CYAN, 1.0f, COLOR_CYAN, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 8.0f);
	LightMotion* lm0 = data->lightmotion0;
	Light* l0 = lm0->getLight();
	//light's flyby
	lm0->interpolateMotion((float) data->internaltime);
	//call opengl lighting config
	l0->setupLightOpenGL();
	//enable light
	l0->turnState(status);
	//light bulb
	createLightBulb(l0, 2.0, 8);
}

void toggleFullScreen() {
	fullscreen = !fullscreen;
	if (fullscreen) {
		glutFullScreen();
	}
	else {
		// TODO: restore previous window resolution here (must store it previously)
		glutReshapeWindow(data->window_width, data->window_height);
		glutPositionWindow(0, 0);
	}
}

void createDisplayLists() {
	data->scene_display_list = glGenLists(1);

	//display list of static scene
	glNewList(data->scene_display_list, GL_COMPILE);
	staticScene(GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
	glEndList();
}

void modifiersStatus() {
	int modifier = glutGetModifiers();
	data->keyboard->shift = modifier & GLUT_ACTIVE_SHIFT;
	data->keyboard->alt = modifier & GLUT_ACTIVE_ALT;
	data->keyboard->control = modifier & GLUT_ACTIVE_CTRL;
}

void specialKeyPressed(int key, int x, int y) {
	modifiersStatus();

	if (key == GLUT_KEY_UP) {
		data->keyboard->up = 1;
	} else if (key == GLUT_KEY_DOWN) {
		data->keyboard->down = 1;
	} else if (key == GLUT_KEY_LEFT) {
		data->keyboard->left = 1;
	} else if (key == GLUT_KEY_RIGHT) {
		data->keyboard->right = 1;
	} else if (key == GLUT_KEY_PAGE_UP) {
		data->keyboard->pageup = 1;
	} else if (key == GLUT_KEY_PAGE_DOWN) {
		data->keyboard->pagedown = 1;
	} else if (key == GLUT_KEY_HOME) {
		data->keyboard->home = 1;
	} else if (key == GLUT_KEY_END) {
		data->keyboard->end = 1;
	}
}

void specialKeyReleased(int key, int x, int y) {
	modifiersStatus();

	if (key == GLUT_KEY_UP) {
		data->keyboard->up = 0;
	} else if (key == GLUT_KEY_DOWN) {
		data->keyboard->down = 0;
	} else if (key == GLUT_KEY_LEFT) {
		data->keyboard->left = 0;
	} else if (key == GLUT_KEY_RIGHT) {
		data->keyboard->right = 0;
	} else if (key == GLUT_KEY_PAGE_UP) {
		data->keyboard->pageup = 0;
	} else if (key == GLUT_KEY_PAGE_DOWN) {
		data->keyboard->pagedown = 0;
	} else if (key == GLUT_KEY_HOME) {
		data->keyboard->home = 0;
	} else if (key == GLUT_KEY_END) {
		data->keyboard->end = 0;
	}
}

void normalKeyPressed(unsigned char key, int x, int y) {
	modifiersStatus();

	//qweasdzxcrty
	if (key == 'q' || key == 'Q') {
		data->keyboard->q = 1;
	} else if (key == 'w' || key == 'W') {
		data->keyboard->w = 1;
	} else if (key == 'e' || key == 'E') {
		data->keyboard->e = 1;
	} else if (key == 'a' || key == 'A') {
		data->keyboard->a = 1;
	} else if (key == 's' || key == 'S') {
		data->keyboard->s = 1;
	} else if (key == 'd' || key == 'D') {
		data->keyboard->d = 1;
	} else if (key == 'c' || key == 'C') {
		data->keyboard->c = 1;
		//show current manual camera position
		data->camera->debugSimple();
	} else if (key == 'r' || key == 'R') {
		data->keyboard->r = 1;
	} else if (key == 't' || key == 'T') {
		data->keyboard->t = 1;
	} else if (key == 'y' || key == 'Y') {
		data->keyboard->y = 1;
	}
	//fghvbnuiopjklm
	else if (key == 'f' || key == 'F') {
		data->keyboard->f = 1;
	} else if (key == 'g' || key == 'G') {
		data->keyboard->g = 1;
	} else if (key == 'h' || key == 'H') {
		data->keyboard->h = 1;
	} else if (key == 'v' || key == 'V') {
		data->keyboard->v = 1;
	} else if (key == 'b' || key == 'B') {
		data->keyboard->b = 1;
	} else if (key == 'n' || key == 'N') {
		data->keyboard->n = 1;
	} else if (key == 'u' || key == 'U') {
		data->keyboard->u = 1;
	} else if (key == 'i' || key == 'I') {
		data->keyboard->i = 1;
	} else if (key == 'o' || key == 'O') {
		data->keyboard->o = 1;
	} else if (key == 'p' || key == 'P') {
		data->keyboard->p = 1;
	} else if (key == 'j' || key == 'J') {
		data->keyboard->j = 1;
	} else if (key == 'k' || key == 'K') {
		data->keyboard->k = 1;
	} else if (key == 'l' || key == 'L') {
		data->keyboard->l = 1;
	} else if (key == 'm' || key == 'M') {
		data->keyboard->m = 1;
	} else if (key == 27) {
		data->keyboard->esc = 1;
	} else if (key == ' ') {
		data->keyboard->space = 1;
		//toggle auto camera
		data->autocamera = !data->autocamera;
	}

	else if (key == '\r' && data->keyboard->alt) { //enter, code = 13
		toggleFullScreen();
	}
}

void normalKeyReleased(unsigned char key, int x, int y) {
	modifiersStatus();

	//qweasdzxcrty
	if (key == 'q' || key == 'Q') {
		data->keyboard->q = 0;
	} else if (key == 'w' || key == 'W') {
		data->keyboard->w = 0;
	} else if (key == 'e' || key == 'E') {
		data->keyboard->e = 0;
	} else if (key == 'a' || key == 'A') {
		data->keyboard->a = 0;
	} else if (key == 's' || key == 'S') {
		data->keyboard->s = 0;
	} else if (key == 'd' || key == 'D') {
		data->keyboard->d = 0;
	} else if (key == 'z' || key == 'Z') {
		data->keyboard->z = 0;
	} else if (key == 'x' || key == 'X') {
		data->keyboard->x = 0;
	} else if (key == 'c' || key == 'C') {
		data->keyboard->c = 0;
	} else if (key == 'r' || key == 'R') {
		data->keyboard->r = 0;
	} else if (key == 't' || key == 'T') {
		data->keyboard->t = 0;
	} else if (key == 'y' || key == 'Y') {
		data->keyboard->y = 0;
	}
	//fghvbnuiopjklm
	else if (key == 'f' || key == 'F') {
		data->keyboard->f = 0;
	} else if (key == 'g' || key == 'G') {
		data->keyboard->g = 0;
	} else if (key == 'h' || key == 'H') {
		data->keyboard->h = 0;
	} else if (key == 'v' || key == 'V') {
		data->keyboard->v = 0;
	} else if (key == 'b' || key == 'B') {
		data->keyboard->b = 0;
	} else if (key == 'n' || key == 'N') {
		data->keyboard->n = 0;
	} else if (key == 'u' || key == 'U') {
		data->keyboard->u = 0;
	} else if (key == 'i' || key == 'I') {
		data->keyboard->i = 0;
	} else if (key == 'o' || key == 'O') {
		data->keyboard->o = 0;
	} else if (key == 'p' || key == 'P') {
		data->keyboard->p = 0;
	} else if (key == 'j' || key == 'J') {
		data->keyboard->j = 0;
	} else if (key == 'k' || key == 'K') {
		data->keyboard->k = 0;
	} else if (key == 'l' || key == 'L') {
		data->keyboard->l = 0;
	} else if (key == 'm' || key == 'M') {
		data->keyboard->m = 0;
	} else if (key == 27) {
		data->keyboard->esc = 0;
	} else if (key == ' ') {
		data->keyboard->space = 0;
	}
}

void setSectionViewportPerspective(GLint _center_x, GLint _center_y, GLsizei _width, GLsizei _heigh) {
	glViewport(_center_x, _center_y, _width, _heigh);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOV, (GLdouble) _width / (GLdouble) _heigh, ZBUFFER_MIN, ZBUFFER_MAX);
	//save projection matrix
	glGetFloatv(GL_PROJECTION_MATRIX, data->cameraProjectionMatrix);
}

void displayFullViewport() {
	setSectionViewportPerspective(0, 0, data->window_width, data->window_height);
}

void setupCamera() {
	glMatrixMode(GL_MODELVIEW);
	// CLEAR MODEL VIEW MATRIX
	glLoadIdentity();
	// move auto camera
	data->campath->interpolateMotion((float) data->internaltime);
	// SETUP CAMERA according to its type
	if (data->autocamera && data->campath->numberPoints > 0)
		data->campath->updateCamera();
	else
		data->camera->lookAt();
	//save view matrix
	glGetFloatv(GL_MODELVIEW_MATRIX, data->cameraViewMatrix);
}

void display(void) {
	glClear(GL_ACCUM_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	//setup view port
	if (data->window_resized) {
		displayFullViewport();
	}
	//setup camera
	setupCamera();
	// DRAW SCENE
	displayScene();
	//roll the machine
	glFlush();
	glutSwapBuffers();
	if (data->window_resized) {
		data->window_resized = false;
	}
}

void reshape(int w, int h) {
	data->window_height = (GLsizei) h;
	data->window_width = (GLsizei) w;
	//request for resize
	data->window_resized = true;
}

// getting precision time in windows, returns wall time in microseconds
unsigned long getLongCPUTimeuS() {
	LARGE_INTEGER pf, pc;
	LONGLONG ticks, hz, ratio;
	QueryPerformanceFrequency(&pf);
	QueryPerformanceCounter(&pc);
	ticks = ((LONGLONG(pc.HighPart) << 32) + pc.LowPart);
	hz = ((LONGLONG(pf.HighPart) << 32) + pf.LowPart);
	ratio = (ticks / (hz / 1000000));
	long ret = (long) (ratio % LONG_MAX); //set upper limit on time because of conversion
	return ret;
}

//update internal scene speed
void updateSceneSpeed() {
	//get total execution time
	unsigned long time = getLongCPUTimeuS();
	//frame time (ms)
	unsigned long timedelta = (time - data->timebase);
	//test if first time (else already rendered)
	if (data->timebase > 0) {
		data->ms_per_frame = (long) (timedelta / 1000.0);
		//calculate motion per frame speed
		data->internaltimedelta = MOTION_PER_MS * timedelta / 1000.0;
		//move path
		data->internaltime += data->internaltimedelta;
	}
	//reset time & frame
	data->timebase = time;
}

void checkForErrors() {
	GLenum glerror = glGetError();
	if (glerror != 0) {
		printf("GL Error %d, %s\n", glerror, gluErrorString(glerror));
	}
}

void processKeys() {
	// KEYBOARD MODIFIERS
	float modf_speed = 1.0;
	if (data->keyboard->shift) {
		modf_speed *= MODF_SHIFT;
	}
	if (data->keyboard->control) {
		modf_speed *= MODF_CTRL;
	}
	if (data->keyboard->alt) {
		modf_speed *= MODF_ALT;
	}
	// STRAFING
	if (data->keyboard->w) {
		data->camera->moveForwardBackward(MOVE_DELTA * modf_speed);
	}
	if (data->keyboard->s) {
		data->camera->moveForwardBackward(-MOVE_DELTA * modf_speed);
	}
	if (data->keyboard->a) {
		data->camera->moveLeftRight(MOVE_DELTA * modf_speed);
	}
	if (data->keyboard->d) {
		data->camera->moveLeftRight(-MOVE_DELTA * modf_speed);
	}
	if (data->keyboard->pagedown) {
		data->camera->moveUpDown(-MOVE_DELTA * modf_speed);
	}
	if (data->keyboard->pageup) {
		data->camera->moveUpDown(MOVE_DELTA * modf_speed);
	}
	// LOOKING
	if (data->keyboard->up) {
		data->camera->rotateViewVirtualX(-ANGLE_DELTA * modf_speed);
	}
	if (data->keyboard->down) {
		data->camera->rotateViewVirtualX(ANGLE_DELTA * modf_speed);
	}
	if (data->keyboard->left) {
		data->camera->rotateViewVirtualY(ANGLE_DELTA * modf_speed);
	}
	if (data->keyboard->right) {
		data->camera->rotateViewVirtualY(-ANGLE_DELTA * modf_speed);
	}
	if (data->keyboard->q) {
		data->camera->rotateViewVirtualZ(ANGLE_DELTA * modf_speed);
	}
	if (data->keyboard->e) {
		data->camera->rotateViewVirtualZ(-ANGLE_DELTA * modf_speed);
	}
	// MISC
	if (data->keyboard->esc) {
		//get out of here!
		//		system("PAUSE");
		exit(1);
	}
}

void engineLoop() {
	//update engine speed according to rendering speed
	updateSceneSpeed();
	//make sure glut calls displaying function
	glutPostRedisplay();
	//if we have done something opengl doesn't like, show it to us
	checkForErrors();
	//check for keyboard events
	processKeys();
	//sleep the specified amount of milliseconds per frame
//	Sleep(mspf);
	//re-schedule the timer
	glutIdleFunc(engineLoop);
}

void showOpenGLSpecs() {
	printf("GL_VENDOR: %s\n", glGetString(GL_VENDOR));
	printf("GL_RENDERER: %s\n", glGetString(GL_RENDERER));
	printf("GL_VERSION: %s\n", glGetString(GL_VERSION));

	int d;

	glGetIntegerv(GL_DEPTH_BITS, &d);
	printf("GL_DEPTH_BITS: %d\n", d);

	glGetIntegerv(GL_MAX_LIGHTS, &d);
	printf("GL_MAX_LIGHTS: %d\n", d);

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &d);
	printf("GL_MAX_TEXTURE_SIZE: %d\n", d);
}

void initShadows() {
	//Check for necessary extensions
	if (!GLEE_ARB_depth_texture || !GLEE_ARB_shadow) {
		printf("I require ARB_depth_texture and ARB_shadow extensions\n");
		exit(-1);
	}
	//create and setup shadow texture
	data->shadowMapSize = SHADOW_MAP_SIZE;
	if (data->shadowMapSize > data->gl_max_texture_size)
		data->shadowMapSize = data->gl_max_texture_size;
	printf("using shadow map of %dx%d\n", data->shadowMapSize, data->shadowMapSize);
	createShadowTexture();
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GEQUAL, 0.5);
}

void initScene() {
	data->model_scene = glmReadOBJ("INDOOR_STORAGE.obj");
	glmScale(data->model_scene, 5.0);

	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	initLights();

	//give zero ambient light
	GLfloat lmodel_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	createDisplayLists();
	puts("Scene initialized");
}

void initCamera() {
	data->campath = new CameraPathFollower("camera_path.txt");
	data->autocamera = true;
	data->internaltime = 0.0;

	data->camera = new Camera(false);
	data->camera->position->z = -10.0;
	data->camera->direction->z = 1.0;
	data->camera->up->y = 1.0;
	data->camera->right->x = 1.0;
}

void EngineInit() {
	showOpenGLSpecs();

	//force first time run to setup view port
	data->window_resized = true;
	glEnable(GL_DEPTH_TEST);
	glDrawBuffer(GL_BACK);
	//clear drawing buffer
	glClearColor(0.0, 0.0, 0.0, 0.0);
	//time counter
	data->timebase = 0;
	data->internaltimedelta = MOTION_PER_MS;

	matrix_bias = new GLfloat[16];
	matrix_bias[0] = 0.5;
	matrix_bias[1] = 0.0;
	matrix_bias[2] = 0.0;
	matrix_bias[3] = 0.0;
	matrix_bias[4] = 0.0;
	matrix_bias[5] = 0.5;
	matrix_bias[6] = 0.0;
	matrix_bias[7] = 0.0;
	matrix_bias[8] = 0.0;
	matrix_bias[9] = 0.0;
	matrix_bias[10] = 0.5;
	matrix_bias[11] = 0.0;
	matrix_bias[12] = 0.5;
	matrix_bias[13] = 0.5;
	matrix_bias[14] = 0.5;
	matrix_bias[15] = 1.0;
	matrix_product = new GLfloat[16];
	data->cameraProjectionMatrix = new GLfloat[16];
	data->cameraViewMatrix = new GLfloat[16];

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &(data->gl_max_texture_size));

	data->keyboard = new Keyboard();
	initCamera();
	initScene();
	initShadows();
}

int main(int argc, char** argv) {
	// disable stdout buffering
	setvbuf(stdout, NULL, _IONBF, 0);

	data = new EngineData();

	data->window_width = WINDOW_WIDTH;
	data->window_height = WINDOW_HEIGHT;

	fullscreen = false;

	puts("(C) Joao Carlos Ferreira Goncalves");
	puts("jcfgonc@gmail.com");

	glutInit(&argc, argv);
	glutInitDisplayString(" acca rgba>=8 double depth>=24 ");
	glutInitWindowSize(data->window_width, data->window_height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Shadow mapping demo");
//	glutFullScreen();

	EngineInit();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(normalKeyPressed);
	glutKeyboardUpFunc(normalKeyReleased);
	glutSpecialFunc(specialKeyPressed);
	glutSpecialUpFunc(specialKeyReleased);
	glutIdleFunc(engineLoop);
	glutMainLoop();

	return 0;
}
