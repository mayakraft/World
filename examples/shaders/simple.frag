// INPUTS (outputs from Vertex Shader)

// vec4 gl_FrontColor           — write out color info in the vertex stage if you want
// vec4 gl_BackColor            — if GL_VERTEX_PROGRAM_TWO_SIDE is enabled
// vec4 gl_FrontSecondaryColor  — secondary output color (normally unused, get creative)
// vec4 gl_BackSecondaryColor   — same as above
// vec4 gl_TexCoord[0] - gl_TexCoord[7] — up to 8 transformed texture coordinates

void main(){
	gl_FragColor = gl_Color;
}

// OUTPUTS

// vec4 gl_FragColor     — fragment program must write to this
// vec4 gl_FragData[0 - gl_MaxDrawBuffers] — same as gl_FragColor but for any framebuffer that you had bound
// vec4 gl_FragDepth     — normally glFragCoord.z (depth buffer value)
// vec4 gl_FragCoord     — This is the pixel coordinate within the buffer
// bool gl_FrontFacing   — boolean flag representing the 'direction' of the fragment (away or toward the view)