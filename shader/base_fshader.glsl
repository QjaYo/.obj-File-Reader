#version 330

in vec4 f_Color;

out vec4 final_Color;

uniform vec4 u_Color;

void main()
{
	//final_Color = f_Color;
	//final_Color = vec4(0.1, 0.1, 0.1, 1.0);
	final_Color = u_Color;
}