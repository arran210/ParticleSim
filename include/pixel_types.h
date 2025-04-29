#pragma once
#include "pixel_base_types.h"

class Sand : public MovableSolid
{
public:
  Sand(int x, int y);
};

class Water : public Liquid
{
public:
  Water(int x, int y);
};

class Smoke : public Gas
{
public:
  Smoke(int x, int y);
};

class Stone : public ImmovableSolid
{
public:
  Stone(int x, int y);
};

class Fire : public Gas
{
public:
  Fire(int x, int y);

  void Update(PixelMap* map) override;
  void FireStep(PixelMap* map) override;
  void Draw(PixelMap* map) override;

  void Decay(PixelMap* map) override;
};

class Wood : public ImmovableSolid
{
public:
  Wood(int x, int y);
};

class Methane : public Gas
{
public:
  Methane(int x, int y);
};


enum PixelType
{
  PIXEL_TYPE_NULL = -1,
  PIXEL_TYPE_SAND,
  PIXEL_TYPE_WATER,
  PIXEL_TYPE_SMOKE,
  PIXEL_TYPE_STONE,
  PIXEL_TYPE_FIRE,
  PIXEL_TYPE_COUNT,
  PIXEL_TYPE_WOOD,
  PIXEL_TYPE_METHANE
};