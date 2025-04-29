#include "map_cursor.h"
#include "pixel.h"
#include "pixel_map.h"
#include <raylib.h>

MapCursor::MapCursor()
{
  m_size_display = "Cursor Size: " + std::to_string(m_size + 1);
  m_selected_type_display = "Selected Type: " + m_selected_type_name;
}

void MapCursor::Update(PixelMap* map)
{
  Vector2 mouse_pos = GetMousePosition();
  Vector2 mouse_delta = GetMouseDelta();

  if (mouse_pos.y > GetScreenHeight() - 80)
  {
    // check if mouse is in the UI
    if (IsCursorHidden())
    {
      ShowCursor();
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
      if (mouse_pos.x > 200 && mouse_pos.x < GetScreenWidth() - 200)
      {
        // check if mouse is in the tab area
        if (mouse_pos.y > GetScreenHeight() - 80 && mouse_pos.y < GetScreenHeight() - 60)
        {
          // check which tab was clicked
          for (int i = 0; i < m_tab_names.size(); i++)
          {
            if (mouse_pos.x > 200 + i * ((GetScreenWidth() - 200) / 4) && mouse_pos.x < 200 + (i + 1) * ((GetScreenWidth() - 200) / 4))
            {
              m_selected_tab_index = i;
              break;
            }
          }
        }
      }

      int pixel_name_x = 204;
      int pixel_name_y = GetScreenHeight() - 56;
      for (int i = 0; i < m_tab_pixels[m_selected_tab_index].size(); i++)
      {
        if (mouse_pos.x > pixel_name_x && mouse_pos.x < pixel_name_x + 100 && mouse_pos.y > pixel_name_y && mouse_pos.y < pixel_name_y + 15)
        {
          m_selected_pixel_tab_index = m_selected_tab_index;
          m_selected_pixel_index = i;
          m_selected_type = m_tab_pixels[m_selected_tab_index][m_selected_pixel_index].first;
          m_selected_type_name = m_tab_pixels[m_selected_tab_index][m_selected_pixel_index].second;
          m_selected_type_display = "Selected Type: " + m_selected_type_name;
          break;
        }

        pixel_name_x += 100;
        if (pixel_name_x > GetScreenWidth() - 100)
        {
          pixel_name_x = 204;
          pixel_name_y += 16;
        }
      }
    }


    return;
  }
  else
  {
    if (!IsCursorHidden())
    {
      HideCursor();
    }
  }

  // used for drawing outline
  m_x = (static_cast<int>(mouse_pos.x) + PIXEL_SIZE - m_size / 2) - (m_x - m_size / 2) % PIXEL_SIZE;
  m_y = (static_cast<int>(mouse_pos.y) + PIXEL_SIZE - m_size / 2) - (m_y - m_size / 2) % PIXEL_SIZE;
  // used for pixel map
  int mouse_x = m_x / PIXEL_SIZE;
  int mouse_y = (GetScreenHeight() - m_y) / PIXEL_SIZE;
  
  int lbx = mouse_x - map->GetX() / PIXEL_SIZE;
  int lby = mouse_y - m_size - map->GetY() / PIXEL_SIZE;
  int ubx = mouse_x + m_size - map->GetX() / PIXEL_SIZE;
  int uby = mouse_y - map->GetY() / PIXEL_SIZE;
  int mouse_dx = lbx - m_prev_x;
  int mouse_dy = lby - m_prev_y;

  // update cursor size
  if (IsKeyPressed(KEY_UP) || IsKeyPressedRepeat(KEY_UP))
  {
    m_size++;
    if (m_size > std::max(map->GetWidth(), map->GetHeight()))
    {
      m_size = std::max(map->GetWidth(), map->GetHeight());
    }
    m_size_display = "Cursor Size: " + std::to_string(m_size + 1);
  }
  if (IsKeyPressed(KEY_DOWN) || IsKeyPressedRepeat(KEY_DOWN))
  {
    m_size--;
    if (m_size < 0)
    {
      m_size = 0;
    }
    m_size_display = "Cursor Size: " + std::to_string(m_size + 1);
  }

  if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
  {
    // set the pixel at the cursor position
    map->SetPixelRectLine(lbx, lby, ubx, uby, -mouse_dx, -mouse_dy, m_selected_type);
  }
  if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
  {
    // erase the pixel at the cursor position
    map->SetPixelRectLine(lbx, lby, ubx, uby, -mouse_dx, -mouse_dy, PIXEL_TYPE_NULL);
  }

  m_prev_x = lbx;
  m_prev_y = lby;
}

void MapCursor::Draw(PixelMap* pixel_map) const
{
  // Draw cursor
  DrawRectangleLines(m_x, m_y, (m_size + 1) * PIXEL_SIZE, (m_size + 1) * PIXEL_SIZE, RED);
  
// Draw UI
  // Draw UI background
  DrawRectangle(0, GetScreenHeight() - 80, GetScreenWidth(), 80, BLACK);
  // Display Cursor Size
  DrawText(m_size_display.c_str(), 0, GetScreenHeight() - 80, 16, WHITE);
  // Display Selected Type
  DrawText(m_selected_type_display.c_str(), 0, GetScreenHeight() - 60, 16, WHITE);

  // Draw main pixel selection background
  DrawRectangleLinesEx(Rectangle{ 200, (float)(GetScreenHeight() - 60), (float)(GetScreenWidth() - 200), 60 }, 2, WHITE);
  
  for (int i = 0; i < m_tab_names.size(); i++)
  {
    // Draw tab names
    DrawText(m_tab_names[i].c_str(), 204 + i * ((GetScreenWidth() - 200) / 4), GetScreenHeight() - 78, 16, WHITE);
    if (i == m_selected_tab_index)
    {
      // Draw selected tab background
      DrawRectangleLinesEx(Rectangle{ (float)200 + i * ((GetScreenWidth() - 200) / m_tab_names.size()), (float)(GetScreenHeight() - 80), (float)((GetScreenWidth() - 200) / 4), 20 }, 2, WHITE);
      
      int pixel_name_x = 204;
      int pixel_name_y = GetScreenHeight() - 56;
      int pixel_name_size = 12;
      // Draw tab contents
      for (int j = 0; j < m_tab_pixels[i].size(); j++)
      {
        DrawText(m_tab_pixels[i][j].second.c_str(), pixel_name_x, pixel_name_y, pixel_name_size, WHITE);
        if (m_selected_tab_index == m_selected_pixel_tab_index && j == m_selected_pixel_index)
        {
          // Draw selected pixel background
          DrawRectangleLinesEx(Rectangle{ (float)pixel_name_x - 2, (float)pixel_name_y - 2, 100, 15 }, 1, RED);
        }
        pixel_name_x += 100;
        if (pixel_name_x > GetScreenWidth() - 100)
        {
          pixel_name_x = 204;
          pixel_name_y += 16;
        }
      }
    }
  }
}