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

#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

///////////////////////////////////////////////////////////
// Headers
///////////////////////////////////////////////////////////
#include <string>
#include <list>
#include <map>

#include "bitmap_fwd.h"

class Rect;
class Color;
class ZObj;
class Drawable;
struct Texture;

///////////////////////////////////////////////////////////
/// Graphics namespace
///////////////////////////////////////////////////////////
namespace Graphics {
  struct Clipper {
    Clipper(Rect const& r);
    ~Clipper();
  };

  void Init();
  void InitOpenGL();
  void Exit();
  void RefreshAll();
  void TimerWait();
  void TimerContinue();
  void DrawFrame();

  void Update();
  void Freeze();
  void Transition(int duration, std::string const& filename, int vague);
  void FrameReset();
  void Wait(int duration);
  void ResizeScreen(int width, int height);
  BitmapRef SnapToBitmap();
  void FadeOut(int duration);
  void FadeIn(int duration);
  int GetFrameRate();
  void SetFrameRate(int nframerate);
  int GetFrameCount();
  void SetFrameCount(int nframecount);
  ColorRef const& GetBackColor();
  void SetBackColor(ColorRef const& nbackcolor);
  int GetBrightness();
  void SetBrightness(int nbrightness);

  void Clip(Rect const& r);
  void Unclip();

  bool SortZObj(ZObj &first, ZObj &second);
  void RegisterZObj(long z, Drawable& d);
  void RegisterZObj(long z, Drawable& id, bool multiz);
  void RemoveZObj(Drawable& id);
  void UpdateZObj(Drawable& id, long z);

  Texture const& GetTexture(BitmapRef const& bmp);

  int get_creation_id();

  int GetFPS();
};

#endif
