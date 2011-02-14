varying vec3 normal;
varying vec3 p;
varying vec3 view;

void main(void)
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
  	normal = gl_NormalMatrix * gl_Normal;
    vec4 ptmp = gl_ModelViewMatrix * gl_Vertex;
    p = ptmp.xyz/ptmp.w;

	// vecteur observateur
	// view = point_courant - position_camera
	view = vec3(gl_Vertex.xyz - gl_Position.xyz);
	
	gl_Position = ftransform();
}

