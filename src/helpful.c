void typicalOpenGLSettings(){
  firstPersonPerspective();
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glShadeModel(GL_SMOOTH); // GL_FLAT
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_POLYGON_SMOOTH);
  glEnable(GL_POINT_SMOOTH);
  // glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LESS);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glLineWidth(1);
}

/////////////////////////    HELPFUL ORIENTATION    //////////////////////////
void simpleLights(){
  GLfloat red[] =   {1.0f, 0.2f, 0.0f, 0.0f};
  GLfloat green[] = {0.3f, 0.9f, 0.3f, 0.0f};
  GLfloat blue[] =  {0.0f, 0.2f, 1.0f, 0.0f};
  GLfloat light_position0[] = { 0.0, 100.0, 0.0, 1.0};
  GLfloat light_position1[] = { 87.0, -50.0, 0.0, 1.0};
  GLfloat light_position2[] = { -87.0, -50.0, 0.0, 1.0 };
  glLightfv(GL_LIGHT0, GL_DIFFUSE, red);
  glLightfv(GL_LIGHT0, GL_SPECULAR, red);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, green);
  glLightfv(GL_LIGHT1, GL_SPECULAR, green);
  glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, blue);
  glLightfv(GL_LIGHT2, GL_SPECULAR, blue);
  glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHT2);
  glEnable(GL_LIGHTING);
}
void simpleGrayLights(){
  GLfloat red[] =   {1.0f, 1.0f, 1.0f, 0.0f};
  GLfloat green[] = {1.0f, 1.0f, 1.0f, 0.0f};
  GLfloat blue[] =  {1.0f, 1.0f, 1.0f, 0.0f};
  GLfloat light_position0[] = { 0.0, 100.0, 40.0, 1.0};
  GLfloat light_position1[] = { 87.0, -50.0, -40.0, 1.0};
  GLfloat light_position2[] = { -87.0, -50.0, 0.0, 1.0 };
  glLightfv(GL_LIGHT0, GL_DIFFUSE, red);
  glLightfv(GL_LIGHT0, GL_SPECULAR, red);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, green);
  glLightfv(GL_LIGHT1, GL_SPECULAR, green);
  glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, blue);
  glLightfv(GL_LIGHT2, GL_SPECULAR, blue);
  glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHT2);
  glEnable(GL_LIGHTING);
}
void headsUpDisplay(int x, int y, int z){
  static char monthStrings[][10] = { "JANUARY", "FEBRUARY", "MARCH", "APRIL", "MAY", "JUNE", "JULY", "AUGUST", "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER" };
  switch(PERSPECTIVE){
    case FPP:   text("First Person Perspective", x, y, z); break;
    case POLAR: text("Polar Perspective", x, y, z); break;
    case ORTHO: text("Orthographic Perspective", x, y, z); break;
  }
  char line1String[50], line2String[50], line3String[50];
  switch(PERSPECTIVE){
    case ORTHO:
    sprintf(line1String, "X:%.1f, Y:%.1f, W:%.1f, H:%.1f", FRAME[0], FRAME[1], FRAME[2], FRAME[3] );
    float mWorldX = mouseX/(float)WIDTH * FRAME[2] + FRAME[0];
    float mWorldY = mouseY/(float)HEIGHT * FRAME[3] + FRAME[1];
    sprintf(line2String, "MOUSE WORLD (%.2f, %.2f)", mWorldX, mWorldY);
    break;
    case FPP: case POLAR:
    sprintf(line1String, "LOOK AZ:%.2f, ALT:%.2f, ZOOM:%.2f", HORIZON[0], HORIZON[1], HORIZON[2]);
    sprintf(line2String, "ORIGIN X:%.2f, Y:%.2f, Z:%.2f", ORIGIN[0], ORIGIN[1], ORIGIN[2]);   
    break;
  }
  char line4String[70];
  sprintf(line3String, "MOUSE SCREEN (%d, %d)", mouseX, mouseY );
  sprintf(line4String, "%d %s %d  %02d:%02d:%02d UTC", YEAR, monthStrings[(MONTH-1)%12], DAY, HOUR, MINUTE, SECOND);
  text(line1String, x, y+13*1, z);
  text(line2String, x, y+13*2, z);
  text(line3String, x, y+13*3, z);
  text(line4String, x, y+13*4, z);
}