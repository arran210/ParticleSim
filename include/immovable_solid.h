#pragma once
#include "solid.h"

class ImmovableSolid : public Solid
{
private:
  float m_stickiness = 0;
  bool m_stuck = false;
public:
  ImmovableSolid(int x, int y, Color color);
  virtual void Step(PixelMap* map) override;
};