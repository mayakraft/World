#version 120

//varying float xpos;
//varying float ypos;
//varying float zpos;

void main(void)
{
//	float abcd;
//	abcd = 1.0;
//	abcd = gl_Color[0];//gl_FragCoord[0];
	gl_FragColor = gl_Color;   //vec4(1.0, 1.0, 1.0, 1.0);//gl_Color;
//	gl_FragColor = vec4(xpos, abcd, zpos, 1.0);
}

//void main(void)
//{
//	gl_FragColor = vec4(gl_PointCoord.x, gl_PointCoord.y, gl_PointCoord.z, 1.0);
//}