// GLFW_PRESS, GLFW_REPEAT or GLFW_RELEASE
static void keyboardHandler (GLFWwindow* window, int key, int scancode, int action, int mods) {
  // printf("PRESS %d, %d, %d\n", GLFW_PRESS, GLFW_REPEAT, GLFW_RELEASE);
  if (key == GLFW_KEY_UNKNOWN) { return; }
  // if (KEYBOARD[key] == 1){ return; }  // prevent repeated keyboard calls
  if (action == GLFW_REPEAT) { return; }

  KEYBOARD[key] = action;
  // printf("key: %d, action: %d\n", key, action);
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
    glfwSetWindowShouldClose(window, 1);
  }
  if (action == GLFW_RELEASE) { return; }

  KEYBOARD[key] = 1;
  if(SETTINGS & (1 << BIT_KEYBOARD_FUNCTIONS)){
    if(key == ESCAPE_KEY){ exit (0); }
    else if(key == 'F' || key == 'f'){
      toggleFullscreen();
    }
    else if(key == 'P' || key == 'p'){
      PERSPECTIVE = (PERSPECTIVE+1)%3;
      rebuildProjection();
    }
    else if(key == '.'){
      FOV += 0.01;
      rebuildProjection();
    }
    else if(key == ','){
      FOV -= 0.01;
      rebuildProjection();
    }
    else if(key == 'G' || key == 'g'){
      SETTINGS ^= SET_SHOW_GROUND;
      // glutPostRedisplay();
    }
    else if (key == 'X' || key == 'x'){
      SETTINGS ^= SET_SHOW_GRID;
      // glutPostRedisplay();
    }
  }
  keyDown(key);
  if(!CONTINUOUS_REFRESH){
    // keyboardSetIdleFunc(); // for efficient screen draw, trigger redraw if needed
  }
}

static void mousePositionHandler (GLFWwindow* window, double xpos, double ypos) {
  int leftState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
  // when mouse is moving but no buttons are pressed
  if (leftState != GLFW_PRESS) {
    mouseX = xpos;
    switch(HANDED){
      case LEFT: mouseY = ypos; break;
      case RIGHT: mouseY = HEIGHT - ypos; break;
    }
    mouseMoved(mouseX, mouseY);
    return;
  }
  int y_correct = ypos;
  switch(HANDED){
    case LEFT: y_correct = ypos; break;
    case RIGHT: y_correct = HEIGHT - ypos; break;
  }
  // change since last mouse event
  if(SETTINGS & (1 << BIT_MOUSE_LOOK)){
    switch(HANDED){
      case LEFT: mouseUpdatePerspective(mouseX - xpos, mouseY - y_correct); break;
      case RIGHT: mouseUpdatePerspective(mouseX - xpos, y_correct - mouseY); break;
    }
  }
  // update new state
  mouseX = xpos;
  mouseY = y_correct;
  mouseDownX = mouseDownStartX - xpos;
  mouseDownY = mouseDownStartY - y_correct;
  mouseMoved(mouseX, mouseY);
  // glutPostRedisplay();
}
void mouseButtonHandler (GLFWwindow* window, int button, int action, int mods) {
  // if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) { }
  // if (button == GLUT_LEFT_BUTTON){
  if (button == GLFW_MOUSE_BUTTON_LEFT) {
    if (action == GLFW_PRESS) { // button down
      // mouseX = x;
      // switch (HANDED){
      //   case LEFT: mouseY = y; break;
      //   case RIGHT: mouseY = HEIGHT - y; break;
      // }
      mouseDownStartX = mouseX;
      mouseDownStartY = mouseY;
    }
  }
  if (action == GLFW_PRESS) { mouseDown(button); }
  else { mouseUp(button); } // actually say "RELEASE"
}
///////////////////////////////////////
//////////       INPUT       //////////
///////////////////////////////////////
void moveOriginWithArrowKeys(){
  // process input devices if in first person perspective mode
  // map movement direction to the direction the person is facing
  // float lookAzimuth = lookOrientation[0]/180.0*M_PI;
  float lookAzimuth = (HANDED) ? HORIZON[0]/180.0*M_PI : -HORIZON[0]/180.0*M_PI;
  if(PERSPECTIVE == POLAR){ lookAzimuth += M_PI; }
  float dOrigin[3] = {0.0f, 0.0f, 0.0f};
  if(KEYBOARD[UP_KEY] || KEYBOARD['W'] || KEYBOARD['w']){
    switch(HANDED){
      case LEFT:
        dOrigin[0] -= WALK_INTERVAL * cosf(lookAzimuth);
        dOrigin[1] += WALK_INTERVAL * sinf(lookAzimuth);
        break;
      case RIGHT:
        dOrigin[0] += WALK_INTERVAL * cosf(lookAzimuth);
        dOrigin[1] -= WALK_INTERVAL * sinf(lookAzimuth);
      break;
    }
  }
  if(KEYBOARD[DOWN_KEY] || KEYBOARD['S'] || KEYBOARD['s']){
    switch(HANDED){
      case LEFT:
    dOrigin[0] += WALK_INTERVAL * cosf(lookAzimuth);
    dOrigin[1] -= WALK_INTERVAL * sinf(lookAzimuth);
      break;
      case RIGHT:
    dOrigin[0] -= WALK_INTERVAL * cosf(lookAzimuth);
    dOrigin[1] += WALK_INTERVAL * sinf(lookAzimuth);
      break;
    }
  }
  if(KEYBOARD[LEFT_KEY] || KEYBOARD['A'] || KEYBOARD['a']){
    dOrigin[0] -= WALK_INTERVAL * cosf(lookAzimuth+M_PI_2);
    dOrigin[1] += WALK_INTERVAL * sinf(lookAzimuth+M_PI_2);
  }
  if(KEYBOARD[RIGHT_KEY] || KEYBOARD['D'] || KEYBOARD['d']){
    dOrigin[0] += WALK_INTERVAL * cosf(lookAzimuth+M_PI_2);
    dOrigin[1] -= WALK_INTERVAL * sinf(lookAzimuth+M_PI_2);
  }
  if(KEYBOARD['Q'] || KEYBOARD['q'])
    dOrigin[2] += WALK_INTERVAL;
  if(KEYBOARD['Z'] || KEYBOARD['z'])
    dOrigin[2] -= WALK_INTERVAL;
  switch(HANDED){
    case LEFT:
      ORIGIN[0] -= dOrigin[0];
      ORIGIN[1] -= dOrigin[1];
    break;
    case RIGHT:
      ORIGIN[0] += dOrigin[0];
      ORIGIN[1] += dOrigin[1];
      // ORIGIN[2] += dOrigin[2];
    break;
  }
  ORIGIN[2] += dOrigin[2];
}
void mouseUpdatePerspective(int dx, int dy){
  switch(PERSPECTIVE){
    case FPP:
      HORIZON[0] -= (dx * MOUSE_SENSITIVITY);
      HORIZON[1] += (dy * MOUSE_SENSITIVITY);
      // HORIZON[2] = 0.0;
      firstPersonPerspective();
    break;
    case POLAR:
      HORIZON[0] -= (dx * MOUSE_SENSITIVITY);
      HORIZON[1] -= (dy * MOUSE_SENSITIVITY);
      // HORIZON[2] = 0.0;
      polarPerspective();
      break;
    case ORTHO:
      FRAME[0] += dx / (WIDTH / FRAME[2]);
      // FRAME[1] -= dy / (HEIGHT / FRAME[3]);
      switch(HANDED){
        case LEFT: FRAME[1] += dy / (HEIGHT / FRAME[3]); break;
        case RIGHT: FRAME[1] -= dy / (HEIGHT / FRAME[3]); break;
      }
      orthoPerspective(FRAME[0], FRAME[1], FRAME[2], FRAME[3]);
      break;
  }
}
