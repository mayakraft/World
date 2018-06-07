// Various step functions
// By David Hoskins.

// The function interpolates smoothly between two input values based on a third one
// that should be between the first two. The returned value is clamped between 0 and 1.

//---------------------------------------------------------------------------
float original_smoothstep(float edge0, float edge1, float x)
{
	float  t = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
    return t * t * (3.0 - 2.0 * t);
}

//---------------------------------------------------------------------------
float linearstep(float edge0, float edge1, float x)
{
    return  clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
}

//---------------------------------------------------------------------------
float smoothbumpstep(float edge0, float edge1, float x)
{
    x = 1.0-abs(clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0)-.5)* 2.0;
    return x * x * (3.0 - x - x);
}

//---------------------------------------------------------------------------
float bumpstep(float edge0, float edge1, float x)
{
    return 1.0-abs(clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0)-.5)*2.0;
}

//---------------------------------------------------------------------------
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
	vec2 uv = fragCoord.xy;
    int ver = int(mod(iGlobalTime*1.31, 5.0));
 
    float x = 0.0;
    if (ver == 0)
    {
    	x = linearstep(0.0,  iResolution.x, uv.x);
	}else
	if (ver == 1)
    {
        //  or use 'original_smoothstep', which takes the same time to process...
		x = smoothstep(0.0,  iResolution.x, uv.x);
	}else
	if (ver == 2)
	{
		x = bumpstep(0.0,  iResolution.x, uv.x);
	}else
	if (ver == 3)
    {
		x = smoothbumpstep(0.0,  iResolution.x, uv.x);
	}else
	if (ver == 4)
    {
        // Fade in and out example...
        float dist = iResolution.x / 10.0;
        x = smoothstep(0.0, dist, uv.x) * smoothstep( iResolution.x, iResolution.x-dist, uv.x);
	}
   
    float f = linearstep(x-0.005,x+0.005, uv.y/iResolution.y);
    fragColor = vec4(f);
}