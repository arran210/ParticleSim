#include "liquid.h"
#include "pixel_map.h"
#include <random>

Liquid::Liquid(int x, int y, Color color) : Pixel(x, y, color) 
{
  m_is_solid = false;
  m_is_liquid = true;
}

void Liquid::Step(PixelMap* map)
{
  bool on_bottom_edge = m_y_coord == 0;
  bool on_left_edge = m_x_coord == 0;
  bool on_right_edge = m_x_coord == map->GetWidth() - 1;

  bool can_move_down = !on_bottom_edge && map->GetPixelUnsafe(m_x_coord, m_y_coord - 1) == nullptr;
  if (can_move_down)
  {
    map->MovePixelUnsafe(m_x_coord, m_y_coord, m_x_coord, m_y_coord - 1);
    return;
  }
  // if pixel is gas
  if (!on_bottom_edge && !map->GetPixelUnsafe(m_x_coord, m_y_coord - 1)->IsLiquid() && !map->GetPixelUnsafe(m_x_coord, m_y_coord - 1)->IsSolid())
  {
    map->SwapPixelsUnsafe(m_x_coord, m_y_coord, m_x_coord, m_y_coord - 1);
    return;
  }

  bool can_move_down_left = !on_bottom_edge && !on_left_edge && map->GetPixelUnsafe(m_x_coord - 1, m_y_coord - 1) == nullptr;
  bool can_move_down_right = !on_bottom_edge && !on_right_edge && map->GetPixelUnsafe(m_x_coord + 1, m_y_coord - 1) == nullptr;

  if (can_move_down_left && can_move_down_right)
  {
    if (rand() % 2 == 0)
    {
      map->MovePixelUnsafe(m_x_coord, m_y_coord, m_x_coord - 1, m_y_coord - 1);
    }
    else
    {
      map->MovePixelUnsafe(m_x_coord, m_y_coord, m_x_coord + 1, m_y_coord - 1);
    }
    return;
  }
  else if (can_move_down_left)
  {
    map->MovePixelUnsafe(m_x_coord, m_y_coord, m_x_coord - 1, m_y_coord - 1);
    return;
  }
  else if (can_move_down_right)
  {
    map->MovePixelUnsafe(m_x_coord, m_y_coord, m_x_coord + 1, m_y_coord - 1);
    return;
  }

  int left_move_distance = 0;
  for (int i = 1; i <= m_max_jump; i++)
  {
    if (m_x_coord - i < 0) { break; }
    Pixel* pixel = map->GetPixelUnsafe(m_x_coord - i, m_y_coord);
    if (pixel == nullptr)
    {
      left_move_distance = i;
      break;
    }
    else
    {
      if (pixel->IsSolid())
      {
        break;
      }
    }
  }

  int right_move_distance = 0;
  for (int i = 1; i <= m_max_jump; i++)
  {
    if (m_x_coord + i >= map->GetWidth()) { break; }
    Pixel* pixel = map->GetPixelUnsafe(m_x_coord + i, m_y_coord);
    if (pixel == nullptr)
    {
      right_move_distance = i;
      break;
    }
    else
    {
      if (pixel->IsSolid())
      {
        break;
      }
    }
  }

  if (left_move_distance > right_move_distance)
  {
    map->MovePixelUnsafe(m_x_coord, m_y_coord, m_x_coord - left_move_distance, m_y_coord);
    return;
  }
  else if (right_move_distance > left_move_distance)
  {
    map->MovePixelUnsafe(m_x_coord, m_y_coord, m_x_coord + right_move_distance, m_y_coord);
    return;
  }
  else
  {
    if (left_move_distance > 0)
    {
      if (rand() % 2 == 0)
      {
        map->MovePixelUnsafe(m_x_coord, m_y_coord, m_x_coord - left_move_distance, m_y_coord);
        return;
      }
      else
      {
        map->MovePixelUnsafe(m_x_coord, m_y_coord, m_x_coord + right_move_distance, m_y_coord);
        return;
      }
    }
  }
}