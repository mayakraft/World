///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////        TINY OPENGL TOOLBOX         //////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
void fill(){ SHAPE_FILL = 1; }
void noFill(){ SHAPE_FILL = 0; }
// void text(const char *text, float x, float y, void *font){
void text(const char *text, float x, float y, float z){
  // GLUT_BITMAP_8_BY_13
  // GLUT_BITMAP_9_BY_15
  // GLUT_BITMAP_TIMES_ROMAN_10
  // GLUT_BITMAP_TIMES_ROMAN_24
  // GLUT_BITMAP_HELVETICA_10
  // GLUT_BITMAP_HELVETICA_12
  // GLUT_BITMAP_HELVETICA_18
  const char *c;
  // glRasterPos2f(x, 24+y);
  glRasterPos3f(x, y, z);
  for (c = text; *c != '\0'; c++){
    // todo need replacement
    // glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
  }
} 
void drawPoint(float x, float y, float z){
  GLfloat _point_vertex[] = { x, y, z };
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, _point_vertex);
  glDrawArrays(GL_POINTS, 0, 1);
  glDisableClientState(GL_VERTEX_ARRAY);
}
void drawLine(float x1, float y1, float z1, float x2, float y2, float z2){
  GLfloat _lines_vertices[6] = {x1, y1, z1, x2, y2, z2};
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, _lines_vertices);
  glDrawArrays(GL_LINES, 0, 2);
  glDisableClientState(GL_VERTEX_ARRAY);
}
void drawUnitOriginSquareFill(){
  static const GLfloat _unit_square_vertex[] = {
    0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f
  };
  static const GLfloat _unit_square_normals[] = {
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f
  };
  static const GLfloat _texture_coordinates[] = {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);  
  glVertexPointer(3, GL_FLOAT, 0, _unit_square_vertex);
  glNormalPointer(GL_FLOAT, 0, _unit_square_normals);
  glTexCoordPointer(2, GL_FLOAT, 0, _texture_coordinates);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}
void drawUnitOriginSquareWireframe(){
  static const GLfloat _unit_square_wireframe_vertex[] = {
    0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,    0.0f, 0.0f, 0.0f };
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, _unit_square_wireframe_vertex);
  glDrawArrays(GL_LINES, 0, 4);
  glDisableClientState(GL_VERTEX_ARRAY);
}
void drawUnitSquare(float x, float y, float z){
  glPushMatrix();
    glTranslatef(x, y, z);
    switch(SHAPE_FILL){
      case 0: drawUnitOriginSquareWireframe(); break;
      default: drawUnitOriginSquareFill(); break;
    }
  glPopMatrix();
}
void drawRect(float x, float y, float z, float width, float height){
  glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(width, height, 1.0);
    switch(SHAPE_FILL){
      case 0: drawUnitOriginSquareWireframe(); break;
      default: drawUnitOriginSquareFill(); break;
    }
  glPopMatrix();
}
void drawUnitPlaneWireframe(int subdivisions){
  GLfloat _plane_vertices[ (subdivisions+2) * 12 ];
  memset(_plane_vertices, 0, sizeof(_plane_vertices));
  for(int i = 0; i <= subdivisions+1; i++){
    float step = (float)i/subdivisions;
    _plane_vertices[ i * 12 + 0] = _plane_vertices[ i * 12 + 3] = _plane_vertices[ i * 12 + 7] = _plane_vertices[ i * 12 + 10] = step;
    _plane_vertices[ i * 12 + 4] = _plane_vertices[ i * 12 + 9] = 1.0;
  }
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, _plane_vertices);
  glDrawArrays(GL_LINES, 0, (subdivisions+1)*4);
  glDisableClientState(GL_VERTEX_ARRAY);
}
void drawUnitOriginPlane(int subdivisions){
  switch(SHAPE_FILL){
    case 0: drawUnitPlaneWireframe(subdivisions); break;
    default: drawUnitOriginSquareFill(); break;
  }
}
void drawPlane(float x, float y, float z, float width, float height, int subdivisions){
  glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(width, height, 1.0);
    switch(SHAPE_FILL){
      case 0: drawUnitPlaneWireframe(subdivisions); break;
      default: drawUnitOriginSquareFill(); break;
    }
  glPopMatrix();
}
// shapes that require some computed properties
GLint _sphere_stacks = 20; //7;
GLint _sphere_slices = 30; //13;
static float _unit_circle_outline_vertices[192];
static float _unit_circle_outline_normals[192];
static float _unit_circle_outline_texCoord[192];
static float _unit_circle_fill_vertices[198]; // includes 1 more point: the center
static float _unit_circle_fill_normals[198];
static float _unit_circle_fill_texCoord[198];
float *_unit_sphere_vertices, *_unit_sphere_normals, *_unit_sphere_texture;

void drawUnitOriginSphereFill(){
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);  
  glVertexPointer(3, GL_FLOAT, 0, _unit_sphere_vertices);
  glNormalPointer(GL_FLOAT, 0, _unit_sphere_normals);
  glTexCoordPointer(2, GL_FLOAT, 0, _unit_sphere_texture);
  // glDrawArrays(GL_LINE_LOOP, 0, _sphere_slices * _sphere_stacks * 2 );//(_sphere_slices+1) * 2 * (_sphere_stacks-1)+2  );
  glDrawArrays(GL_TRIANGLE_STRIP, 0,  _sphere_slices * _sphere_stacks * 2 );// (_sphere_slices+1) * 2 * (_sphere_stacks-1)+2  );
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}
void drawUnitOriginSphereWireframe(int subdivisions){
  glPushMatrix();
    drawUnitCircle(0, 0, 0);  // equator
    // latitude
    for(float pos = 1.0/(subdivisions*0.5); pos < 1.0; pos += 1.0/(subdivisions*0.5)){
      glPushMatrix();
        float r = cosf(pos*M_PI*0.5);
        r = sqrt(1 - powf(pos,2));
        glScalef(r, r, 1.0);
        drawUnitCircle(0, 0, -pos);
        drawUnitCircle(0, 0, pos);
      glPopMatrix();
    }
    // longitude
    glRotatef(90, 0, 1, 0); drawUnitCircle(0, 0, 0);
    float angle = 180.0 / subdivisions;
    for(int pos = 0; pos < subdivisions-1; pos++){
      glRotatef(angle, 1, 0, 0); drawUnitCircle(0, 0, 0);
    }
  glPopMatrix();
}
void drawUnitSphere(float x, float y, float z){
  glPushMatrix();
    glTranslatef(x, y, z);
    switch(SHAPE_FILL){
      case 0: drawUnitOriginSphereWireframe(6); break;
      default: drawUnitOriginSphereFill(); break;
    }
  glPopMatrix();
}
void drawSphere(float x, float y, float z, float radius){
  glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(radius, radius, radius);
    switch(SHAPE_FILL){
      case 0: drawUnitOriginSphereWireframe(6); break;
      default: drawUnitOriginSphereFill(); break;
    }
  glPopMatrix();
}
void drawUnitOriginCircleFill(){
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);  
  glVertexPointer(3, GL_FLOAT, 0, _unit_circle_fill_vertices);
  glNormalPointer(GL_FLOAT, 0, _unit_circle_fill_normals);
  glTexCoordPointer(2, GL_FLOAT, 0, _unit_circle_fill_texCoord);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 66);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}
void drawUnitOriginCircleWireframe(){
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, _unit_circle_outline_vertices);
  glDrawArrays(GL_LINE_LOOP, 0, 64);
  glDisableClientState(GL_VERTEX_ARRAY);
}
void drawUnitOriginCircle(){
  switch(SHAPE_FILL){
    case 0: drawUnitOriginCircleWireframe(); break;
    default: drawUnitOriginCircleFill(); break;
  }
}
void drawUnitCircle(float x, float y, float z){
  glPushMatrix();
    glTranslatef(x, y, z);
    drawUnitOriginCircle();
  glPopMatrix();
}
void drawCircle(float x, float y, float z, float radius){
  glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(radius, radius, 1.0);
    drawUnitOriginCircle();
  glPopMatrix();
}
void draw3DAxesLines(float x, float y, float z, float scale){
  static const GLfloat _axis_lines_vertices[] = {
    1.0f, 0.0f, 0.0f,    -1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,     0.0f, -1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,     0.0f, 0.0f, -1.0f};
  glPushMatrix();
  glTranslatef(x, y, z);
  glScalef(scale, scale, scale);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, _axis_lines_vertices);
  glDrawArrays(GL_LINES, 0, 6);
  glDisableClientState(GL_VERTEX_ARRAY);
  glPopMatrix();
}
const float _tetrahedron_points[12] = {1.0,0.0,0.0,-0.3333,-0.9428,0.0,-0.3333,0.4714,0.81649,-0.3333,0.4714,-0.8164};
const unsigned short _tetrahedron_lines[12] = {2,3,2,0,2,1,3,0,3,1,0,1};
const unsigned short _tetrahedron_faces[12] = {2,1,3,2,3,0,2,0,1,3,1,0};
const float _tetrahedron_dual_points[12] = {-1.0,0.0,0.0,0.3333,0.9428,0.0,0.3333,-0.4714,0.81649,0.3333,-0.4714,-0.81649};
const unsigned short _tetrahedron_dual_lines[12] = {2,3,2,0,2,1,3,0,3,1,0,1};
const unsigned short _tetrahedron_dual_faces[12] = {2,1,3,2,3,0,2,0,1,3,1,0};
const float _octahedron_points[18] = {1.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,1.0,-1.0,0.0,0.0,0.0,-1.0,0.0,0.0,0.0,-1.0};
const unsigned short _octahedron_lines[24] = {1,0,1,2,1,5,1,3,3,2,2,0,0,5,5,3,4,2,4,3,4,5,4,0};
const unsigned short _octahedron_faces[24] = {1,0,2,1,5,0,4,0,5,4,2,0,1,2,3,1,3,5,4,5,3,4,3,2};
const float _hexahedron_points[24] = {0.57735,0.57735,0.57735,0.57735,0.57735,-0.57735,0.57735,-0.57735,-0.57735,0.57735,-0.57735,0.57735,-0.57735,0.57735,0.57735,-0.57735,0.57735,-0.57735,-0.57735,-0.57735,-0.57735,-0.57735,-0.57735,0.57735};
const unsigned short _hexahedron_lines[24] = {0,1,1,2,2,3,3,0,0,4,1,5,2,6,3,7,4,5,5,6,6,7,7,4};
const unsigned short _hexahedron_faces[24] = {0,3,2,1,4,0,1,5,0,3,7,4,7,4,5,6,3,7,6,2,1,5,6,2};
const unsigned short _hexahedron_triangle_faces[36] = {0,2,3,2,0,1,4,1,0,1,4,5,0,3,7,7,4,0,7,5,4,5,7,6,3,6,7,6,3,2,1,5,6,6,2,1};
const float _icosahedron_points[36] = {0.44721,-0.2763,0.85065,-0.44721,0.2763,0.85065,-0.44721,0.2763,-0.85065,0.44721,-0.2763,-0.85065,-0.44721,-0.7236,0.52573,0.44721,0.7236,0.52573,0.44721,0.7236,-0.52573,-0.44721,-0.7236,-0.52573,0.44721,-0.8944,0.0,1.0,0.0,0.0,-0.44721,0.8944,0.0,-1.0,0.0,0.0};
const unsigned short _icosahedron_lines[60] = {0,8,0,9,0,1,0,4,0,5,8,3,8,9,8,7,8,4,9,3,9,6,9,5,7,4,7,3,7,2,7,11,2,10,2,11,2,3,2,6,10,11,10,5,10,6,10,1,11,1,11,4,4,1,5,1,5,6,6,3};
const unsigned short _icosahedron_faces[60] = {8,7,4,7,8,3,8,4,0,8,0,9,9,3,8,9,0,5,9,5,6,9,6,3,3,2,7,3,6,2,0,4,1,0,1,5,11,4,7,11,7,2,11,2,10,1,11,10,11,1,4,10,6,5,10,5,1,10,2,6};
const float _dodecahedron_points[60] = {-0.7946,0.4911,0.3568,-0.1875,0.7946,-0.5773,-0.7946,0.4911,-0.3568,-0.7946,-0.1875,-0.5773,0.1875,0.3035,-0.9341,0.1875,0.9822,0.0,0.1875,-0.7946,-0.5773,-0.1875,-0.3035,-0.9341,-0.1875,-0.9822,0.0,-0.7946,-0.6070,0.0,0.7946,-0.4911,-0.3568,0.1875,-0.7946,0.5773,-0.1875,-0.3035,0.9341,-0.7946,-0.1875,0.5773,0.7946,-0.4911,0.3568,-0.1875,0.7946,0.5773,0.1875,0.3035,0.9341,0.7946,0.1875,0.5773,0.7946,0.6070,0.0,0.7946,0.1875,-0.5773};
const unsigned short _dodecahedron_lines[60] = {19,18,18,17,17,14,14,10,10,19,14,11,11,8,8,6,6,10,19,4,4,1,1,5,5,18,9,3,3,7,7,6,8,9,13,9,3,2,2,0,0,13,12,11,12,13,1,2,5,15,15,0,15,16,16,17,16,12,4,7};
const unsigned short _dodecahedron_triangle_faces[108] = {5,2,1,15,0,5,2,5,0,9,2,13,2,0,13,3,2,9,10,17,19,18,19,17,14,17,10,19,1,4,6,9,8,1,18,5,11,14,8,18,1,19,8,9,11,10,8,14,8,10,6,6,7,9,9,7,3,13,12,9,12,11,9,18,15,5,17,15,18,15,17,16,11,12,14,14,12,17,17,12,16,12,13,16,13,0,16,0,15,16,7,2,3,2,7,1,1,7,4,4,7,19,19,7,10,6,10,7};
const unsigned int _platonic_num_vertices[6] = {4,6,8,12,20,4};
const unsigned int _platonic_num_lines[6] = {6,12,12,30,30,6};
const unsigned int _platonic_num_faces[6] = {4,8,12,20,36,4};//{4,8,6,20,12,4};
const float* _platonic_point_arrays[6] = {_tetrahedron_points,_octahedron_points,_hexahedron_points,_icosahedron_points,_dodecahedron_points,_tetrahedron_dual_points};
const unsigned short* _platonic_line_array[6] = {_tetrahedron_lines,_octahedron_lines,_hexahedron_lines,_icosahedron_lines,_dodecahedron_lines,_tetrahedron_dual_lines};
const unsigned short* _platonic_face_array[6] = {_tetrahedron_faces,_octahedron_faces,_hexahedron_triangle_faces,_icosahedron_faces,_dodecahedron_triangle_faces,_tetrahedron_dual_faces};
const int _platonic_dual_index[6] = { 5,2,1,4,3,0 };
void drawPlatonicSolidFaces(char solidType){
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, _platonic_point_arrays[solidType]);
  glNormalPointer(GL_FLOAT, 0, _platonic_point_arrays[ solidType ]);
  glDrawElements(GL_TRIANGLES, 3*_platonic_num_faces[solidType], GL_UNSIGNED_SHORT, _platonic_face_array[solidType]);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}
void drawPlatonicSolidLines(char solidType){
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, _platonic_point_arrays[solidType]);
  glNormalPointer(GL_FLOAT, 0, _platonic_point_arrays[ solidType ]);
  glDrawElements(GL_LINES, 2*_platonic_num_lines[solidType], GL_UNSIGNED_SHORT, _platonic_line_array[solidType]);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}
void drawPlatonicSolidPoints(char solidType){
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, _platonic_point_arrays[solidType]);
  glNormalPointer(GL_FLOAT, 0, _platonic_point_arrays[ solidType ]);
  glDrawArrays(GL_POINTS, 0, _platonic_num_vertices[solidType]);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}
void drawTetrahedron(float scale){
  glPushMatrix();
  glScalef(scale, scale, scale);
  switch(SHAPE_FILL){
    case 0: drawPlatonicSolidLines(0); break;
    default: drawPlatonicSolidFaces(0); break;
  }
  glPopMatrix();
}
void drawOctahedron(float scale){
  glPushMatrix();
  glScalef(scale, scale, scale);
  switch(SHAPE_FILL){
    case 0: drawPlatonicSolidLines(1); break;
    default: drawPlatonicSolidFaces(1); break;
  }
  glPopMatrix();
}
void drawHexahedron(float scale){
  glPushMatrix();
  glScalef(scale, scale, scale);
  switch(SHAPE_FILL){
    case 0: drawPlatonicSolidLines(2); break;
    default: drawPlatonicSolidFaces(2); break;
  }
  glPopMatrix();
}
void drawCube(float scale){ drawHexahedron(scale); }
void drawIcosahedron(float scale){
  glPushMatrix();
  glScalef(scale, scale, scale);
  switch(SHAPE_FILL){
    case 0: drawPlatonicSolidLines(3); break;
    default: drawPlatonicSolidFaces(3); break;
  }
  glPopMatrix();
}
void drawDodecahedron(float scale){
  glPushMatrix();
  glScalef(scale, scale, scale);
  switch(SHAPE_FILL){
    case 0: drawPlatonicSolidLines(4); break;
    default: drawPlatonicSolidFaces(4); break;
  }
  glPopMatrix();
}
void initPrimitives(){
  static unsigned char _geometry_initialized = 0;
  if (!_geometry_initialized) {
    // CIRCLE
    for(int i = 0; i < 64; i++){
      _unit_circle_outline_vertices[i*3+0] = -sinf(M_PI*2/64.0f*i);
      _unit_circle_outline_vertices[i*3+1] = cosf(M_PI*2/64.0f*i);
      _unit_circle_outline_vertices[i*3+2] = 0.0f;
      _unit_circle_outline_normals[i*3+0] = _unit_circle_outline_normals[i*3+1] = 0.0;
      _unit_circle_outline_normals[i*3+2] = 1.0;
      _unit_circle_outline_texCoord[i*3+0] = -sinf(M_PI*2/64.0f*i)*0.5 + 0.5;
      _unit_circle_outline_texCoord[i*3+1] = cosf(M_PI*2/64.0f*i)*0.5 + 0.5;
    }
    _unit_circle_fill_vertices[0] = 0.0f;
    _unit_circle_fill_vertices[1] = 0.0f;
    _unit_circle_fill_vertices[2] = 0.0f;
    _unit_circle_fill_normals[0] = _unit_circle_fill_normals[1] = 0.0f;
    _unit_circle_fill_normals[2] = 1.0f;
    _unit_circle_fill_texCoord[0] = 0.0f;
    _unit_circle_fill_texCoord[1] = 0.0f;
    for(int i = 1; i <= 65; i++){
      _unit_circle_fill_vertices[i*3+0] = -sinf(M_PI*2/64.0f*(i-1));
      _unit_circle_fill_vertices[i*3+1] = cosf(M_PI*2/64.0f*(i-1));
      _unit_circle_fill_vertices[i*3+2] = 0.0f;
      _unit_circle_fill_normals[i*3+0] = _unit_circle_fill_normals[i*3+1] = 0.0f;
      _unit_circle_fill_normals[i*3+2] = 1.0f;
      _unit_circle_fill_texCoord[i*3+0] = -sinf(M_PI*2/64.0f*(i-1))*0.5 + 0.5f;
      _unit_circle_fill_texCoord[i*3+1] = cosf(M_PI*2/64.0f*(i-1))*0.5 + 0.5f;
    }
    // SPHERE
    GLfloat m_Scale = 1;
    GLfloat *vPtr = _unit_sphere_vertices = (GLfloat*)malloc(sizeof(GLfloat) * 3 * ((_sphere_slices*2+2) * (_sphere_stacks)));
    GLfloat *nPtr = _unit_sphere_normals = (GLfloat*)malloc(sizeof(GLfloat) * 3 * ((_sphere_slices*2+2) * (_sphere_stacks)));
    GLfloat *tPtr = _unit_sphere_texture = (GLfloat*)malloc(sizeof(GLfloat) * 2 * ((_sphere_slices*2+2) * (_sphere_stacks)));
    for(unsigned int phiIdx = 0; phiIdx < _sphere_stacks; phiIdx++){
      // Latitude
      //starts at -pi/2 goes to pi/2
      float phi0 = M_PI * ((float)(phiIdx+0) * (1.0/(float)(_sphere_stacks)) - 0.5);  // the first circle
      float phi1 = M_PI * ((float)(phiIdx+1) * (1.0/(float)(_sphere_stacks)) - 0.5);  // second one
      float cosPhi0 = cos(phi0);
      float sinPhi0 = sin(phi0);
      float cosPhi1 = cos(phi1);
      float sinPhi1 = sin(phi1);
      for(unsigned int thetaIdx = 0; thetaIdx < _sphere_slices; thetaIdx++){
        //longitude
        float theta = 2.0*M_PI * ((float)thetaIdx) * (1.0/(float)(_sphere_slices - 1));
        float cosTheta = cos(theta+M_PI*.5);
        float sinTheta = sin(theta+M_PI*.5);
        vPtr[0] = m_Scale*cosPhi0 * cosTheta;
        vPtr[1] = m_Scale*(cosPhi0 * sinTheta);
        vPtr[2] = -m_Scale*sinPhi0;
        vPtr[3] = m_Scale*cosPhi1 * cosTheta;
        vPtr[4] = m_Scale*(cosPhi1 * sinTheta);
        vPtr[5] = -m_Scale*sinPhi1;
        nPtr[0] = cosPhi0 * cosTheta;
        nPtr[1] = cosPhi0 * sinTheta;
        nPtr[2] = -sinPhi0;
        nPtr[3] = cosPhi1 * cosTheta;
        nPtr[4] = cosPhi1 * sinTheta;
        nPtr[5] = -sinPhi1;
        GLfloat texX = (float)thetaIdx * (1.0f/(float)(_sphere_slices-1));
        tPtr[0] = texX;
        tPtr[1] = (float)(phiIdx + 0) * (1.0f/(float)(_sphere_stacks));
        tPtr[2] = texX;
        tPtr[3] = (float)(phiIdx + 1) * (1.0f/(float)(_sphere_stacks));
        vPtr += 2*3;
        nPtr += 2*3;
        tPtr += 2*2;
      }
    }
    _geometry_initialized = 1;
  }
}

void drawAxesLabels(float scale){
  text("+X", scale, 0, 0);  text("-X", -scale, 0, 0);
  text("+Y", 0, scale, 0);  text("-Y", 0, -scale, 0);
  text("+Z", 0, 0, scale);  text("-Z", 0, 0, -scale);
}
void drawAxesCoordinates(float scale){
  int scaleInt = scale;
  char string[50];
  sprintf(string, "(%d, 0, 0)", scaleInt);   text(string, scale, 0, 0);
  sprintf(string, "(0, %d, 0)", scaleInt);   text(string, 0, scale, 0);
  sprintf(string, "(0, 0, %d)", scaleInt);   text(string, 0, 0, scale);
  sprintf(string, "(%d, 0, 0)", -scaleInt);  text(string, -scale, 0, 0);
  sprintf(string, "(0, %d, 0)", -scaleInt);  text(string, 0, -scale, 0);
  sprintf(string, "(0, 0, %d)", -scaleInt);  text(string, 0, 0, -scale);
}
void drawCheckerboard(float walkX, float walkY, int numSquares){
  static GLfloat mat_white[] = { 1.0, 1.0, 1.0, 1.0 };
  static GLfloat mat_black[] = { 0.0, 0.0, 0.0, 1.0 };
  int XOffset = ceil(walkX);
  int YOffset = ceil(walkY);
  int evenOdd = (numSquares%2);
  if(evenOdd) 
    numSquares--;
  for(int i = -numSquares*.5; i < numSquares*.5; i++){
    for(int j = -numSquares*.5; j < numSquares*.5; j++){
      int b = abs(((i+j+XOffset+YOffset)%2));
      if(b) { glColor3f(1.0, 1.0, 1.0); glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_white); }
      else { glColor3f(0.0, 0.0, 0.0); glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_black); }
      drawUnitSquare(i-XOffset - evenOdd, j-YOffset - evenOdd, 0);
    }
  }
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_white);
}
// span: how many units to skip inbetween each axis
// repeats: how many rows/cols/stacks on either side of center
void drawAxesGrid(float walkX, float walkY, float walkZ, int span, int repeats){
  float XSpanMod = walkX - floor(walkX/span)*span;
  float YSpanMod = walkY - floor(walkY/span)*span;
  float ZSpanMod = walkZ - floor(walkZ/span)*span;
  for(int i = -repeats*span; i < repeats*span; i+=span){
    for(int j = -repeats*span; j < repeats*span; j+=span){
      for(int k = -repeats*span; k < repeats*span; k+=span){
        // distance approximation works just fine in this case
        float distance = fabs(i+XSpanMod-1) + fabs(j+YSpanMod-1) + abs(k);
        float brightness = 1.0 - distance/(repeats*span);
        GLfloat material[] = { 1.0, 1.0, 1.0, brightness };
        glColor4f(1.0, 1.0, 1.0, brightness);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, material);
        // glLineWidth(100.0/distance/distance);
        draw3DAxesLines(i + XSpanMod - walkX,
                        j + YSpanMod - walkY,
                        k + ZSpanMod - walkZ, 1.0);
      }
    }
  }
}
