#pragma once
#include "pixel_types.h"
#include <string>
#include <vector>
#include <utility>
// Pre-declare PixelMap
class PixelMap;

class MapCursor
{
private:
  int m_x = 0;
  int m_y = 0;
  int m_size = 10;
  int m_prev_x = 0;
  int m_prev_y = 0;

  PixelType m_selected_type = PIXEL_TYPE_SAND;
  std::string m_selected_type_name = "Sand";

  std::string m_size_display = "";
  std::string m_selected_type_display = "";

  int m_selected_tab_index = 0;
  int m_selected_pixel_tab_index = 0;
  int m_selected_pixel_index = 0;
  std::vector<std::string> m_tab_names = {
    "Solids",
    "Liquids",
    "Gases",
    "Other"
  };
  std::vector<std::vector<std::pair<PixelType, std::string>>> m_tab_pixels = {
    {
      { PIXEL_TYPE_SAND, "Sand" },
      { PIXEL_TYPE_STONE, "Stone" },
      { PIXEL_TYPE_WOOD, "Wood" }
    },
    {
      { PIXEL_TYPE_WATER, "Water" }
    },
    {
      { PIXEL_TYPE_SMOKE, "Smoke" },
      { PIXEL_TYPE_METHANE, "Methane" }
    },
    {
      { PIXEL_TYPE_FIRE, "Fire" }
    }
  };


public:
  MapCursor();
  void Update(PixelMap* pixel_map);
  void Draw(PixelMap* pixel_map) const;
};