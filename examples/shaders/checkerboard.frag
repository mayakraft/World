// by Nikos Papadopoulos, 4rknova / 2013
// WTFPL

#define S 5. // Scale

uniform vec2 u_resolution;// = vec2(800,600);

void main(){
	vec2 uv = vec2(floor(S * gl_FragCoord.xy * vec2(u_resolution.x / u_resolution.y, 1) / u_resolution.xy));
	gl_FragColor = vec4(vec3(mod(uv.x + uv.y, 2.)), 1);
}