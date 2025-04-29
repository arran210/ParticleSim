#include <raylib.h>
#include <random>
#include "pixel_map.h"
#include "pixel.h"

// Constructor
Pixel::Pixel(int x, int y, Color color)
  : m_x_coord(x), m_y_coord(y), m_color(color)
{}

void Pixel::Update(PixelMap* map) 
{
  if (m_updated_this_frame) { return; }
  m_updated_this_frame = true;
  Step(map);
  if (m_on_fire)
  {
    FireStep(map);
  }
}

void Pixel::FireStep(PixelMap* map)
{
  Darken();
  int surrounding_fire_count = 0;
  int surrounding_pixel_count = 0;
  for (int i = -1; i <= 1; i++)
  {
    for (int j = -1; j <= 1; j++)
    {
      if (i == 0 && j == 0) { continue; }
      Pixel* pixel = map->GetPixel(m_x_coord + i, m_y_coord + j);
      if (pixel == nullptr) 
      {
        if (i == 0 && j == 1)
        {
          if (static_cast<double>(rand()) / static_cast<double>(RAND_MAX) < 0.25)
          {
            map->SetPixel(m_x_coord + i, m_y_coord + j, PIXEL_TYPE_FIRE);
          }
        }
        continue; 
      }

      surrounding_pixel_count++;

      if (pixel->m_flammability > 0)
      {
        if (static_cast<double>(rand()) / static_cast<double>(RAND_MAX) < static_cast<double>(pixel->m_flammability))
        {
          pixel->Ignite();
          surrounding_fire_count++;
        }
      }
    }
  }

  if (static_cast<double>(rand()) / static_cast<double>(RAND_MAX) < static_cast<double>(m_extinguishability * surrounding_pixel_count))
  {
    Extinguish();
    return;
  }
  if (static_cast<double>(rand()) / static_cast<double>(RAND_MAX) < static_cast<double>(m_volatility * (8 - surrounding_pixel_count)))
  {
    Decay(map);
  }

}
void Pixel::Decay(PixelMap* map)
{
  map->SetPixelUnsafe(m_x_coord, m_y_coord, PIXEL_TYPE_NULL);
}

void Pixel::Draw(PixelMap* map)
{
  m_updated_this_frame = false;
  if (m_on_fire)
  {
    DrawRectangle(m_x_coord * PIXEL_SIZE + map->GetX(), GetScreenHeight() - (m_y_coord + 1) * PIXEL_SIZE - map->GetY(), PIXEL_SIZE, PIXEL_SIZE, Color{0xff, (unsigned char)(rand() % 256), 0, 0xff});
    return;
  }
  DrawRectangle(m_x_coord * PIXEL_SIZE + map->GetX(), GetScreenHeight() - (m_y_coord + 1) * PIXEL_SIZE - map->GetY(), PIXEL_SIZE, PIXEL_SIZE, m_color);
}

int Pixel::GetXCoord() const { return m_x_coord; }
int Pixel::GetYCoord() const { return m_y_coord; }
Color Pixel::GetColor() const { return m_color; }
bool Pixel::IsSolid() const { return m_is_solid; }
bool Pixel::IsLiquid() const { return m_is_liquid; }
bool Pixel::IsOnFire() const { return m_on_fire; }
float Pixel::GetFlammability() const { return m_flammability; }
float Pixel::GetExtinguishability() const { return m_extinguishability; }
float Pixel::GetVolatility() const { return m_volatility; }

void Pixel::SetCoords(int x, int y)
{
  m_x_coord = x;
  m_y_coord = y;
}
void Pixel::SetColor(Color color)
{
  m_color = color;
}
void Pixel::AddColorVariance(float variance)
{
  float multiplier;
  multiplier = (1.0f + ((float)rand() / RAND_MAX - 0.5f) * variance);
  m_color.r = std::max(std::min(multiplier * m_color.r, 255.0f), 0.0f);
  multiplier = (1.0f + ((float)rand() / RAND_MAX - 0.5f) * variance);
  m_color.g = std::max(std::min(multiplier * m_color.g, 255.0f), 0.0f);
  multiplier = (1.0f + ((float)rand() / RAND_MAX - 0.5f) * variance);
  m_color.b = std::max(std::min(multiplier * m_color.b, 255.0f), 0.0f);
}
void Pixel::Darken()
{
  if (rand() % 15 == 0)
  {
    m_color.r = std::max(m_color.r * 0.999f, 0.0f);
    m_color.g = std::max(m_color.g * 0.999f, 0.0f);
    m_color.b = std::max(m_color.b * 0.999f, 0.0f);
  }
}
void Pixel::Ignite()
{
  m_on_fire = true;
}
void Pixel::Extinguish()
{
  m_on_fire = false;
}