#include "sdl.hpp"

int main(int argc, char* argv[]) {
    // Create an instance of our SdlWindow class
  SdlWindow window("My SDL2 Window", 800, 600);

  // Run the main loop
  window.run();

  return 0;
}

