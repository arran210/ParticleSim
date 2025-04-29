#pragma once
#include "pixel.h"

class Gas : public Pixel
{
public:
  Gas(int x, int y, Color color);
  virtual void Step(PixelMap* map) override;
};