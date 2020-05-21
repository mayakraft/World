static void error_callback (int error, const char* description) {
  fprintf(stderr, "%s\n", description);
}

int main (int argc, char **argv) {
  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) { exit(EXIT_FAILURE); }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  GLFWmonitor *monitor = FULLSCREEN ? glfwGetPrimaryMonitor() : NULL;
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, APP_TITLE, monitor, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  // attach handlers
  glfwSetKeyCallback(window, keyboardHandler);
  glfwSetCursorPosCallback(window, mousePositionHandler);
  glfwSetMouseButtonCallback(window, mouseButtonHandler);
  glfwSetWindowSizeCallback(window, windowSizeHandler);
  // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  // Init OpenGL
  GLenum err = glewInit();
  // glewExperimental = GL_TRUE;
  if(err != GLEW_OK) {
    printf("%s", glewGetErrorString(err));
    exit(EXIT_FAILURE);
  }
  // setup this program
  FRAME[0] = 0.02 * -WIDTH*0.5;
  FRAME[1] = 0.02 * -HEIGHT*0.5;
  FRAME[2] = 0.02 * WIDTH;
  FRAME[3] = 0.02 * HEIGHT;
  ASPECT = FRAME[2] / FRAME[3];
  memset(KEYBOARD, 0, 348);
  clock_gettime(CLOCK_MONOTONIC, &START);
  initPrimitives();
  time_t t;
  srand((unsigned) time(&t));
  typicalOpenGLSettings();
  // glutPostRedisplay();
  glfwGetFramebufferSize(window, &WIDTH, &HEIGHT);
  reshapeWindow(WIDTH, HEIGHT);
  WINDOW = window;
  MONITOR = glfwGetPrimaryMonitor();
  setup();  // user defined function
  // begin main loop
  while (!glfwWindowShouldClose(window)) {
    UPDATE();
    DRAW(window);
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
