#include "sdl.hpp"
#include <iostream>

SdlWindow::SdlWindow(const char* title, int width, int height)
  : m_window(nullptr),
    m_renderer(nullptr),
    m_isRunning(false),
    m_isFullscreen(false),
    m_width(width),
    m_height(height),
    m_glContext(nullptr)
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
  if (glewInit() != GLEW_OK) {
      std::cerr << "Failed to init GLEW" << std::endl;
      return;
  }
  #endif

  // 5. Set vsync (optional)
  SDL_GL_SetSwapInterval(1);

  // 6. Mark as running
  m_isRunning = true;
  m_isRunning = true;
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

  // Swap buffers
  SDL_GL_SwapWindow(m_window);
}

void SdlWindow::change_res(int newWidth, int newHeight)
{
  // Just call SDL_SetWindowSize
  if (m_window) {
    SDL_SetWindowSize(m_window, newWidth, newHeight);
  }
}

void SdlWindow::setFullscreen(bool fullscreen) {
    if (m_window) {
        m_isFullscreen = fullscreen;
        if (m_isFullscreen) {
            SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        } else {
            SDL_SetWindowFullscreen(m_window, 0); // return to windowed
            change_res(m_width, m_height);
        }
    }
}
