#include "pixel_types.h"
#include "pixel_map.h"
#include <iostream>
#include <random>

// Sand
Sand::Sand(int x, int y) : MovableSolid(x, y, Color{0xa6, 0x91, 0x50, 0xff})
{
  AddColorVariance(0.1f);
}

// Water
Water::Water(int x, int y) : Liquid(x, y, Color{ 0x00, 0x00, 0xff, 0xff })
{
  m_max_jump = 4;
}

// Smoke
Smoke::Smoke(int x, int y) : Gas(x, y, LIGHTGRAY)
{
  
}

// Stone
Stone::Stone(int x, int y) : ImmovableSolid(x, y, DARKGRAY) 
{
  AddColorVariance(0.1f);
}

// Fire
Fire::Fire(int x, int y) : Gas(x, y, RED)
{
  m_volatility = 0.15f;
}
void Fire::Update(PixelMap* map)
{
  if (m_updated_this_frame) { return; }
  m_updated_this_frame = true;
  Gas::Step(map);
  FireStep(map);
}
void Fire::FireStep(PixelMap* map)
{
  if (static_cast<double>(rand()) / static_cast<double>(RAND_MAX) < m_volatility)
  {
    Decay(map);
    return;
  }
  for (int i = -1; i <= 1; i++)
  {
    for (int j = -1; j <= 1; j++)
    {
      if (i == 0 && j == 0) { continue; }
      Pixel* pixel = map->GetPixel(m_x_coord + i, m_y_coord + j);
      if (pixel == nullptr) { continue; }
      if (pixel->GetFlammability() > 0)
      {
        if (static_cast<double>(rand()) / static_cast<double>(RAND_MAX) < static_cast<double>(pixel->GetFlammability()) * 1.5)
        {
          pixel->Ignite();
        }
      }
    }
  }
}
void Fire::Draw(PixelMap* map)
{
  m_updated_this_frame = false;
  DrawRectangle(m_x_coord * PIXEL_SIZE + map->GetX(), GetScreenHeight() - (m_y_coord + 1) * PIXEL_SIZE - map->GetY(), PIXEL_SIZE, PIXEL_SIZE, Color{ 0xff, (unsigned char)(rand() % 256), 0, 0xff });
}
void Fire::Decay(PixelMap* map)
{
  if (static_cast<double>(rand()) / static_cast<double>(RAND_MAX) < 0.0025)
  {
    map->SetPixelUnsafe(m_x_coord, m_y_coord, PIXEL_TYPE_SMOKE);
  }
  else
  {
    map->SetPixelUnsafe(m_x_coord, m_y_coord, PIXEL_TYPE_NULL);
  }
}

// Wood
Wood::Wood(int x, int y) : ImmovableSolid(x, y, Color{ 0x8b, 0x5b, 0x29, 0xff })
{
  m_flammability = 0.025f;
  m_extinguishability = 0.005f;
  m_volatility = 0.002f;
}

Methane::Methane(int x, int y) : Gas(x, y, Color{ 0x00, 0xff, 0x00, 0xff })
{
  m_flammability = 0.5f;
  m_extinguishability = 0.0f;
  m_volatility = 0.1f;
}