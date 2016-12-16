// A simple, if a little square, water caustic effect.
// David Hoskins.
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.

// Inspired by akohdr's "Fluid Fields"
// https://www.shadertoy.com/view/XsVSDm

uniform float u_time;

#define F length(.5-fract(gl_FragColor.xyw*=mat3(-2,-1,2, 3,-2,1, 1,2,2)*

void main(){
	gl_FragColor.xy = vec2(1.0);
    gl_FragColor.xy = gl_FragCoord.xy*(sin(gl_FragColor*.5).w+2.)/2e2;
    gl_FragColor = pow(min(min(F.5)),F.4))),F.3))), 6.)*25.+vec4(0,.35,.5,1);
}
