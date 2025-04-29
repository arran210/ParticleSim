#include "immovable_solid.h"

ImmovableSolid::ImmovableSolid(int x, int y, Color color) : Solid(x, y, color) 
{
  m_is_solid = true;
}

void ImmovableSolid::Step(PixelMap* map)
{
  return;
}