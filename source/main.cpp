#include <vgl.h>
#include <InitShader.h>
#include <vec.h>
#include <mat.h>
#include <MyCube.h>
#include <MyPyramid.h>

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "object.h"
#include "util.h"
#include "global.h"
#include "callback.h"
#include "struct.h"


void init(const std::vector<VertexData>& vertices)
{
	base_program = InitShader("base_vshader.glsl", "base_fshader.glsl");
	phong_program = InitShader("phong_vshader.glsl", "phong_fshader.glsl");

	obj.init(vertices, phong_program);
	cube.init();
}

void drawAxis(GLuint program)
{
	glUseProgram(program);

	static GLuint u_MVPMatrix = glGetUniformLocation(program, "u_MVPMatrix");
	static GLuint u_Color = glGetUniformLocation(program, "u_Color");

	mat4 R = RotateX(obj.rotation.x) * RotateY(obj.rotation.y) * RotateZ(obj.rotation.z);
	mat4 T = Translate(obj.position);
	static mat4 modelMatrix = mat4(1.0);
	static float axis_thickness = 0.02f;
	static float axis_length = 1.0f;

	//x_axis
	vec4 x_axis_color = vec4(1.0, 0.0, 0.0, 0.0); // R
	modelMatrix = T * R * Translate(axis_length / 2, 0.0, 0.0) * Scale(axis_length, axis_thickness, axis_thickness);
	glUniformMatrix4fv(u_MVPMatrix, 1, true, projMatrix * viewMatrix * modelMatrix);
	glUniform4f(u_Color, x_axis_color.x, x_axis_color.y, x_axis_color.z, x_axis_color.w);
	cube.Draw(program);

	//y_axis
	vec4 y_axis_color = vec4(0.0, 1.0, 0.0, 0.0); // G
	modelMatrix = T * R * Translate(0.0, axis_length / 2, 0.0) * Scale(axis_thickness, axis_length, axis_thickness);
	glUniformMatrix4fv(u_MVPMatrix, 1, true, projMatrix * viewMatrix * modelMatrix);
	glUniform4f(u_Color, y_axis_color.x, y_axis_color.y, y_axis_color.z, y_axis_color.w); // G
	cube.Draw(program);

	//z_axis
	vec4 z_axis_color = vec4(0.0, 0.0, 1.0, 0.0); // B
	modelMatrix = T * R * Translate(0.0, 0.0, axis_length / 2) * Scale(axis_thickness, axis_thickness, axis_length);
	glUniformMatrix4fv(u_MVPMatrix, 1, true, projMatrix * viewMatrix * modelMatrix);
	glUniform4f(u_Color, 0.0, 0.0, 1.0, 1.0); // B
	cube.Draw(program);
}

void drawLightsource(GLuint program)
{
	glUseProgram(program);

	GLuint u_MVPMatrix = glGetUniformLocation(program, "u_MVPMatrix");
	GLuint u_Color = glGetUniformLocation(program, "u_Color");

	mat4 modelMatrix = Translate(lightPosition) * Scale(0.2, 0.2, 0.2);
	mat4 MVPMatrix = projMatrix * viewMatrix * modelMatrix;

	glUniformMatrix4fv(u_MVPMatrix, 1, true, MVPMatrix);
	glUniform4f(u_Color, 1.0, 1.0, 1.0, 1.0);
	cube.Draw(program);
}

void display()
{
	
	glClearColor(0.2 * ambientLight_intensity, 0.2 * ambientLight_intensity, 0.2 * ambientLight_intensity, 1.0f);
	//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	
	// Update Global Transform Matrix
	viewMatrix = Util::getViewMatrix(cameraPosition, at, up);			//world -> camera
	projMatrix = Util::getProjMatrix(fovy, g_aspect, 0.1, 10.0);		//camera -> screen

	// Draw Axis
	if (b_showAxis)
		drawAxis(base_program);

	// Draw lightsource
	drawLightsource(base_program);

	// Draw Object
	glUseProgram(phong_program);

	mat4 modelMatrix = Util::getModelMatrix(obj);
	static GLuint u_modelMatrix = glGetUniformLocation(phong_program, "u_modelMatrix");
	glUniformMatrix4fv(u_modelMatrix, 1, true, modelMatrix);

	static GLuint u_viewMatrix = glGetUniformLocation(phong_program, "u_viewMatrix");
	glUniformMatrix4fv(u_viewMatrix, 1, true, viewMatrix);

	static GLuint u_projMatrix = glGetUniformLocation(phong_program, "u_projMatrix");
	glUniformMatrix4fv(u_projMatrix, 1, true, projMatrix);
	
	static GLuint u_lightPosition = glGetUniformLocation(phong_program, "u_lightPosition");
	glUniform3fv(u_lightPosition, 1, &lightPosition.x);

	static GLuint u_lightColor = glGetUniformLocation(phong_program, "u_lightColor");
	glUniform4fv(u_lightColor, 1, &lightColor.x);

	static GLuint u_cameraPosition = glGetUniformLocation(phong_program, "u_cameraPosition");
	glUniform3fv(u_cameraPosition, 1, &cameraPosition.x);
	
	static GLuint u_shadingMode = glGetUniformLocation(phong_program, "u_shadingMode");
	glUniform1i(u_shadingMode, shadingMode);

	static GLuint u_ks = glGetUniformLocation(phong_program, "u_ks");
	glUniform4fv(u_ks, 1, &ks.x);

	static GLuint u_shininess = glGetUniformLocation(phong_program, "u_shininess");
	glUniform1f(u_shininess, shininess);

	static GLuint u_ambientLight_intensity = glGetUniformLocation(phong_program, "u_ambientLight_intensity");
	glUniform1f(u_ambientLight_intensity, ambientLight_intensity);

	static GLuint u_diffuse_rate = glGetUniformLocation(phong_program, "u_diffuse_rate");
	glUniform1f(u_diffuse_rate, diffuse_rate);

	obj.draw();

	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	std::vector<VertexData> vertices;
	while (true)
	{
		std::cout << "Input File Name : ";
		std::string fileName;
		std::cin >> fileName;
		bool success = Util::loadObj(fileName, vertices); // initialize vertices
		if (success) break;
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Assignment4: .obj File Reader");

	glewExperimental = true;
	glewInit();

	init(vertices);
	vertices.clear();

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);

	Util::printDivider();
	Util::printTxtFile("manual.txt");
	glutMainLoop();

	return 0;
}