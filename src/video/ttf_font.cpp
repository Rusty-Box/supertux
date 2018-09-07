//  SuperTux
//  Copyright (C) 2018 Ingo Ruhnke <grumbel@gmail.com>,
//                     Tobias Markus <tobbi.bugs@googlemail.com>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "video/ttf_font.hpp"

#include <iostream>
#include <sstream>

#include "physfs/physfs_sdl.hpp"
#include "video/canvas.hpp"
#include "video/surface.hpp"
#include "video/ttf_surface_manager.hpp"

TTFFont::TTFFont(const std::string& filename, int font_size, int shadow_size, int border) :
  m_font(),
  m_filename(),
  m_font_size(font_size),
  m_shadow_size(shadow_size),
  m_border(border)
{
  m_font = TTF_OpenFontRW(get_physfs_SDLRWops(filename), 1, font_size);
  if (!m_font)
  {
    std::ostringstream msg;
    msg << "Couldn't load TTFFont: " << filename << ": " << SDL_GetError();
    throw std::runtime_error(msg.str());
  }
}

TTFFont::~TTFFont()
{
  TTF_CloseFont(m_font);
}

float
TTFFont::get_text_width(const std::string& text) const
{
  if(text.empty())
    return 0.0f;

  int w = 0;
  int h = 0;
  int ret = TTF_SizeUTF8(m_font, text.c_str(), &w, &h);
  if (ret < 0)
  {
    std::cout << "TTFFont::get_text_width(): " << TTF_GetError() << std::endl;
  }

  return static_cast<float>(w);
}

float
TTFFont::get_text_height(const std::string& text) const
{
  if(text.empty())
    return 0.0f;

  float total_height = get_height();
  // since UTF8 multibyte characters are decoded with values
  // outside the ASCII range there is no risk of overlapping and
  // thus we don't need to decode the utf-8 string
  for(auto c : text)
  {
    if (c == '\n')
    {
      total_height += get_height();
    }
  }

  return total_height;
}

void
TTFFont::draw_text(Canvas& canvas, const std::string& text,
                   const Vector& pos, FontAlignment alignment, int layer, const Color& color)

{
  int last_pos = 0;
  float last_y = pos.y;
  for(size_t i = 0; i < text.size(); i++)
  {
    if (text[i] != '\n' && i != text.size() - 1)
    {
      continue;
    }

    std::string str;
    if(text[i] == '\n')
    {
      str = text.substr(last_pos, i - last_pos);
    }
    else
    {
      str = text.substr(last_pos, i + 1);
    }

    last_pos = static_cast<int>(i + 1);

    SurfacePtr surface = TTFSurfaceManager::current()->create_surface(*this, str);

    Vector new_pos(pos.x, last_y);

    if (alignment == ALIGN_CENTER)
    {
      new_pos.x -= static_cast<float>(surface->get_width()) / 2.0f;
    }
    else if (alignment == ALIGN_RIGHT)
    {
      new_pos.x -= static_cast<float>(surface->get_width());
    }

    // draw outline
    for(int offy = -m_shadow_size ; offy <= m_shadow_size; ++offy)
      for(int offx = -m_shadow_size ; offx <= m_shadow_size; ++offx)
        canvas.draw_surface(surface, new_pos + Vector(static_cast<float>(4 + offx), static_cast<float>(4 + offy)),
                            0.0f, Color(0, 0, 0, 0.125f), Blend(), layer);

    for(int offy = -1 ; offy <= 1; ++offy)
      for(int offx = -1 ; offx <= 1; ++offx)
        canvas.draw_surface(surface, new_pos + Vector(static_cast<float>(offx), static_cast<float>(offy)),
                            0.0f, Color::BLACK, Blend(), layer);

    // draw text
    canvas.draw_surface(surface, new_pos, 0.0f, color, Blend(), layer);

    last_y += get_height();
  }
}

std::string
TTFFont::wrap_to_width(const std::string& text, float width, std::string* overflow)
{
  std::string s = text;

  // if text is already smaller, return full text
  if (get_text_width(s) <= width) {
    if (overflow) *overflow = "";
    return s;
  }

  // if we can find a whitespace character to break at, return text up to this character
  for (int i = static_cast<int>(s.length()) - 1; i >= 0; i--) {
    std::string s2 = s.substr(0,i);
    if (s[i] != ' ') continue;
    if (get_text_width(s2) <= width) {
      if (overflow) *overflow = s.substr(i+1);
      return s.substr(0, i);
    }
  }

  // FIXME: hard-wrap at width, taking care of multibyte characters
  if (overflow) *overflow = "";
  return s;
}

/* EOF */
