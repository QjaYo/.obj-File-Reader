#pragma once

#include <vgl.h>
#include <vec.h>

#include <vector>

#include "struct.h"

class Object
{
public:
	vec3 position = vec3(0.0f, 0.0f, 0.0f);
	vec3 rotation = vec3(0.0f, 0.0f, 0.0f);
	vec3 scale = vec3(1.0f, 1.0f, 1.0f);

	std::vector<VertexData> vertices;

	GLuint VAO = 0;
	GLuint VBO = 0;
	GLuint program = 0;

	bool bInitialized = false;

	Object();
	~Object();
	void init(const std::vector<VertexData>& vertices, GLuint program);
	void draw();
};