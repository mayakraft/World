#define HARMONICS 64.0

// http://en.wikipedia.org/wiki/Square_wave
// http://en.wikipedia.org/wiki/Sawtooth_wave

// Left out the PI, sorry, feel free to put it back in! :)

uniform float u_time;
uniform vec2 u_resolution;
	
float Wave( vec2 uv, float time){
	
	float t = (time/16.0-uv.x+.12) * 25.0;
	time = mod(time*8.0, HARMONICS*2.0);
	if (time > HARMONICS) time = HARMONICS*2.0 - time;
	time += 1.0;

	float x = 0.0;
	for (float i = 1.0; i <= HARMONICS; i+=1.0)
	{
		if (i > time) continue;
		// Square wave...
		float h = i * 2.0 - 1.0;
		float square = sin(h*t) / h;

		
		float saw = .5*(sin(i*t) / i);

		// Mix across the centre line...
		float n = smoothstep(.4, .601, uv.x);
		x += mix(square, saw, n);
	}
	float y = uv.y*2.0-1.0;	
	return smoothstep(y+.02, y-.04, x);
}

void main()
{
	vec2 uv = gl_FragCoord.xy / u_resolution.xy;
	float a = 0.0;
	// Anti-alias the x direction...
	for (int x = 0; x < 4; x++)
	{
		a += Wave(uv, u_time);
		uv.x += (.25 / u_resolution.x);
	}
	a *= .25;
	gl_FragColor = vec4( vec3(a), 1.0);
}
