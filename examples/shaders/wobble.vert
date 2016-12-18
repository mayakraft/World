uniform float u_time;
uniform float u_resolution;

void main(){
	float mag = 0.4;
	vec4 nudge = vec4(mag * sin(u_time) + 0.5, 
	                  mag * cos(u_time*0.4444) + 0.5, 
	                  mag * -sin(u_time*0.7777), 
	                  1.0);
	gl_Position = gl_ModelViewProjectionMatrix * (gl_Vertex+nudge);
}