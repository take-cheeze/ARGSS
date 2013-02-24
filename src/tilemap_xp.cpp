/////////////////////////////////////////////////////////////////////////////
// ARGSS - Copyright (c) 2009 - 2010, Alejandro Marzini (vgvgf)
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//  * Redistributions of source code must retain the above copyright notice,
//  this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//  notice, this list of conditions and the following disclaimer in the
//  documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ''AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
/////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// Headers
///////////////////////////////////////////////////////////
#include <string>
#include <map>
#include <math.h>
#include "tilemap_xp.h"
#include "graphics.h"
#include "viewport.h"
#include "player.h"
#include "bitmap.h"
#include "table.h"
#include "texture.h"

///////////////////////////////////////////////////////////
/// Static Variables
///////////////////////////////////////////////////////////
const uint8_t Tilemap::autotiles_id[6][8][4] = {
  { {26, 27, 32, 33}, {04, 27, 32, 33}, {26, 05, 32, 33}, {04, 05, 32, 33},
    {26, 27, 32, 11}, {04, 27, 32, 11}, {26, 05, 32, 11}, {04, 05, 32, 11} },
  { {26, 27, 10, 33}, {04, 27, 10, 33}, {26, 05, 10, 33}, {04, 05, 10, 33},
    {26, 27, 10, 11}, {04, 27, 10, 11}, {26, 05, 10, 11}, {04, 05, 10, 11} },
  { {24, 25, 30, 31}, {24, 05, 30, 31}, {24, 25, 30, 11}, {24, 05, 30, 11},
    {14, 15, 20, 21}, {14, 15, 20, 11}, {14, 15, 10, 21}, {14, 15, 10, 11} },
  { {28, 29, 34, 35}, {28, 29, 10, 35}, {04, 29, 34, 35}, {04, 29, 10, 35},
    {38, 39, 44, 45}, {04, 39, 44, 45}, {38, 05, 44, 45}, {04, 05, 44, 45} },
  { {24, 29, 30, 35}, {14, 15, 44, 45}, {12, 13, 18, 19}, {12, 13, 18, 11},
    {16, 17, 22, 23}, {16, 17, 10, 23}, {40, 41, 46, 47}, {04, 41, 46, 47} },
  { {36, 37, 42, 43}, {36, 05, 42, 43}, {12, 17, 18, 23}, {12, 13, 42, 43},
    {36, 41, 42, 47}, {16, 17, 46, 47}, {12, 17, 42, 47}, {00, 01, 06, 07} }
};

///////////////////////////////////////////////////////////
/// Constructor
///////////////////////////////////////////////////////////
Tilemap::Tilemap() {
  visible = true;
  ox = 0;
  oy = 0;
  autotile_time = 0;
  autotile_frame = 0;
}

Tilemap::~Tilemap() {
  if (viewport_) {
    viewport_->RemoveZObj(*this);
  } else {
    Graphics::RemoveZObj(*this);
  }
}

///////////////////////////////////////////////////////////
/// Update
///////////////////////////////////////////////////////////
void Tilemap::Update() {
  autotile_time += 1;
  if (autotile_time == 8) {
    autotile_time = 0;
    autotile_frame += 1;
  }
}

///////////////////////////////////////////////////////////
/// Refresh Bitmaps
///////////////////////////////////////////////////////////
void Tilemap::RefreshBitmaps() {
  std::map<Bitmap*, std::map<int, std::map<int, BitmapRef> > >::iterator it1_autotiles_cache;
  std::map<int, std::map<int, BitmapRef> >::iterator it2_autotiles_cache;
  std::map<int, BitmapRef>::iterator it3_autotiles_cache;
  for (it1_autotiles_cache = autotiles_cache.begin(); it1_autotiles_cache != autotiles_cache.end(); it1_autotiles_cache++) {
    for (it2_autotiles_cache = it1_autotiles_cache->second.begin(); it2_autotiles_cache != it1_autotiles_cache->second.end(); it2_autotiles_cache++) {
      for (it3_autotiles_cache = it2_autotiles_cache->second.begin(); it3_autotiles_cache != it2_autotiles_cache->second.end(); it3_autotiles_cache++) {
        it3_autotiles_cache->second->Changed();
      }
    }
  }
}

///////////////////////////////////////////////////////////
/// Draw
///////////////////////////////////////////////////////////
void Tilemap::Draw(long z_level) {
  if (not visible or not tileset or not map_data_ or not priorities) return;

  if (z_level == 0) {
    RefreshData();
  }

  int width = map_data_->xsize();
  int height = map_data_->ysize();
  int layers = map_data_->zsize();

  glEnable(GL_TEXTURE_2D);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  if (viewport_) {
    Rect rect = viewport_->GetViewportRect();

    glEnable(GL_SCISSOR_TEST);
    glScissor(rect.x, Player::GetHeight() - (rect.y + rect.height), rect.width, rect.height);

    glTranslatef((float)rect.x, (float)rect.y, 0.0f);
  }

  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

  int tiles_x = (int)ceil(Player::GetWidth() / 32.0);
  int tiles_y = (int)ceil(Player::GetHeight() / 32.0);

  for (int z = 0; z < layers; z++) {
    for (int y = 0; y <= tiles_y; y++) {
      for (int x = 0; x <= tiles_x; x++) {
        int map_x = ox / 32 + x;
        int map_y = oy / 32 + y;

        if (map_x >= width || map_y >= height) continue;

        TileData tile = data_cache[map_x][map_y][z];

        int tile_z;
        if (tile.priority == 0) {
          tile_z = 0;
        } else {
          tile_z = tile.priority * 32 + y * 32 + z * 32;
          if (map_y == 0 && tile.priority == 1) tile_z += 32;
        }

        if (tile_z == z_level) {
          float dst_x = (float)(x * 32 - ox % 32);
          float dst_y = (float)(y * 32 - oy % 32);
          if (tile.id < 384 && tile.id != 0) {
              BitmapRef const bitmap_id = autotiles[tile.id / 48 - 1];
              if (not bitmap_id) continue;
              Bitmap* autotile_bitmap = bitmap_id.get();
              int tile_id = tile.id % 48;
              int frame = autotile_frame % (autotile_bitmap->GetWidth() / 96);

              if (autotiles_cache.count(bitmap_id.get()) == 0 ||
                  autotiles_cache[bitmap_id.get()].count(tile_id) == 0 ||
                  autotiles_cache[bitmap_id.get()][tile_id].count(frame) == 0) {
                autotiles_cache[bitmap_id.get()][tile_id][frame] = boost::make_shared<Bitmap>(32, 32);
                uint8_t const* tiles = autotiles_id[tile_id >> 3][tile_id & 7];
                for (int i = 0; i < 4; i++) {
                  Rect rect(((tiles[i] % 6) << 4) + frame * 96, (tiles[i] / 6) << 4, 16, 16);
                  autotiles_cache[bitmap_id.get()][tile_id][frame]->Blit((i % 2) << 4, (i / 2) << 4, *autotile_bitmap, rect, 255);
                }
                glPushMatrix();

                // autotiles_cache[bitmap_id.get()][tile_id][frame]->Refresh();

                glMatrixMode(GL_MODELVIEW);
                glPopMatrix();
              }
              Graphics::GetTexture(autotiles_cache[bitmap_id.get()][tile_id][frame]).Bind();

              glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 0.0f); glVertex2f(dst_x, dst_y);
                glTexCoord2f(1.0f, 0.0f); glVertex2f(dst_x + 32.0f, dst_y);
                glTexCoord2f(1.0f, 1.0f); glVertex2f(dst_x + 32.0f, dst_y + 32.0f);
                glTexCoord2f(0.0f, 1.0f); glVertex2f(dst_x, dst_y + 32.0f);
              glEnd();
          } else if (tile.id != 0){
            float src_x = (float)((tile.id - 384) % 8 * 32);
            float src_y = (float)((tile.id - 384) / 8 * 32);

            Graphics::GetTexture(tileset).Bind();

            float bmpw = (float)tileset->GetWidth();
            float bmph = (float)tileset->GetHeight();

            glBegin(GL_QUADS);
              glTexCoord2f(src_x / bmpw, src_y / bmph);            glVertex2f(dst_x, dst_y);
              glTexCoord2f((src_x + 32.0f) / bmpw, src_y / bmph);        glVertex2f(dst_x + 32.0f, dst_y);
              glTexCoord2f((src_x + 32.0f) / bmpw, (src_y + 32.0f) / bmph);  glVertex2f(dst_x + 32.0f, dst_y + 32.0f);
              glTexCoord2f(src_x / bmpw, (src_y + 32.0f) / bmph);        glVertex2f(dst_x, dst_y + 32.0f);
            glEnd();
          }
        }
      }
    }
  }

  glDisable(GL_SCISSOR_TEST);
}

///////////////////////////////////////////////////////////
/// Refresh Data
///////////////////////////////////////////////////////////
void Tilemap::RefreshData() {
  int width = map_data_->xsize();
  int height = map_data_->ysize();
  int layers = map_data_->zsize();

  data_cache.resize(width);
  for (int x = 0; x < width; x++) {
    data_cache[x].resize(height);
    for (int y = 0; y < height; y++) {
      data_cache[x][y].resize(layers);
      for (int z = 0; z < layers; z++) {
        TileData tile;
        tile.id = (*map_data_)(x, y, z);
        tile.priority = (*priorities)(tile.id);
        data_cache[x][y][z] = tile;
      }
    }
  }
}

///////////////////////////////////////////////////////////
/// Properties
///////////////////////////////////////////////////////////
void Tilemap::viewport(ViewportRef const& nviewport) {
  if (viewport_ != nviewport) {
    if (viewport_) {
      viewport_->RemoveZObj(*this);
    } else {
      Graphics::RemoveZObj(*this);
    }
    int height = map_data_->ysize();
    if (nviewport) {
      for (int i = 0; i < height + 5; i++) {
        nviewport->RegisterZObj(i * 32, *this, true);
      }
    } else {
      for (int i = 0; i < height + 5; i++) {
        Graphics::RegisterZObj(i * 32, *this, true);
      }
    }
  }
  viewport_ = nviewport;
}
void Tilemap::map_data(TableRef const& nmap_data) {
  if (map_data_ != nmap_data) {
    if (viewport_) {
      viewport_->RemoveZObj(*this);
    } else {
      Graphics::RemoveZObj(*this);
    }
    if (nmap_data) {
      int height = nmap_data->ysize();
      if (viewport_) {
        for (int i = 0; i < height + 8; i++) {
          viewport_->RegisterZObj(i * 32, *this, true);
        }
      } else {
        for (int i = 0; i < height + 8; i++) {
          Graphics::RegisterZObj(i * 32, *this, true);
        }
      }
    }
  }
  map_data_ = nmap_data;
}
