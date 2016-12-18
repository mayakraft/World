uniform vec2 u_resolution;
uniform float u_time;

float circle(float radius, float thickness){
    float scale = 1.0/min(u_resolution.x, u_resolution.y);   // depending on aspect ratio
	vec2 uv = (gl_FragCoord.xy - u_resolution.xy * 0.5) * scale;  // center on the screen
    float l = length(uv);
	return 1.0 - smoothstep(0., scale, max(l - radius, radius-thickness - l));
}

float X(float weight){
	float shift1 = u_resolution.x*0.5 - u_resolution.y*0.5;
	float shift2 = u_resolution.y + (u_resolution.x - u_resolution.y)*0.5;
	float first = 0.0;
	first += float(gl_FragCoord.x > gl_FragCoord.y + (-weight + shift1) && 
	               gl_FragCoord.x < gl_FragCoord.y + (weight + shift1) );

	first += float(gl_FragCoord.x > -gl_FragCoord.y-weight+shift2 && 
	               gl_FragCoord.x < -gl_FragCoord.y+weight + shift2);
	return first;
}

float aspectX(float weight){
	float asp = (u_resolution.x/u_resolution.y);
	float second = 0.0;
	second += float(gl_FragCoord.x > gl_FragCoord.y*asp - weight &&
	               gl_FragCoord.x < gl_FragCoord.y*asp + weight);

	second += float(gl_FragCoord.x > u_resolution.x -gl_FragCoord.y*asp - weight &&
	               gl_FragCoord.x < u_resolution.x -gl_FragCoord.y*asp + weight);
	return second;
}

float frame(float thickness){
	return float(gl_FragCoord.x < thickness * u_resolution.x || 
	             gl_FragCoord.x > u_resolution.x-thickness * u_resolution.x ||
	             gl_FragCoord.y < thickness * u_resolution.y || 
	             gl_FragCoord.y > u_resolution.y-thickness * u_resolution.y );
}

float frameHole(float thickness){
	return float(gl_FragCoord.x > thickness * u_resolution.x && 
	             gl_FragCoord.x < u_resolution.x-thickness * u_resolution.x &&
	             gl_FragCoord.y > thickness * u_resolution.y && 
	             gl_FragCoord.y < u_resolution.y-thickness * u_resolution.y);
}

void main(){
	float xLine = X(1.0);
	float xAspectLine = aspectX(1.0);
	float circle = circle(0.5, 0.001);
	float f = frame(0.125-0.0015);
	float s = frameHole(0.125+0.0015);
	float frameLine = float(!bool(f) && !bool(s));
	float padding = 0.0;//frame(0.125) * 0.15;


	float brightness = 0.66;

	vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
	// color += vec4(vec3(0.0, 0.0, 0.0), 0.0);
	float redO = clamp( brightness*( circle ), 0.0, 1.0);
	float greenO = brightness*( xLine + xAspectLine );//clamp( brightness*( xLine + xAspectLine ), 0.0, 1.0);
	// color += vec4(1.0, 0.0, 0.0, redO);
	// color += vec4(0.0, 1.0, 0.0, greenO);
	// color += vec4(0.0, 0.0, 1.0, blueO);

	color += vec4(redO, 0.0, 0.0, redO);
	color += vec4(0.0, greenO, 0.0, greenO);

	gl_FragColor = color;
}