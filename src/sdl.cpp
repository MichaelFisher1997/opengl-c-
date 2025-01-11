#include <GL/glew.h> // Include GLEW before <SDL2/SDL.h>?
#include "sdl.hpp"
#include "VertexBufferLayout.h"
#include "colormod.h" 
#include <SDL2/SDL_video.h>
#include <alloca.h>
#include <cstdio>
#include <iostream>
#include <string>
#include <fstream>
//#include <iterator>
#include <ostream>
#include <string>
#include <sstream>

#if defined(_MSC_VER)  // Microsoft Visual C++
    #include <intrin.h>
    #define DEBUG_BREAK() __debugbreak()
#elif defined(__i386__) || defined(__x86_64__)
    // Use inline assembly for x86/x86_64
    #define DEBUG_BREAK() __asm__ volatile("int3")
#else
    // Fallback on non-x86 platforms
    #include <signal.h>
    #define DEBUG_BREAK() raise(SIGTRAP)
#endif

// ASSERT macro that shows file, line, and the failed expression
#define ASSERT(x)                                                      \
    do {                                                               \
        if (!(x)) {                                                    \
            std::cerr << "Assertion Failed: " << #x << '\n'           \
                      << "File: " << __FILE__ << '\n'                  \
                      << "Line: " << __LINE__ << std::endl;            \
            DEBUG_BREAK();                                             \
        }                                                              \
    } while (false)

#define GLCall(x) GLClearError();\
  x;\
  ASSERT(GLLogCall())


struct ShaderProgramSource {
  std::string VertexSource, FragmentSource;
};

SdlWindow::SdlWindow(const char* title, int width, int height)
  : m_window(nullptr),
    m_renderer(nullptr),
    m_isRunning(false),
    m_isFullscreen(false),
    m_width(width),
    m_height(height),
    m_glContext(nullptr),
    m_windowedWidth(width),
    m_windowedHeight(height),
    r(0.5f),
    location(),
    increment(0.05f),
    ib(nullptr, 0)
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
  SDL_GL_SetSwapInterval(1); //Vsync
  SdlWindow::GLClearError();

  // 6. Mark as running
  m_isRunning = true;

  float positions[] = { // vertex for a triangle
    //x,y
   -0.5f, -0.5f, // 0 
    0.5f, -0.5f, // 1
    0.5f,  0.5f, //2
    -0.5f,  0.5f // 3
  }; 

  unsigned int indices[] = {
      0, 1, 2,
      2, 3, 0
  };

  unsigned int vao; //vertext array object
  GLCall(glGenVertexArrays(1, &vao));
  GLCall(glBindVertexArray(vao));

  VertexArray va;
  VertexBuffer vb(positions, 4 * 2 * sizeof(float));
  VertexBufferLayout layout;
  layout.Push<float>(2);
  va.AddBuffer(vb, layout);
  
  IndexBuffer ib(indices, 6); 

  ShaderProgramSource source = parseShader("res/shaders/Basic.shader");
  unsigned int m_ShaderID = createShader(source.VertexSource, source.FragmentSource);
  GLCall(glUseProgram(m_ShaderID));
  
  GLCall(int location = glGetUniformLocation(m_ShaderID, "u_Color"));
  ASSERT(location != -1); // -1 is an error
  GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));

  GLCall(glBindVertexArray(0));
  GLCall(glUseProgram(0));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));


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
  if (shader) {
        glDeleteProgram(shader);
        shader = 0;
    }

  delete &ib;
  SDL_Quit();
}


void SdlWindow::run() {
  while (m_isRunning) {
    processEvents(); 
    update();
    render();
  }
  GLCall(glDeleteProgram(shader));
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
        GLCall(glViewport(0, 0, newWidth, newHeight));

        // (Optional) If you have a projection matrix, update it here as well
        // e.g., recalc the aspect ratio for a perspective projection
      }
    }
  }
}

void SdlWindow::update() {
  // Update game/application logic here
  if (r > 1.0f) {
    increment = -0.05f;
  } else if (r < 0.0f) {
    increment = 0.05f;
  }
  r += increment;
}

void SdlWindow::render() {
  // Use GL calls instead of SDL’s renderer
  GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f)); //background
  //
  GLCall(glClear(GL_COLOR_BUFFER_BIT));
  
  GLCall(glUseProgram(shader));
  GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

  va.Bind();
  ib.Bind();
  // TODO: Draw with OpenGL here (shaders, triangles, etc.)
  //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
  GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); //macro assert for debugging
  
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
  GLCall(unsigned int id = glCreateShader(type));
  const char* src = source.c_str(); // <--- this string needs to exist when compiling/running
  GLCall(glShaderSource(id, 1, &src, nullptr));
  GLCall(glCompileShader(id));
  //TODO: error handling
  int result;
  GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
  if (result == GL_FALSE) {
    int length;
    GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
    char* message = (char*)alloca(length * sizeof(char)); //do i need to deallocate this??
    GLCall(glGetShaderInfoLog(id, length, &length, message));
    std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex":"fragment") << " shader" << std::endl; // print out what type of shader it is
    std::cout << message << std::endl;
    GLCall(glDeleteShader(id));
    return 0;

  }
  //
  return id;

  
}

unsigned int SdlWindow::createShader(const std::string& vertexShader, const std::string& fragmentShader) {
  GLCall(unsigned int program = glCreateProgram());
  unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
  GLCall(glAttachShader(program, vs));
  GLCall(glAttachShader(program, fs));

  GLCall(glLinkProgram(program));
  GLCall(glValidateProgram(program));

  GLCall(glDeleteShader(vs));
  GLCall(glDeleteShader(fs));
  return program;

}
SdlWindow::ShaderProgramSource SdlWindow::parseShader(const std::string& filepath) {
  //can be changed to c standard which is a bit faster
  std::ifstream stream(filepath);
  if (!stream.is_open()) {
      std::cerr << "parseShader ERROR: Could not open file " << filepath << std::endl;
      // Return empty (or handle however you prefer)
      return {"", ""};
  }

  enum class ShaderType {
    NONE = -1, VERTEX = 0, FRAGMENT = 1
  };
  std::string line;
  std::stringstream ss[2]; //vertex - fragment
  ShaderType type = ShaderType::NONE;

  while (getline(stream, line)) {
    if (line.find("#shader") != std::string::npos) {
      if (line.find("vertex") != std::string::npos) {
        type = ShaderType::VERTEX;
        
      }
      else if (line.find("fragment") != std::string::npos) {
        type = ShaderType::FRAGMENT;

      }
    }
    else {
      ss[(int)type] << line << '\n';
    }
  }
  return {ss[0].str(), ss[1].str() };
  
}

void SdlWindow::GLClearError() {
  while (glGetError() != GL_NO_ERROR);
    
}

bool SdlWindow::GLLogCall() {
  Color::Modifier red(Color::FG_RED);
  Color::Modifier def(Color::FG_DEFAULT);
  while (GLenum error = glGetError()) {
    std::cout << red << "[OpenGL Error] (" << error << ")" << def << std::endl; //if error, it will return a number, this needs to be converted to hex to then look up that value inn GL docs
    return false;
  }
  return true;
}

