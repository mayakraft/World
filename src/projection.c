void windowSizeHandler (GLFWwindow* window, int width, int height) {
  int w, h;
  glfwGetFramebufferSize(window, &w, &h);
  // reshapeWindow(width, height);
  reshapeWindow(w, h);
  DRAW(window);
}

// unsigned char IsFullscreen () {
//   return glfwGetWindowMonitor(WINDOW) != NULL;
// }

void toggleFullscreen () {
  static int w = 640;
  static int h = 480;
  static int posX = 0;
  static int posY = 0;
  FULLSCREEN = !FULLSCREEN;
  if (FULLSCREEN) {
    // backup window position and window size
    glfwGetWindowPos(WINDOW, &posX, &posY);
    glfwGetWindowSize(WINDOW, &w, &h);
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowMonitor(WINDOW, MONITOR, 0, 0, mode->width, mode->height, 0);
  }
  else {
    glfwSetWindowMonitor(WINDOW, NULL, posX, posY, w, h, 0);
  }
}

void firstPersonPerspective(){
  PERSPECTIVE = FPP;
  float a = (float)min(WIDTH, HEIGHT) / max(WIDTH, HEIGHT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (WIDTH < HEIGHT) { glFrustum(-FOV, FOV, -FOV/a, FOV/a, NEAR_CLIP, FAR_CLIP); }
  else                { glFrustum(-FOV/a, FOV/a, -FOV, FOV, NEAR_CLIP, FAR_CLIP); }
  // change POV
  glRotatef(-90-HORIZON[1], 1, 0, 0);
  glRotatef(90+HORIZON[0], 0, 0, 1);
  // for left handed, invert the Y
  if (!HANDED) { glMultMatrixf(_invert_y_m); }
  // raise POV 1.0 above the floor, 1.0 is an arbitrary value
  glTranslatef(0.0f, 0.0f, -EYE_HEIGHT);
  glMatrixMode(GL_MODELVIEW);
}
void polarPerspective(){
  PERSPECTIVE = POLAR;
  float a = (float)min(WIDTH, HEIGHT) / max(WIDTH, HEIGHT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (WIDTH < HEIGHT) { glFrustum(-FOV, FOV, -FOV/a, FOV/a, NEAR_CLIP, FAR_CLIP); }
  else                { glFrustum(-FOV/a, FOV/a, -FOV, FOV, NEAR_CLIP, FAR_CLIP); }
  // change POV
  glTranslatef(0, 0, -HORIZON[2]);
  glRotatef(-90+HORIZON[1], 1, 0, 0);
  glRotatef(90+180+HORIZON[0], 0, 0, 1);
  // for left handed, invert the Y
  if(!HANDED){ glMultMatrixf(_invert_y_m); }
  glMatrixMode(GL_MODELVIEW);
}
void orthoPerspective(float x, float y, float width, float height){
  PERSPECTIVE = ORTHO;
  FRAME[0] = x;
  FRAME[1] = y;
  FRAME[2] = width;
  FRAME[3] = height;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  switch(HANDED){
    case LEFT: glOrtho(x, width + x, height + y, y, -FAR_CLIP, FAR_CLIP); break;
    case RIGHT: glOrtho(x, width + x, y, height + y, -FAR_CLIP, FAR_CLIP); break;
  }
  glMatrixMode(GL_MODELVIEW);
}
void rebuildProjection(){
  switch(PERSPECTIVE){
    case FPP:   firstPersonPerspective(); break;
    case POLAR: polarPerspective(); break;
    case ORTHO: orthoPerspective(FRAME[0], FRAME[1], FRAME[2], FRAME[3]); break;
  }
}

void reshapeWindow (int w, int h) {
  WIDTH = w;
  HEIGHT = h;
  ASPECT = (float)WIDTH / (float)HEIGHT;
  glViewport(0, 0, (GLsizei) WIDTH, (GLsizei) HEIGHT);
  // update orthographic frame with new aspect ratio
  float newW = FRAME[3] * ASPECT;
  float dW = FRAME[2] - newW;
  FRAME[2] = newW;
  FRAME[0] += dW * 0.5;
  rebuildProjection();
  UPDATE();
}
