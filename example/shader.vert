#version 120

varying float xpos;
varying float ypos;
varying float zpos;

uniform float time; //elasped time

void main(void)
{
  xpos = clamp(gl_Vertex.x,0.0,1.0);
  ypos = clamp(gl_Vertex.y,0.0,1.0);
  zpos = clamp(gl_Vertex.z,0.0,1.0);
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
//  gl_FrontColor = vec4(0.0, clamp(gl_Position[0], 0.0, 1.0), 1.0, 1.0);
  vec4 v = gl_ProjectionMatrix * gl_Vertex;
  // gl_FrontColor = vec4(1.0-v[0], 1.0-v[1], 1.0-v[2], 1.0);
  gl_FrontColor = vec4(v[0] * abs(sin(time)), v[1] * abs(cos(time)), v[2], 1.0);
}

//void main(void)
//{
//	vec4 a = gl_Vertex;
//	a.x = a.x * 1.0;
//	a.y = a.y * 1.0;
//	a.z = a.z * 1.0;
//	gl_Position = gl_ModelViewProjectionMatrix * a;
//}