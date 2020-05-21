void updateTime (){
  // input is human-readable: March 1st is month:03 day:01
  time_t current; time(&current);
  // todo, get time zone
  struct tm GMT;  GMT = *gmtime(&current);
  YEAR = GMT.tm_year + 1900; // The number of years since 1900   
  MONTH = GMT.tm_mon + 1;    // range 0 to 11  
  DAY = GMT.tm_mday;         // range 1 to 31  
  HOUR = GMT.tm_hour;        // range 0 to 23  
  MINUTE = GMT.tm_min;       // range 0 to 59  
  SECOND = GMT.tm_sec;       // range 0 to 59  
  // int tm_isdst;           // daylight saving time     
}
void UPDATE () {
  updateTime();
  clock_gettime(CLOCK_MONOTONIC, &CURRENT);
  ELAPSED = (CURRENT.tv_sec - START.tv_sec);
  ELAPSED += (CURRENT.tv_nsec - START.tv_nsec) / 1000000000.0;
  // keyboard input
  if(SETTINGS & (1 << BIT_KEYBOARD_MOVE)){
    moveOriginWithArrowKeys();
  }
  if(SETTINGS & (1 << BIT_KEYBOARD_ZOOM)){
    if(KEYBOARD[MINUS_KEY]){
      switch(PERSPECTIVE){
        case FPP:
          FOV += 0.001; break;
        case POLAR:
          HORIZON[2] += ZOOM_SPEED; break;
        case ORTHO: {
          float dH = -ZOOM_SPEED;
          FRAME[3] -= dH;
          FRAME[1] += dH * 0.5;
          float newW = FRAME[3] * ((float)WIDTH / (float)HEIGHT);
          float dW = FRAME[2] - newW;
          FRAME[2] = newW;
          FRAME[0] += dW * 0.5;
        } break;
      }
      rebuildProjection();
    }
    if(KEYBOARD[PLUS_KEY]){
      switch(PERSPECTIVE){
        case FPP:
          FOV -= 0.001; break;
        case POLAR:
          HORIZON[2] -= ZOOM_SPEED; break;
        case ORTHO:{
          float dH = ZOOM_SPEED;
          FRAME[3] -= dH;
          FRAME[1] += dH * 0.5;
          float newW = FRAME[3] * ((float)WIDTH / (float)HEIGHT);
          float dW = FRAME[2] - newW;
          FRAME[2] = newW;
          FRAME[0] += dW * 0.5;
        } break;
      }
      rebuildProjection();
    }
  }
  update();
  // glutPostRedisplay();
}
void DRAW (GLFWwindow* window) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();
    glPushMatrix();
      glColor4f(1.0, 1.0, 1.0, 1.0);
      if (SETTINGS & (1 << BIT_KEYBOARD_MOVE)) {
        glTranslatef(-ORIGIN[0], -ORIGIN[1], -ORIGIN[2]);
      }
      draw3D();
    glPopMatrix();
    // // 3D REPEATED STRUCTURE
    if (SETTINGS & (1 << BIT_SHOW_GRID)) {
      float newX = modulusContext(-ORIGIN[0], 5);
      float newY = modulusContext(-ORIGIN[1], 5);
      float newZ = modulusContext(-ORIGIN[2], 5);
      glPushMatrix();
        glTranslatef(newX, newY, newZ);
        drawAxesGrid(newX, newY, newZ, 5, 4);
      glPopMatrix();
    }
    // 2D REPEATED STRUCTURE
    if (SETTINGS & (1 << BIT_SHOW_GROUND)) {
      float newX = modulusContext(-ORIGIN[0], 2);
      float newY = modulusContext(-ORIGIN[1], 2);
      glPushMatrix();
        glTranslatef(newX, newY, -ORIGIN[2]);
        drawCheckerboard(newX, newY, 8);
      glPopMatrix();
    }
  glPopMatrix();
  
  // TO ORTHOGRAPHIC
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  switch (HANDED) {
    case LEFT: glOrtho(0, WIDTH, HEIGHT, 0, -100.0, 100.0); break;
    case RIGHT: glOrtho(0, WIDTH, 0, HEIGHT, -100.0, 100.0); break;
  }
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
    glColor4f(1.0, 1.0, 1.0, 1.0);
    draw2D();
  glPopMatrix();
  rebuildProjection();

  // bring back buffer to the front on vertical refresh, auto-calls glFlush
  glfwSwapBuffers(window);
  // glutSwapBuffers();
  // glFlush();
}
