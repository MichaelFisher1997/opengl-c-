#include "sdl.hpp"
#include "VertexBufferLayout.h"
#include <GL/glew.h> // Include GLEW before <SDL2/SDL.h>?
#include <SDL2/SDL_video.h>
#include <alloca.h>
#include <cstdio>
#include <iostream>
#include <ostream>
#include <string>

#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

SdlWindow::SdlWindow(const char *title, int width, int height)
    : m_window(nullptr), m_renderer(nullptr), m_isRunning(false),
      m_isFullscreen(false), m_width(width), m_height(height),
      m_windowedWidth(width), m_windowedHeight(height), r(0.5f),
      m_glContext(nullptr), increment(0.05f), m_Shader(nullptr), m_Location(-1),
      m_IB(nullptr), m_VB(nullptr), m_VA(nullptr) {

  std::cout << "Step 0: hellow world" << std::endl;

  // 1. Set attributes
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  std::cout << "Step 1: SDL_GL_SetAttribute completed" << std::endl;
  // 2. Create the window with OpenGL flag
  m_window = SDL_CreateWindow(
      title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
      SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  if (!m_window) {
    std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
    return;
  }
  std::cout << "Step 2: SDL_CreateWindow completed" << std::endl;

  m_glContext = SDL_GL_CreateContext(m_window);
  if (!m_glContext) {
    std::cerr << "Failed to create GL context: " << SDL_GetError() << std::endl;
    return;
  }
  std::cout << "Step 3: SDL_GL_CreateContext completed" << std::endl;

  if (SDL_GL_MakeCurrent(m_window, m_glContext) != 0) {
    std::cerr << "Failed to make GL context current: " << SDL_GetError()
              << std::endl;
    return;
  }
  std::cout << "Step 4: SDL_GL_MakeCurrent completed" << std::endl;

#ifndef __APPLE__
  glewExperimental = GL_TRUE;
  GLenum glewErr = glewInit();
  if (glewErr != GLEW_OK) {
    std::cerr << "Failed to init GLEW: " << glewGetErrorString(glewErr)
              << std::endl;
    return;
  }
  std::cout << "Step 5: GLEW initialized successfully" << std::endl;
  glGetError(); // Clear GLEW's initial error
#endif

  std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION)
            << std::endl;
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;

  // 5. Set vsync (optional)
  SDL_GL_SetSwapInterval(1); // Vsync

  // 6. Mark as running
  m_isRunning = true;

  float positions[] = {
      // vertex for a triangle
      // x,y
      -0.5f, -0.5f, // 0
      0.5f,  -0.5f, // 1
      0.5f,  0.5f,  // 2
      -0.5f, 0.5f   // 3
  };

  unsigned int indices[] = {0, 1, 2, 2, 3, 0};
  m_VA = new VertexArray();
  m_VB = new VertexBuffer(positions, 4 * 2 * sizeof(float));
  m_IB = new IndexBuffer(indices, 6);

  VertexBufferLayout layout;
  layout.Push<float>(2);

  m_VA->AddBuffer(*m_VB, layout);

  m_Shader = new Shader("res/shaders/Basic.shader");
  m_Shader->Bind();
}
SdlWindow::~SdlWindow() {
  // If using SDL Renderer, destroy it. But if you’re purely using OpenGL, you
  // might remove it.
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
  ////TODO
  // some point check everything is being deleted
  SDL_Quit();
}
//
//
void SdlWindow::run() {
  while (m_isRunning) {
    processEvents();
    update();
    render();
  }
}
//
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
        m_isRunning = false; // exit application
      }
    } else if (event.type == SDL_WINDOWEVENT) {
      if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
        // SDL gives the new width/height in event.window.data1/data2
        int newWidth = event.window.data1;
        int newHeight = event.window.data2;

        // Update your internal width/height (if you keep track)
        m_width = newWidth;
        m_height = newHeight;
        // If we are in windowed mode, update the "windowed" size.
        if (!m_isFullscreen) {
          m_windowedWidth = newWidth;
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
//
void SdlWindow::update() {
  // Update game/application logic here
  if (r > 1.0f) {
    increment = -0.05f;
  } else if (r < 0.0f) {
    increment = 0.05f;
  }
  r += increment;
}
//
void SdlWindow::render() {
  // Use GL calls instead of SDL’s renderer
  GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f)); // background
  //
  GLCall(glClear(GL_COLOR_BUFFER_BIT));

  m_Shader->Bind();
  m_Shader->SetUniform4f("u_Color", r, 0.3, 0.8, 1.0);

  m_VA->Bind();
  m_IB->Bind();
  // TODO: Draw with OpenGL here (shaders, triangles, etc.)
  // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
  GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,
                        nullptr)); // macro assert for debugging

  // Swap buffers
  SDL_GL_SwapWindow(m_window);
}
//
void SdlWindow::setFullscreen(bool fullscreen) {
  if (m_window) {
    m_isFullscreen = fullscreen;
    if (m_isFullscreen) {
      // Going TO fullscreen:
      // (Optional) store the current size again, in case it changed
      m_windowedWidth = m_width;
      m_windowedHeight = m_height;

      SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    } else {
      // Returning FROM fullscreen:
      SDL_SetWindowFullscreen(m_window, 0); // return to windowed
      // Now restore the window’s old size
      SDL_SetWindowSize(m_window, m_windowedWidth, m_windowedHeight);

      // Update m_width, m_height so they reflect the new (restored) size
      m_width = m_windowedWidth;
      m_height = m_windowedHeight;
    }
  }
}
