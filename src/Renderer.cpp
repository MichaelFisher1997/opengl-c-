#include "Renderer.h"
#include <iostream>



void GLClearError() {
  //while (glGetError() != GL_NO_ERROR);
    
}

bool GLLogCall() {
  Color::Modifier red(Color::FG_RED);
  Color::Modifier def(Color::FG_DEFAULT);
  while (GLenum error = glGetError()) {
    std::cout << red << "[OpenGL Error] (" << error << ")" << def << std::endl; //if error, it will return a number, this needs to be converted to hex to then look up that value inn GL docs
    return false;
  }
  return true;
}

