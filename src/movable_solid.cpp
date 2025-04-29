#include "movable_solid.h"
#include "pixel_map.h"
#include <random>

MovableSolid::MovableSolid(int x, int y, Color color) : Solid(x, y, color) {}

void MovableSolid::Step(PixelMap* map)
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

  bool can_swap_down = !on_bottom_edge && map->GetPixelUnsafe(m_x_coord, m_y_coord - 1) != nullptr && !map->GetPixelUnsafe(m_x_coord, m_y_coord - 1)->IsSolid();
  if (can_swap_down)
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

  bool can_swap_down_left = !on_bottom_edge && !on_left_edge && map->GetPixelUnsafe(m_x_coord - 1, m_y_coord - 1) != nullptr && !map->GetPixelUnsafe(m_x_coord - 1, m_y_coord - 1)->IsSolid();
  bool can_swap_down_right = !on_bottom_edge && !on_right_edge && map->GetPixelUnsafe(m_x_coord + 1, m_y_coord - 1) != nullptr && !map->GetPixelUnsafe(m_x_coord + 1, m_y_coord - 1)->IsSolid();
  if (can_swap_down_left && can_swap_down_right)
  {
    if (rand() % 2 == 0)
    {
      map->SwapPixelsUnsafe(m_x_coord, m_y_coord, m_x_coord - 1, m_y_coord - 1);
    }
    else
    {
      map->SwapPixelsUnsafe(m_x_coord, m_y_coord, m_x_coord + 1, m_y_coord - 1);
    }
    return;
  }
  else if (can_swap_down_left)
  {
    map->SwapPixelsUnsafe(m_x_coord, m_y_coord, m_x_coord - 1, m_y_coord - 1);
    return;
  }
  else if (can_swap_down_right)
  {
    map->SwapPixelsUnsafe(m_x_coord, m_y_coord, m_x_coord + 1, m_y_coord - 1);
    return;
  }
}