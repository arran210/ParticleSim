#pragma once
#include <raylib.h>
#include <raymath.h>

constexpr int PIXEL_SIZE = 4;

// Pre-declare PixelMap
class PixelMap;

class Pixel
{
protected:
  int m_x_coord;
  int m_y_coord;
  Color m_color;

  bool m_is_solid = false;
  bool m_is_liquid = false;
  bool m_updated_this_frame = false;

  bool m_on_fire = false;
  // likelihood of catching fire
  float m_flammability = 0.0f;
  // likelihood of extinguishing
  float m_extinguishability = 0.0f;
  // likelihood of decaying
  float m_volatility = 0.0f;
  
public:
  Pixel(int x, int y, Color color);

  virtual void Update(PixelMap* map);
  virtual void Step(PixelMap* map) = 0;
  virtual void FireStep(PixelMap* map);
  virtual void Decay(PixelMap* map);
  virtual void Draw(PixelMap* map);

  int GetXCoord() const;
  int GetYCoord() const;
  Color GetColor() const;
  bool IsSolid() const;
  bool IsLiquid() const;
  bool IsOnFire() const;
  float GetFlammability() const;
  float GetExtinguishability() const;
  float GetVolatility() const;

  void SetCoords(int x, int y);
  void SetColor(Color color);
  void AddColorVariance(float variance);
  void Darken();
  void Ignite();
  void Extinguish();
};