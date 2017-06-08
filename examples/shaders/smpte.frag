uniform float u_time;
uniform vec2 u_resolution;

void main() {
	vec2 st = gl_FragCoord.xy/u_resolution.xy;
	vec3 color = vec3(0.0);

	bool sev1 = bool( step(st.x, 1.0/7.0) );
	bool sev2 = bool( step(st.x, 2.0/7.0) );
	bool sev3 = bool( step(st.x, 3.0/7.0) );
	bool sev4 = bool( step(st.x, 4.0/7.0) );
	bool sev5 = bool( step(st.x, 5.0/7.0) );
	bool sev6 = bool( step(st.x, 6.0/7.0) );

	bool row1 = !bool( step(st.y, 0.3333) );
	bool row2 = !bool( step(st.y, 0.25) );
	bool row3 = true;

	/////////////////////////////////////////
	// R : 0.5  1.0            1.0  1.0      
	// G : 0.5  1.0  1.0  1.0                
	// B : 0.5       1.0       1.0       1.0
	/////////////////////////////////////////
	float top_red =   0.75*float(sev1) + float((sev6 && !sev4) || (sev2 && !sev1) );
	float top_green = 0.75*float(sev1) + float(sev4 && !sev1);
	float top_blue =  0.75*float(sev1) + float(!sev6 || (sev5 && !sev4) || (sev3 && !sev2) );

	/////////////////////////////////////////
	// R :           1.0                 0.5      
	// G :                     1.0       0.5                
	// B : 1.0       1.0       1.0       0.5
	/////////////////////////////////////////
	float mid_red =   0.75*float(!sev6) + 0.075*float( (sev6 && !sev5)||(sev4 && !sev3)||(sev2 && !sev1)) + float((sev3 && !sev2) );
	float mid_green = 0.75*float(!sev6) + 0.075*float( (sev6 && !sev5)||(sev4 && !sev3)||(sev2 && !sev1)) + float((sev5 && !sev4) );
	float mid_blue =  0.75*float(!sev6) + 0.075*float( (sev6 && !sev5)||(sev4 && !sev3)||(sev2 && !sev1)) + float((sev5 && !sev4) || (sev3 && !sev2) || (sev1));

	///////////////////////
	// R: 0.03  1.0  0.22
	// G: 0.24  1.0  0.00
	// B: 0.35  1.0  0.49
	/////////////////////// 
	bool fourth1 = bool( step(st.x, 1.0*(5.0/7.0)/4.0 ));
	bool fourth2 = bool( step(st.x, 2.0*(5.0/7.0)/4.0 ));
	bool fourth3 = bool( step(st.x, 3.0*(5.0/7.0)/4.0 ));
	bool fourth4 = bool( step(st.x, (5.0/7.0) ));

	bool littleThird1 = bool( step(st.x, 5.0/7.0 + 1.0/7.0/3.0) );
	bool littleThird2 = bool( step(st.x, 5.0/7.0 + 1.0/7.0/3.0*2.0) );
	bool littleThird3 = bool( step(st.x, 5.0/7.0 + 1.0/7.0/3.0*3.0) );


	float bottom_red =   0.03*float(fourth1) + float(fourth2 && !fourth1) + 0.22*float(fourth3 && !fourth2) + 0.075*float(fourth4 && !fourth3) + 0.075*float(littleThird2 && !littleThird1) + 0.15*float(littleThird3 && !littleThird2) + 0.075*float(!sev6);
	float bottom_green = 0.24*float(fourth1) + float(fourth2 && !fourth1)                                   + 0.075*float(fourth4 && !fourth3) + 0.075*float(littleThird2 && !littleThird1) + 0.15*float(littleThird3 && !littleThird2) + 0.075*float(!sev6);
	float bottom_blue =  0.35*float(fourth1) + float(fourth2 && !fourth1) + 0.5*float(fourth3 && !fourth2)  + 0.075*float(fourth4 && !fourth3) + 0.075*float(littleThird2 && !littleThird1) + 0.15*float(littleThird3 && !littleThird2) + 0.075*float(!sev6);

	gl_FragColor = vec4(top_red*float(row1)   + mid_red*float(row2 && !row1) + bottom_red*float(row3 && !row2), 
	                    top_green*float(row1) + mid_green*float(row2 && !row1) + bottom_green*float(row3 && !row2), 
	                    top_blue*float(row1)  + mid_blue*float(row2 && !row1) + bottom_blue*float(row3 && !row2),1.);
}