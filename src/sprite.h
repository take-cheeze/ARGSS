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

#ifndef _SPRITE_H_
#define _SPRITE_H_

///////////////////////////////////////////////////////////
// Headers
///////////////////////////////////////////////////////////
#include <string>
#include "color.h"
#include "tone.h"
#include "rect.h"
#include "drawable.h"
#include "opengl.h"
#include "bitmap_fwd.h"

///////////////////////////////////////////////////////////
/// Sprite class
///////////////////////////////////////////////////////////
class Sprite : public Drawable {
public:
  Sprite();
  ~Sprite();

  void RefreshBitmaps();
  void Draw(long z);

  void Flash(int duration);
  void Flash(Color color, int duration);
  void Update();

  RectRef src_rect;
  bool visible;
  int x;
  int y;
  int ox;
  int oy;
  float zoom_x;
  float zoom_y;
  float angle;
  int bush_depth;
  ColorRef color;
  int blend_type;
  int opacity;

  bool flipx;
  bool flipy;

  ToneRef const& tone() const { return tone_; }
  BitmapRef const& bitmap() const { return bitmap_; }

  void tone(ToneRef const& t);
  void bitmap(BitmapRef const& bmp);

 private:
  ToneRef tone_;
  BitmapRef bitmap_;

  GLuint flash_texture;
  Color flash_color;
  int flash_duration;
  int flash_frame;
  BitmapRef sprite;
  Rect src_rect_sprite;
  Rect src_rect_last;
  bool needs_refresh;
  bool flash_needs_refresh;

  void Refresh();
  void RefreshFlash();
};

#endif
