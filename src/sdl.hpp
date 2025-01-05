#pragma once

#include <SDL2/SDL.h>
// Include the OpenGL headers (these may vary by system)
#if defined(__APPLE__)
  #include <OpenGL/gl3.h>  // On macOS, <OpenGL/gl3.h> is often used
#else
  #include <GL/glew.h>     // or <glad/glad.h> if using GLAD
#endif

#include <iostream>

// Forward declaration of classes and structs if needed
// class SomethingElse;

class SdlWindow {
public:
    // Constructor
    SdlWindow(const char* title, int width, int height);

    // Destructor
    ~SdlWindow();

    // Run the main loop
    void run();

    void setFullscreen(bool fullscreen);

private:
    // Private data members
    SDL_Window*   m_window;
    SDL_Renderer* m_renderer;
    bool          m_isRunning;
    bool          m_isFullscreen; 
    int           m_width;
    int           m_height;
    int           m_windowedWidth;   // stored width before fullscreen
    int           m_windowedHeight;  // stored height before fullscreen
    SDL_GLContext  m_glContext;
    

    // Private methods
    void processEvents();
    void update();
    void render();
};

