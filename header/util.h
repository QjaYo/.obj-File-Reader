#pragma once

#include <vgl.h>
#include <vec.h>
#include <mat.h>

#include <vector>

#include "object.h"
#include "global.h"

class Util
{
public:
	static void printDivider();
	static bool printTxtFile(const std::string& fileName);
	static bool loadObj(const std::string& fileName, std::vector<VertexData>& vertices);
	static mat4 getProjMatrix(float fovy, float aspect, float zNear, float zFar);
	static mat4 getViewMatrix(vec3 eye, vec3 at, vec3 up);
	static mat4 getModelMatrix(Object& obj);
};