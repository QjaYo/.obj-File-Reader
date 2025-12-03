#include <vgl.h>
#include <vec.h>
#include <mat.h>

#include "global.h"
#include "object.h"
#include "cube.h"

GLuint base_program;
GLuint phong_program;

Object obj;
Cube cube;

mat4 viewMatrix = mat4(1.0);			//world -> camera
mat4 projMatrix = mat4(1.0);			//camera -> screen

vec3 cameraPosition = vec3(0.0f, 0.0f, 3.0f);
vec3 at = vec3(0.0f, 0.0f, 0.0f);
vec3 up = vec3(0.0f, 1.0f, 0.0f);
vec3 lightPosition = vec3(0.0, 5.0, 5.0);
vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);
float fovy = 45.0f;
bool b_play = false;
bool b_rotateLightsource = true;
bool b_showAxis = true;
int shadingMode = 1; // 1: vertex normal, 2: face normal
vec4 ks = vec4(0.5, 0.5, 0.5, 1.0);
float shininess = 30.0;
float ambientLight_intensity = 0.3;
float diffuse_rate = 0.5;
int rotation_axis = 2; // 0: x_axis, 1: y_axis, 2: z_axis
float rotationSpeed = 60.0f;
float g_time = 0.0f;
float g_aspect = 1.0f;