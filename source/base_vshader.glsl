#version 330

in vec3 vPosition;
in vec4 vColor;

out vec4 f_Color;

uniform mat4 u_MVPMatrix;
uniform vec4 u_Color;

void main()
{
	gl_Position = u_MVPMatrix * vec4(vPosition, 1.0);
	gl_Position *= vec4(1.0, 1.0, -1.0, 1.0);
	f_Color = vColor;
}