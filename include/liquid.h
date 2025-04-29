#pragma once
#include "pixel.h"

class Liquid : public Pixel
{
protected:
  int m_max_jump = 1;
public:
  Liquid(int x, int y, Color color);
  virtual void Step(PixelMap* map) override;
};