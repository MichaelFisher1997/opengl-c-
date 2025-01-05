#pragma once

#include <SDL2/SDL.h>

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

    void change_res(int newWidth, int newHeight);
    void setFullscreen(bool fullscreen);

private:
    // Private data members
    SDL_Window*   m_window;
    SDL_Renderer* m_renderer;
    bool          m_isRunning;
    bool          m_isFullscreen; 
    int           m_width;
    int           m_height;

    // Private methods
    void processEvents();
    void update();
    void render();
};

