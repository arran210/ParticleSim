#include "solid.h"

Solid::Solid(int x, int y, Color color) : Pixel(x, y, color) 
{
  m_is_solid = true;
}