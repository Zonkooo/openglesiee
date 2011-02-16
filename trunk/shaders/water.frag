varying vec3 N;
varying vec3 V;
varying vec3 S;

uniform samplerCube sky;
uniform int time;

float reflect = 0.6;

void main(void)
{
	float x = gl_TexCoord[0].x;
	float y = gl_TexCoord[0].y;
	S.x += 0.1*cos(0.001*time + (x + y) * 100.);
	S.z += gl_TexCoord[0].z;
	S = normalize(S);
	vec3 water_color = reflect*textureCube(sky, S).rgb + (1.0 - reflect)*vec3(0.0, 0.0, 1.0);
	
	gl_FragColor.rgb = water_color;
}

