#include "sdl.hpp"

int main(int argc, char *argv[]) {
  // Create an instance of our SdlWindow class
  std::cout << "starting" << std::endl;
  SdlWindow window("My SDL2 Window", 800, 600);
  std::cout << "About to run: " << std::endl;

  // Run the main loop
  window.run();

  return 0;
}
