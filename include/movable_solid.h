#pragma once
#include "solid.h"

class MovableSolid : public Solid
{
public:
  MovableSolid(int x, int y, Color color);
  virtual void Step(PixelMap* map) override;
};