#include "object.h"

Object::Object()
{
	this->bInitialized = false;
	this->position = vec3(0.0f, 0.0f, 0.0f);
	this->rotation = vec3(0.0f, 0.0f, 0.0f);
	this->scale = vec3(1.0f, 1.0f, 1.0f);
}

Object::~Object()
{
	;
}

void Object::init(const std::vector<VertexData>& vertices, GLuint program)
{
	if (this->bInitialized == true) return;

	this->vertices = vertices;
	this->program = program;

	// VAO VBO setting
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(VertexData), this->vertices.data(), GL_STATIC_DRAW);

	// Vertex Attribute Pointer Setting
	glUseProgram(program);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	{
		GLuint v_Position = glGetAttribLocation(program, "v_Position");
		glEnableVertexAttribArray(v_Position);
		glVertexAttribPointer(v_Position, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, position));

		GLuint v_Color = glGetAttribLocation(program, "v_Color");
		glEnableVertexAttribArray(v_Color);
		glVertexAttribPointer(v_Color, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, color));

		GLuint v_VertexNormal = glGetAttribLocation(program, "v_VertexNormal");
		glEnableVertexAttribArray(v_VertexNormal);
		glVertexAttribPointer(v_VertexNormal, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, vertexNormal));

		GLuint v_FaceNormal = glGetAttribLocation(program, "v_FaceNormal");
		glEnableVertexAttribArray(v_FaceNormal);
		glVertexAttribPointer(v_FaceNormal, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, faceNormal));
	}

	glBindVertexArray(0);

	bInitialized = true;
}

void Object::draw()
{
	if (!bInitialized) return;

	glUseProgram(this->program);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
	glBindVertexArray(0);
}