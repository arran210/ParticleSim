#include "config.h"
#include <string>

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

int main()
{
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "window");
  SetTargetFPS(60);
  DisableCursor();

  PixelMap pixel_map(0, 80, SCREEN_WIDTH / PIXEL_SIZE, (SCREEN_HEIGHT - 80) / PIXEL_SIZE);

  while (!WindowShouldClose())
  {
    std::string title = "window | FPS: " + std::to_string(static_cast<int>(1 / GetFrameTime()));
    SetWindowTitle(title.c_str());
    BeginDrawing();
    ClearBackground(Color{0x22, 0x22, 0x22, 0xff});
    
    pixel_map.Update();
    pixel_map.Draw();

    EndDrawing();
  }

  return 0;
}