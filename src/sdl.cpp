#include "sdl.hpp"
#include <iostream>

SdlWindow::SdlWindow(const char* title, int width, int height)
  : m_window(nullptr),
    m_renderer(nullptr),
    m_isRunning(false),
    m_isFullscreen(false),
    m_width(width),
    m_height(height),
    m_glContect(nullptr)
{
  // 1. Set attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  // 2. Create the window
  m_window = SDL_CreateWindow(
    title,
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    width,
    height,
    SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
  );
  if (!m_window) {
    std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
    return;
  }

  // 3. Create the renderer
  m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
  if (!m_renderer) {
    std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
    return;
  }

  // 4. If everything succeeded, mark the application as running
  m_isRunning = true;
}

SdlWindow::~SdlWindow() {
  // Cleanup
  if (m_renderer) {
    SDL_DestroyRenderer(m_renderer);
    m_renderer = nullptr;
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
  }
}

void SdlWindow::update() {
  // Update game/application logic here
}

void SdlWindow::render() {
  // Set background color to red
  SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
  SDL_RenderClear(m_renderer);

  // You can draw shapes, textures, etc. here

  SDL_RenderPresent(m_renderer);
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
