#pragma once

#include <vgl.h>
#include <vec.h>

struct CubeVertex
{
	vec4 position;
	vec4 color;
};

class Cube
{
public:
	Cube(void);
	~Cube(void);

	static const int NumVertices = 36;

	CubeVertex Vertices[NumVertices];
	GLuint init();
	void SetPositionAndColorAttribute(GLuint program);

	void ColorCube(vec4* vin, vec4* cin);
	void Quad(int a, int b, int c, int d, vec4* vin, vec4* cin);

	int NumCurVertices;

	GLuint vao;
	GLuint buffer;
	bool bInitialized;

	void Draw(GLuint program);
};