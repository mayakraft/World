// INPUTS

// vec4  gl_Position                  — vertex program must write to this
// float gl_PointSize  — same data as glPointSize if GL_VERTEX_PROGRAM_POINT_SIZE is enabled
// vec4  gl_Vertex     — input vertex position data (ie. glVertex3f)
// vec4  gl_Normal     — input vertex normal attribute (ie. glNormal3f)
// vec4  gl_Color      — input vertex color attribute (ie. glColor3f)
// vec4  gl_MultiTexCoord0 - gl_MultiTexCoord7 — 8 different texture coordinate attributes (ie. glMultiTexCoord)

void main() {
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}

// OUTPUTS

// vec4 gl_FrontColor           — write out color info in the vertex stage if you want
// vec4 gl_BackColor            — if GL_VERTEX_PROGRAM_TWO_SIDE is enabled
// vec4 gl_FrontSecondaryColor  — secondary output color (normally unused, get creative)
// vec4 gl_BackSecondaryColor   — same as above
// vec4 gl_TexCoord[0] - gl_TexCoord[7] — up to 8 transformed texture coordinates