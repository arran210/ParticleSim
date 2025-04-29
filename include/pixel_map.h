#pragma once
#include "pixel_types.h"
#include "map_cursor.h"

// Pre-declare Pixel
class Pixel;

class PixelMap
{
private:
  // 2D array of Pixel pointers
  Pixel*** m_map;
  // Width and height of the pixel map
  float m_x;
  float m_y;
  int m_width;
  int m_height;

  MapCursor m_map_cursor;

public:
// Constructor
  PixelMap(float x, float y, int width, int height);

// Destructor
  ~PixelMap();

// Functions
  void Update();
  void Step();
  void UpdateStrip(int x1, int x2);
  void Draw();

// Setters
  // Set a pixel at (x, y) to the given pixel
  void SetPixel(int x, int y, PixelType pixel_type);
  void SetPixelUnsafe(int x, int y, PixelType pixel_type);
  // Set a line of pixels from (x, y) to (x + dx, y + dy)
  void SetPixelLine(int x, int y, int dx, int dy, PixelType pixel_type);
  void SetPixelLineUnsafe(int x, int y, int dx, int dy, PixelType pixel_type);
  // Set a rectangle of pixels from (x1, y1) to (x2, y2)
  void SetPixelRect(int x1, int y1, int x2, int y2, PixelType pixel_type);
  void SetPixelRectUnsafe(int x1, int y1, int x2, int y2, PixelType pixel_type);
  // Set a rectangle of pixels in a line
  void SetPixelRectLine(int x1, int y1, int x2, int y2, int dx, int dy, PixelType pixel_type);
  void SetPixelRectLineUnsafe(int x1, int y1, int x2, int y2, int dx, int dy, PixelType pixel_type);
  // Move a pixel from (x1, y1) to (x2, y2)
  void MovePixel(int x1, int y1, int x2, int y2);
  void MovePixelUnsafe(int x1, int y1, int x2, int y2);
  // Swap two pixels at (x1, y1) and (x2, y2)
  void SwapPixels(int x1, int y1, int x2, int y2);
  void SwapPixelsUnsafe(int x1, int y1, int x2, int y2);
  
// Getters
  Pixel* GetPixel(int x, int y);
  Pixel* GetPixelUnsafe(int x, int y);
  float GetX() const;
  float GetY() const;
  int GetWidth() const;
  int GetHeight() const;
};