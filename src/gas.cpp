#include "gas.h"
#include "pixel_map.h"
#include <random>
#include <iostream>

Gas::Gas(int x, int y, Color color) : Pixel(x, y, color) 
{
  m_is_solid = false;
}

void Gas::Step(PixelMap* map)
{
  bool on_top_edge = m_y_coord == map->GetHeight() - 1;
  bool on_bottom_edge = m_y_coord == 0;
  bool on_left_edge = m_x_coord == 0;
  bool on_right_edge = m_x_coord == map->GetWidth() - 1;

  bool can_move_up = !on_top_edge && map->GetPixelUnsafe(m_x_coord, m_y_coord + 1) == nullptr;
  if (can_move_up)
  {
    map->MovePixelUnsafe(m_x_coord, m_y_coord, m_x_coord, m_y_coord + 1);
    return;
  }

  bool can_move_up_left = !on_top_edge && !on_left_edge && map->GetPixelUnsafe(m_x_coord - 1, m_y_coord + 1) == nullptr;
  bool can_move_up_right = !on_top_edge && !on_right_edge && map->GetPixelUnsafe(m_x_coord + 1, m_y_coord + 1) == nullptr;

  if (can_move_up_left && can_move_up_right)
  {
    if (rand() % 2 == 0)
    {
      map->MovePixelUnsafe(m_x_coord, m_y_coord, m_x_coord - 1, m_y_coord + 1);
    }
    else
    {
      map->MovePixelUnsafe(m_x_coord, m_y_coord, m_x_coord + 1, m_y_coord + 1);
    }
    return;
  }
  else if (can_move_up_left)
  {
    map->MovePixelUnsafe(m_x_coord, m_y_coord, m_x_coord - 1, m_y_coord + 1);
    return;
  }
  else if (can_move_up_right)
  {
    map->MovePixelUnsafe(m_x_coord, m_y_coord, m_x_coord + 1, m_y_coord + 1);
    return;
  }

  bool can_move_left = !on_left_edge && map->GetPixelUnsafe(m_x_coord - 1, m_y_coord) == nullptr;
  bool can_move_right = !on_right_edge && map->GetPixelUnsafe(m_x_coord + 1, m_y_coord) == nullptr;

  if (can_move_left && can_move_right)
  {
    if (rand() % 2 == 0)
    {
      map->MovePixelUnsafe(m_x_coord, m_y_coord, m_x_coord - 1, m_y_coord);
    }
    else
    {
      map->MovePixelUnsafe(m_x_coord, m_y_coord, m_x_coord + 1, m_y_coord);
    }
    return;
  }
  else if (can_move_left)
  {
    map->MovePixelUnsafe(m_x_coord, m_y_coord, m_x_coord - 1, m_y_coord);
    return;
  }
  else if (can_move_right)
  {
    map->MovePixelUnsafe(m_x_coord, m_y_coord, m_x_coord + 1, m_y_coord);
    return;
  }

  bool can_move_down = !on_bottom_edge && map->GetPixelUnsafe(m_x_coord, m_y_coord - 1) == nullptr;
  if (can_move_down)
  {
    map->MovePixelUnsafe(m_x_coord, m_y_coord, m_x_coord, m_y_coord - 1);
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
}