unsigned char* getTextureData(const char * filename, int width, int height){
  FILE * file;
  file = fopen(filename, "rb");
  if (file == NULL) return 0;
  unsigned char *data = (unsigned char *)malloc(width * height * 3);
  // unsigned char *data = (unsigned char *)malloc(width * height * 4);
  fread(data, width * height * 3, 1, file);
  // fread(data, width * height * 4, 1, file);
  fclose(file);
  for(int i = 0; i < width * height; i++){
    int index = i*3;
    unsigned char B,R;
    B = data[index];
    R = data[index+2];
    data[index] = R;
    data[index+2] = B;
  }
  return data;
}
GLuint loadTextureBGR(const char * filename, int width, int height){
  GLuint texture;
  unsigned char *data = getTextureData(filename, width, height);
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_BGR, GL_UNSIGNED_BYTE, data);
  free(data);
  glBindTexture(GL_TEXTURE_2D, 0);
  return texture;
}
GLuint loadTexture(const char * filename, int width, int height){
  GLuint texture;
  unsigned char *data = getTextureData(filename, width, height);
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  // glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
  // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
  // gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
  free(data);
  glBindTexture(GL_TEXTURE_2D, 0);
  return texture;
}
GLuint loadTextureSmooth(const char * filename, int width, int height){
  GLuint texture;
  unsigned char * data = getTextureData(filename, width, height);
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
  free(data);
  glBindTexture(GL_TEXTURE_2D, 0);
  return texture;
}