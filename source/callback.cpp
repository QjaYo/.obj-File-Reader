#include "callback.h"

#include <vgl.h>
#include <vec.h>

void idle()
{
	static int cur_time = 0;
	static int prev_time = 0;
	static float theta = 0.0f;

	cur_time = glutGet(GLUT_ELAPSED_TIME); // ms
	float dt = (cur_time - prev_time) / 1000.0f;
	prev_time = cur_time;
	g_time += dt;
	if (b_play)
	{
		if (rotation_axis == 0)
			obj.rotation.x = fmod((obj.rotation.x + rotationSpeed * dt), 360.0f);
		else if (rotation_axis == 1)
			obj.rotation.y = fmod((obj.rotation.y + rotationSpeed * dt), 360.0f);
		else if (rotation_axis == 2)
			obj.rotation.z = fmod((obj.rotation.z + rotationSpeed * dt), 360.0f);
		else
			std::cout << "idle()::rot_axis corrupted" << std::endl;
	}
	if (b_rotateLightsource)
	{
		theta = fmod(theta + 4.0f * dt, 360.0f);
		float radius = 1.5;
		lightPosition = vec3(radius * sin(theta), 0.0, radius * cos(theta));
	}
	glutPostRedisplay();
}

void keyboard(unsigned char input, int mouse_x, int mouse_y)
{
	if (input == 'q') // exit program
	{
		exit(1);
	}
	else if (input == ' ') // start/stop rotate object (change rotation axis by mouse)
	{
		b_play = !b_play;
	}
	else if (input == '1') // vertex normal shading
	{
		shadingMode = 1;
		glutPostRedisplay();
	}
	else if (input == '2') // face normal(surface normal) shading
	{
		shadingMode = 2;
		glutPostRedisplay();
	}
	else if (input == '3') // increase specular light intensity
	{
		ks += vec4(0.05, 0.05, 0.05, 0.0);
		float max_ks = 1.0f;
		if (ks.x > max_ks)
			ks.x = ks.y = ks.z = max_ks;
		std::cout << "ks: " << ks.x << std::endl;
		glutPostRedisplay();
	}
	else if (input == '4') // decrease specular light intensity
	{
		ks -= vec4(0.05, 0.05, 0.05, 0.0);
		float min_ks = 0.0f;
		if (ks.x < min_ks)
			ks.x = ks.y = ks.z = min_ks;
		std::cout << "ks: " << ks.x << std::endl;
		glutPostRedisplay();
	}
	else if (input == '5') // increase shininess (light reflects specularly)
	{
		shininess -= 2.0f;
		shininess = std::max(shininess, 1.0f);
		std::cout << "shininess: " << shininess << std::endl;
		glutPostRedisplay();
	}
	else if (input == '6') // decrease shininess (light reflects diffusely)
	{
		shininess += 2.0f;
		shininess = std::min(shininess, 400.0f);
		std::cout << "shininess: " << shininess << std::endl;
		glutPostRedisplay();
	}
	else if (input == '7') // increase ambient light intensity
	{
		ambientLight_intensity += 0.05f;
		ambientLight_intensity = std::min(ambientLight_intensity, 1.0f);
		std::cout << "ambient light intensity: " << ambientLight_intensity << std::endl;
		glutPostRedisplay();
	}
	else if (input == '8') // decrease ambient light intensity
	{
		ambientLight_intensity -= 0.05f;
		ambientLight_intensity = std::max(ambientLight_intensity, 0.0f);
		std::cout << "ambient light intensity: " << ambientLight_intensity << std::endl;
		glutPostRedisplay();
	}
	else if (input == '9') // increase diffuse rate
	{
		diffuse_rate += 0.05f;
		diffuse_rate = std::min(diffuse_rate, 1.0f);
		std::cout << "diffuse rate: " << diffuse_rate << std::endl;
		glutPostRedisplay();
	}
	else if (input == '0') // decrease diffuse rate
	{
		diffuse_rate -= 0.05f;
		diffuse_rate = std::max(diffuse_rate, 0.0f);
		std::cout << "diffuse rate: " << diffuse_rate << std::endl;
		glutPostRedisplay();
	}
	else if (input == 'r') // rotate lightsource (rotation axis: y)
	{
		b_rotateLightsource = !b_rotateLightsource;
		glutPostRedisplay();
	}
	else if (input == 'i') // zoom in
	{
		fovy = std::max(10.0f, fovy - 1.0f);
		glutPostRedisplay();
	}
	else if (input == 'o') // zoom out
	{
		fovy = std::min(80.0f, fovy + 1.0f);
		glutPostRedisplay();
	}
	else if (input == 'a') // show axis
	{
		b_showAxis = !b_showAxis;
	}
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) // x rotation
		rotation_axis = 0;
	else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) // y rotation
		rotation_axis = 1;
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) // z rotation
		rotation_axis = 2;
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	g_aspect = w / float(h);
	glutPostRedisplay();
}