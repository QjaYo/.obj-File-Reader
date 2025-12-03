#version 330

in vec3 f_Position;
in vec4 f_Color;
in vec3 f_Normal;

out vec4 final_Color;

uniform vec3 u_lightPosition;
uniform vec4 u_lightColor;
uniform vec3 u_cameraPosition;
uniform vec4 u_ks;
uniform float u_shininess;
uniform float u_ambientLight_intensity;
uniform float u_diffuse_rate;

void main()
{
	// Set shared variables
	//vec3 L = normalize(u_lightPosition); // if (ÅÂ¾ç±¤)
	vec3 L = normalize(u_lightPosition - f_Position); // if (Á¡±¤¿ø)
	vec3 N = normalize(f_Normal);
	vec4 I = u_lightColor;
	vec4 Il = I;
	vec4 Ia = I;

	// Ambient
	vec4 ka = f_Color * u_ambientLight_intensity;
	vec4 ambient = ka * Ia;
	
	// Diffuse
	vec4 kd = f_Color * u_diffuse_rate;
	float NL = max( dot(L, N), 0.0 );
	vec4 diffuse = kd * Il * NL;

	// Specular
	vec3 R = normalize(2 * dot(L, N) * N - L);
	vec3 V = normalize(u_cameraPosition - f_Position);
	vec4 specular = u_ks * Il * pow( max( dot(V, R), 0.0 ), u_shininess );

	final_Color = ambient + diffuse + specular;
}