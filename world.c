// example 1
//
// this example illustrates how to use fill() and noFill()
// to create an opaque wireframe model by
//  1) drawing a filled black shape
//  2) drawing a wireframe scaled up by a small amount: 1.005

#include "world.h"

void setup(){
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  switch(HANDED){
    case LEFT:  glCullFace(GL_BACK); break;
    case RIGHT: glCullFace(GL_FRONT); break;
  } 
  SETTINGS ^= SET_SHOW_GROUND | SET_SHOW_GRID | SET_KEYBOARD_MOVE;
  HORIZON[2] = 2.5;
  HORIZON[1] = 5;
  polarPerspective();
}
void update(){
  HORIZON[0] += 0.1;
  HORIZON[1] += 0.05*sinf(ELAPSED/4);
  HORIZON[2] += -0.0025*sinf(ELAPSED/3);
}
void draw3D(){ 
  glPushMatrix();
    noFill();
    glColor3f(0.6, 0.1, 0.3);
    drawPlane(-50, -50, -0.5, 100, 100, 100);
  glPopMatrix();

  float size1 = cosf(ELAPSED*0.5)*0.12 + 0.88;
  float size2 = -cosf(ELAPSED*0.5)*0.12 + 0.88;
  glPushMatrix();
    glTranslatef(0.0, 0.0, 0.2);
    glRotatef(ELAPSED*20, 1, 0, 0);
    glRotatef(ELAPSED*44, 0, 1, 0);
    fill();
    glColor4f(0.0, 0.0, 0.0, 0.5);
    drawDodecahedron(size1);
    drawIcosahedron(size2);
    noFill();
    glScalef(1.005, 1.005, 1.005);
    glColor4f(0.3, 0.5, 0.9, 1.0);
    drawDodecahedron(size1);
    drawIcosahedron(size2);
  glPopMatrix();
}
void draw2D(){ }
void keyDown(unsigned int key){ }
void keyUp(unsigned int key){ }
void mouseDown(unsigned int button){ }
void mouseUp(unsigned int button){ }
void mouseMoved(int x, int y){ }