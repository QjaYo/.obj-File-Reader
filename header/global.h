#pragma once

#include <vgl.h>
#include <vec.h>
#include <mat.h>

#include "cube.h"
#include "object.h"

// constants
const float PI = 3.141592;

extern GLuint base_program;
extern GLuint phong_program;

extern Object obj;
extern Cube cube;

extern mat4 viewMatrix;
extern mat4 projMatrix;

extern vec3 cameraPosition;
extern vec3 at;
extern vec3 up;
extern vec3 lightPosition;
extern vec4 lightColor;
extern float fovy;
extern bool b_play;
extern bool b_rotateLightsource;
extern bool b_showAxis;
extern int shadingMode;
extern vec4 ks;
extern float shininess;
extern float ambientLight_intensity;
extern float diffuse_rate;
extern int rotation_axis;
extern float rotationSpeed;
extern float g_time;
extern float g_aspect;