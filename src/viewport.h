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

#ifndef _VIEWPORT_H_
#define _VIEWPORT_H_

///////////////////////////////////////////////////////////
// Headers
///////////////////////////////////////////////////////////
#include <string>
#include <list>
#include "color.h"
#include "rect.h"
#include "tone.h"
#include "drawable.h"
#include "zobj.h"
#include "bitmap_fwd.h"

///////////////////////////////////////////////////////////
/// Viewport class
///////////////////////////////////////////////////////////
class Viewport : public Drawable {
public:
  Viewport();
  ~Viewport();

  void RefreshBitmaps();
  void Draw(long z);
  void Draw(long z, Bitmap* dst_bitmap);

  void Flash(int duration);
  void Flash(Color const& color, int duration);
  void Update();

  void RegisterZObj(long z, Drawable& id);
  void RegisterZObj(long z, Drawable& id, bool multiz);
  void RemoveZObj(Drawable& id);
  void UpdateZObj(Drawable& id, long z);

  Rect GetViewportRect();

  RectRef rect;
  bool visible;
  int ox;
  int oy;
  ColorRef color;
  ToneRef tone;

  int z() const { return z_; }
  void z(int nz);

 private:
  std::list<ZObj> zlist;

  int z_;

  Color flash_color;
  int flash_duration;
  int flash_frame;
  bool disposing;

  Rect dst_rect;
};

#endif
