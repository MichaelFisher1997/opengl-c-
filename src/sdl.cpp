#include <GL/glew.h> // Include GLEW before <SDL2/SDL.h>?
#include "sdl.hpp"
#include <alloca.h>
#include <iostream>
//#include <iterator>
#include <ostream>
#include <string>

SdlWindow::SdlWindow(const char* title, int width, int height)
  : m_window(nullptr),
    m_renderer(nullptr),
    m_isRunning(false),
    m_isFullscreen(false),
    m_width(width),
    m_height(height),
    m_glContext(nullptr),
    m_windowedWidth(width),
    m_windowedHeight(height)
{
  // 1. Set attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  // 2. Create the window with OpenGL flag
    m_window = SDL_CreateWindow(title,
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      width,
      height,
      SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN| SDL_WINDOW_RESIZABLE);

  if (!m_window) {
    std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
    return;
  }

  // 3. Create OpenGL context
  m_glContext = SDL_GL_CreateContext(m_window);
  if (!m_glContext) {
      std::cerr << "Failed to create GL context: " << SDL_GetError() << std::endl;
      return;
  }
  // 4. Optionally init GLEW (if not on macOS core profile)
  #ifndef __APPLE__
  glewExperimental = GL_TRUE;
  GLenum glewErr = glewInit();
  if (glewInit() != GLEW_OK) {
      std::cerr << "Failed to init GLEW" << glewGetErrorString(glewErr) << std::endl;
      return;
  }
  glGetError();
  #endif

  // 5. Set vsync (optional)
  SDL_GL_SetSwapInterval(1);

  // 6. Mark as running
  m_isRunning = true;

  float positions[6] = { // vertex for a triangle
    //x,y
   -0.5f, -0.5f, 
    0.0f, 0.5f,
    0.5f, -0.5f
  }; 
  //vertex buffer
  unsigned int buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer); //select buffer called 'buffer'
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW); // assigne buffer size, static as we use many times, but does not change
  //vertext attributes / layout
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

  std::string vertexShader = 
    "#version 330 core\n"
    "\n"
    "layout(location = 0) in vec4 position;\n"
    "\n"
    "void main()\n"
    "{\n"
    " gl_Position = position;\n"
    "}\n";

  std::string fragmentShader = 
    "#version 330 core\n"
    "\n"
    "layout(location = 0) out vec4 color;\n"
    "\n"
    "void main()\n"
    "{\n"
    " color = vec4(1.0, 0.0, 0.0, 1.0);\n"
    "}\n";
  unsigned int shader = createShader(vertexShader, fragmentShader);
  glUseProgram(shader);

}

SdlWindow::~SdlWindow() {
  // If using SDL Renderer, destroy it. But if you’re purely using OpenGL, you might remove it.
  if (m_renderer) {
      SDL_DestroyRenderer(m_renderer);
      m_renderer = nullptr;
  }

  // Delete the OpenGL context
  if (m_glContext) {
      SDL_GL_DeleteContext(m_glContext);
      m_glContext = nullptr;
  }

  if (m_window) {
      SDL_DestroyWindow(m_window);
      m_window = nullptr;
  }

  SDL_Quit();
}


void SdlWindow::run() {
  while (m_isRunning) {
    processEvents(); 
    update();
    render();
  }
}

void SdlWindow::processEvents() {
SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      m_isRunning = false;
    }
    // Handle other events (keyboard, mouse, etc.) here if needed
    else if (event.type == SDL_KEYDOWN) {
      // Check if the key pressed is Enter/Return
      if (event.key.keysym.sym == SDLK_RETURN) {
        std::cout << "input detected" << std::endl;
      }
      if (event.key.keysym.sym == SDLK_SPACE) {
        std::cout << "Full screen togled!" << std::endl;
        setFullscreen(!m_isFullscreen);
      }
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        std::cout << "Bye!" << std::endl;
        m_isRunning = false; //exit application
      }
    }
    else if (event.type == SDL_WINDOWEVENT) {
      if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
        // SDL gives the new width/height in event.window.data1/data2
        int newWidth  = event.window.data1;
        int newHeight = event.window.data2;

        // Update your internal width/height (if you keep track)
        m_width  = newWidth;
        m_height = newHeight;
        // If we are in windowed mode, update the "windowed" size.
        if (!m_isFullscreen) {
            m_windowedWidth  = newWidth;
            m_windowedHeight = newHeight;
        }

        // Update the OpenGL viewport
        glViewport(0, 0, newWidth, newHeight);

        // (Optional) If you have a projection matrix, update it here as well
        // e.g., recalc the aspect ratio for a perspective projection
      }
    }
  }
}

void SdlWindow::update() {
  // Update game/application logic here
}

void SdlWindow::render() {
  // Use GL calls instead of SDL’s renderer
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  // TODO: Draw with OpenGL here (shaders, triangles, etc.)
  glDrawArrays(GL_TRIANGLES, 0, 3);

  // Swap buffers
  SDL_GL_SwapWindow(m_window);
}

void SdlWindow::setFullscreen(bool fullscreen) {
    if (m_window) {
        m_isFullscreen = fullscreen;
        if (m_isFullscreen) {
            // Going TO fullscreen:
            // (Optional) store the current size again, in case it changed
            m_windowedWidth  = m_width;
            m_windowedHeight = m_height;

            SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        } else {
            // Returning FROM fullscreen:
            SDL_SetWindowFullscreen(m_window, 0); // return to windowed
            // Now restore the window’s old size
            SDL_SetWindowSize(m_window, m_windowedWidth, m_windowedHeight);

            // Update m_width, m_height so they reflect the new (restored) size
            m_width  = m_windowedWidth;
            m_height = m_windowedHeight;
        }
    }
}

unsigned int SdlWindow::compileShader(unsigned int type, const std::string& source) {
  unsigned int id = glCreateShader(type);
  const char* src = source.c_str(); // <--- this string needs to exist when compiling/running
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);
  //TODO: error handling
  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char* message = (char*)alloca(length * sizeof(char)); //do i need to deallocate this??
    glGetShaderInfoLog(id, length, &length, message);
    std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex":"fragment") << "shader" << std::endl; // print out what type of shader it is
    std::cout << message << std::endl;
    glDeleteShader(id);
    return 0;

  }
  //
  return id;

  
}

unsigned int SdlWindow::createShader(const std::string& vertexShader, const std::string& fragmentShader) {
  unsigned int program = glCreateProgram();
  unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;

}
