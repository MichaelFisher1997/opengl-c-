#pragma once

#include <SDL2/SDL.h>
#include <string>
// Include the OpenGL headers (these may vary by system)
#if defined(__APPLE__)
#include <OpenGL/gl3.h> // On macOS, <OpenGL/gl3.h> is often used
#else
#include <GL/glew.h> // or <glad/glad.h> if using GLAD
#endif

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include <string>

// Forward declaration of classes and structs if needed
// class SomethingElse;

class SdlWindow {
public:
  // Constructor
  SdlWindow(const char *title, int width, int height);

  // Destructor
  ~SdlWindow();

  // Run the main loop
  void run();
  // unsigned int compileShader(unsigned int type, const std::string& source);
  // unsigned int createShader(const std::string& vetexShader, const
  // std::string& fragmentShader);
  // // for spliting our shaders
  // //struct ShaderProgramSource {
  // //  std::string VetexSource, FragmentSource;
  // struct ShaderProgramSource {
  //   std::string VertexSource;
  //   std::string FragmentSource;
  // };
  // ShaderProgramSource parseShader(const std::string& filepath);

  void setFullscreen(bool fullscreen);

private:
  // Private data members
  SDL_Window *m_window;
  SDL_Renderer *m_renderer;
  bool m_isRunning;
  bool m_isFullscreen;
  int m_width;
  int m_height;
  int m_windowedWidth;  // stored width before fullscreen
  int m_windowedHeight; // stored height before fullscreen
  float r;
  SDL_GLContext m_glContext;
  float increment;
  //  // temp shader stuff
  GLint m_Location;
  VertexArray *m_VA;
  IndexBuffer *m_IB;
  VertexBuffer *m_VB;
  Shader *m_Shader;

  // Private methods
  void processEvents();
  void update();
  void render();
};
