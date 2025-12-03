#version 330

in vec3 v_Position;
in vec4 v_Color;
in vec3 v_VertexNormal;
in vec3 v_FaceNormal;

out vec3 f_Position;
out vec4 f_Color;
out vec3 f_Normal;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projMatrix;
uniform int u_shadingMode;

void main()
{
	gl_Position = u_projMatrix * u_viewMatrix * u_modelMatrix * vec4(v_Position, 1.0);
	gl_Position *= vec4(1.0, 1.0, -1.0, 1.0); // for depth test

	f_Position = vec3(u_modelMatrix * vec4(v_Position, 1.0));
	f_Color = v_Color;
	if (u_shadingMode == 1)
		f_Normal = normalize( (u_modelMatrix * vec4(v_VertexNormal, 0.0)).xyz );
	else if (u_shadingMode == 2)
		f_Normal = normalize( (u_modelMatrix *  vec4(v_FaceNormal, 0.0)).xyz);
}