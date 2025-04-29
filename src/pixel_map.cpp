#include "pixel_map.h"
#include "pixel_types.h"
#include <cmath>
#include <thread>
#include <vector>

// Constructor
PixelMap::PixelMap(float x, float y, int width, int height)
  : m_x(x), m_y(y), m_width(width), m_height(height)
{
  m_map = new Pixel * *[width];
  for (int i = 0; i < width; ++i)
  {
    m_map[i] = new Pixel * [height];
    for (int j = 0; j < height; ++j)
    {
      m_map[i][j] = nullptr;
    }
  }
}

// Destructor
PixelMap::~PixelMap()
{
  for (int i = 0; i < m_width; ++i)
  {
    for (int j = 0; j < m_height; ++j)
    {
      delete m_map[i][j];
    }
    delete[] m_map[i];
  }
  delete[] m_map;

}

void PixelMap::Update()
{
  // Update the map cursor
  m_map_cursor.Update(this);

  Step();
}
void PixelMap::Step()
{
  // Create a thread pool
  int num_threads = std::thread::hardware_concurrency();
  std::vector<std::thread> threads;
  int strip_width = m_width / num_threads;
  int offset = rand() % strip_width;
  
  threads.push_back(std::thread(&PixelMap::UpdateStrip, this, 0, offset));
  for (int i = 1; i < num_threads + 1; ++i)
  {
    int x1 = offset + (i - 1) * strip_width;
    int x2 = offset + i * strip_width;
    if (x2 > m_width)
    {
      x2 = m_width;
    }
    threads.push_back(std::thread(&PixelMap::UpdateStrip, this, x1, x2));
    if (x2 == m_width)
    {
      break;
    }
  }

  // Wait for all threads to finish
  for (auto& thread : threads)
  {
    thread.join();
  }
}
void PixelMap::UpdateStrip(int x1, int x2)
{
  for (int y = 0; y < m_height; y++)
  {
    if (y % 2 == 0)
    {
      for (int x = x1; x < x2; x++)
      {
        if (m_map[x][y] != nullptr)
        {
          m_map[x][y]->Update(this);
        }
      }
    }
    else
    {
      for (int x = x2 - 1; x >= x1; x--)
      {
        if (m_map[x][y] != nullptr)
        {
          m_map[x][y]->Update(this);
        }
      }
    }
  }
}
void PixelMap::Draw()
{
  for (int x = 0; x < m_width; x++)
  {
    for (int y = 0; y < m_height; y++)
    {
      if (m_map[x][y] != nullptr)
      {
        m_map[x][y]->Draw(this);
      }
    }
  }

  // Draw the map cursor
  m_map_cursor.Draw(this);
}


// Getters
Pixel* PixelMap::GetPixel(int x, int y)
{
  if (x < 0 || x >= m_width || y < 0 || y >= m_height)
  {
    return nullptr;
  }
  return m_map[x][y];
}
Pixel* PixelMap::GetPixelUnsafe(int x, int y)
{
  return m_map[x][y];
}
float PixelMap::GetX() const
{
  return m_x;
}
float PixelMap::GetY() const
{
  return m_y;
}
int PixelMap::GetWidth() const
{
  return m_width;
}
int PixelMap::GetHeight() const
{
  return m_height;
}


// Setters
void PixelMap::SetPixel(int x, int y, PixelType pixel_type)
{
  // check if coordinates are within bounds
  if (x < 0 || x >= m_width || y < 0 || y >= m_height) { return; }

  SetPixelUnsafe(x, y, pixel_type);
}
void PixelMap::SetPixelUnsafe(int x, int y, PixelType pixel_type)
{
  if (m_map[x][y] != nullptr)
  {
    delete m_map[x][y];
  }

  switch (pixel_type)
  {
  case PIXEL_TYPE_SAND:
    m_map[x][y] = new Sand(x, y);
    break;
  case PIXEL_TYPE_WATER:
    m_map[x][y] = new Water(x, y);
    break;
  case PIXEL_TYPE_SMOKE:
    m_map[x][y] = new Smoke(x, y);
    break;
  case PIXEL_TYPE_STONE:
    m_map[x][y] = new Stone(x, y);
    break;
  case PIXEL_TYPE_FIRE:
    m_map[x][y] = new Fire(x, y);
    break;
  case PIXEL_TYPE_WOOD:
    m_map[x][y] = new Wood(x, y);
    break;
  case PIXEL_TYPE_METHANE:
    m_map[x][y] = new Methane(x, y);
    break;
  default:
    m_map[x][y] = nullptr;
    break;
  }
}
void PixelMap::SetPixelLine(int x, int y, int dx, int dy, PixelType pixel_type)
{
  // check if coordinates are within bounds
  if (x < 0 || x >= m_width || y < 0 || y >= m_height ||
      x + dx < 0 || x + dx >= m_width || y + dy < 0 || y + dy >= m_height) 
  { return; }
  
  if (dx == 0 && dy == 0)
  {
    SetPixelUnsafe(x, y, pixel_type);
  }

  SetPixelLineUnsafe(x, y, dx, dy, pixel_type);
}
void PixelMap::SetPixelLineUnsafe(int x, int y, int dx, int dy, PixelType pixel_type)
{
  int steps = std::max(std::abs(dx), std::abs(dy)); // Determine the number of steps
  double x_increment = static_cast<double>(dx) / steps;
  double y_increment = static_cast<double>(dy) / steps;

  double x_coord = x;
  double y_coord = y;

  for (int i = 0; i <= steps; ++i) {
    SetPixelUnsafe(static_cast<int>(round(x_coord)), static_cast<int>(round(y_coord)), pixel_type);
    x_coord += x_increment;
    y_coord += y_increment;
  }
}
void PixelMap::SetPixelRect(int x1, int y1, int x2, int y2, PixelType pixel_type)
{
  int lx = std::min(std::max(std::min(x1, x2), 0), m_width - 1);
  int hx = std::min(std::max(std::max(x1, x2), 0), m_width - 1);
  int ly = std::min(std::max(std::min(y1, y2), 0), m_height - 1);
  int hy = std::min(std::max(std::max(y1, y2), 0), m_height - 1);
  
  SetPixelRectUnsafe(lx, ly, hx, hy, pixel_type);
}
void PixelMap::SetPixelRectUnsafe(int x1, int y1, int x2, int y2, PixelType pixel_type)
{
  for (int x = x1; x <= x2; x++)
  {
    for (int y = y1; y <= y2; y++)
    {
      SetPixelUnsafe(x, y, pixel_type);
    }
  }
}
void PixelMap::SetPixelRectLine(int x1, int y1, int x2, int y2, int dx, int dy, PixelType pixel_type)
{
  if (x1 < 0 || x1 >= m_width || y1 < 0 || y1 >= m_height ||
      x2 < 0 || x2 >= m_width || y2 < 0 || y2 >= m_height ||
      x1 + dx < 0 || x1 + dx >= m_width || y1 + dy < 0 || y1 + dy >= m_height ||
      x2 + dx < 0 || x2 + dx >= m_width || y2 + dy < 0 || y2 + dy >= m_height
    )
  {
    int new_x1 = std::min(std::max(std::min(x1, x2), 0), m_width - 1);
    int new_x2 = std::min(std::max(std::max(x1, x2), 0), m_width - 1);
    int new_y1 = std::min(std::max(std::min(y1, y2), 0), m_height - 1);
    int new_y2 = std::min(std::max(std::max(y1, y2), 0), m_height - 1);

    SetPixelRectUnsafe(new_x1, new_y1, new_x2, new_y2, pixel_type);
    return;
  }

  SetPixelRectLineUnsafe(x1, y1, x2, y2, dx, dy, pixel_type);
}
void PixelMap::SetPixelRectLineUnsafe(int x1, int y1, int x2, int y2, int dx, int dy, PixelType pixel_type)
{
  int steps = std::max(std::abs(dx), std::abs(dy)); // Determine the number of steps
  double x_increment = static_cast<double>(dx) / steps;
  double y_increment = static_cast<double>(dy) / steps;

  double x1_coord = x1;
  double y1_coord = y1;
  double x2_coord = x2;
  double y2_coord = y2;

  for (int i = 0; i <= steps; ++i) {
    SetPixelRectUnsafe(static_cast<int>(round(x1_coord)), static_cast<int>(round(y1_coord)), static_cast<int>(round(x2_coord)), static_cast<int>(round(y2_coord)), pixel_type);
    x1_coord += x_increment;
    y1_coord += y_increment;
    x2_coord += x_increment;
    y2_coord += y_increment;
  }
}

void PixelMap::MovePixel(int x1, int y1, int x2, int y2)
{
  // check if coordinates are within bounds
  if (x1 < 0 || x1 >= m_width || y1 < 0 || y1 >= m_height ||
      x2 < 0 || x2 >= m_width || y2 < 0 || y2 >= m_height) 
  { return; }

  MovePixelUnsafe(x1, y1, x2, y2);
}
void PixelMap::MovePixelUnsafe(int x1, int y1, int x2, int y2)
{
  // check if pixel exists at source coordinates
  if (m_map[x2][y2] != nullptr)
  {
    delete m_map[x2][y2];
  }
  // move pixel
  m_map[x2][y2] = m_map[x1][y1];
  m_map[x1][y1] = nullptr;
  // update coordinates
  if (m_map[x2][y2] != nullptr)
  {
    m_map[x2][y2]->SetCoords(x2, y2);
  }
}

void PixelMap::SwapPixels(int x1, int y1, int x2, int y2)
{
  // check if coordinates are within bounds
  if (x1 < 0 || x1 >= m_width || y1 < 0 || y1 >= m_height ||
      x2 < 0 || x2 >= m_width || y2 < 0 || y2 >= m_height)
  { return; }

  SwapPixelsUnsafe(x1, y1, x2, y2);
}
void PixelMap::SwapPixelsUnsafe(int x1, int y1, int x2, int y2)
{
  // swap pixels
  Pixel* temp = m_map[x1][y1];
  m_map[x1][y1] = m_map[x2][y2];
  m_map[x2][y2] = temp;
  // update coordinates
  if (m_map[x1][y1] != nullptr)
  {
    m_map[x1][y1]->SetCoords(x1, y1);
  }
  if (m_map[x2][y2] != nullptr)
  {
    m_map[x2][y2]->SetCoords(x2, y2);
  }
}