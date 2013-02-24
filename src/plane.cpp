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
#include <cmath>
#include "plane.h"
#include "graphics.h"
#include "viewport.h"
#include "rect.h"
#include "player.h"
#include "bitmap.h"
#include "texture.h"
#include "color.h"

///////////////////////////////////////////////////////////
/// Constructor
///////////////////////////////////////////////////////////
Plane::Plane() {
  visible = true;
  z_ = 0;
  ox = 0;
  oy = 0;
  zoom_x = 1.0;
  zoom_y = 1.0;
  opacity = 255;
  blend_type = 0;
  color = boost::make_shared<Color>(0, 0, 0, 255);
  tone = boost::make_shared<Tone>(0, 0, 0, 0);

  if (viewport_) {
    viewport_->RegisterZObj(0, *this);
  } else {
    Graphics::RegisterZObj(0, *this);
  }
}

/*
///////////////////////////////////////////////////////////
/// Class Dispose Plane
///////////////////////////////////////////////////////////
void Plane::Dispose(VALUE id) {
  if (Plane::Get(id)->viewport != Qnil) {
    Viewport::Get(Plane::Get(id)->viewport)->RemoveZObj(id);
  } else {
    Graphics::RemoveZObj(id);
  }
  delete Graphics::drawable_map[id];
  std::map<VALUE, Drawable*>::iterator it = Graphics::drawable_map.find(id);
  Graphics::drawable_map.erase(it);
}
*/

///////////////////////////////////////////////////////////
/// Refresh Bitmaps
///////////////////////////////////////////////////////////
void Plane::RefreshBitmaps() {

}

///////////////////////////////////////////////////////////
/// Draw
///////////////////////////////////////////////////////////
void Plane::Draw(long /* z */) {
  if (not visible or not bitmap) return;

  Texture const& tex = Graphics::GetTexture(bitmap);

  glEnable(GL_TEXTURE_2D);

  tex.Bind();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  float rectw, recth;

  if (viewport_) {
    Rect rect = viewport_->GetViewportRect();

    glEnable(GL_SCISSOR_TEST);
    glScissor(rect.x, Player::GetHeight() - (rect.y + rect.height), rect.width, rect.height);

    rectw = (float)rect.width;
    recth = (float)rect.height;

    glTranslatef((float)rect.x, (float)rect.y, 0.0f);
  } else {
    rectw = (float)Player::GetWidth();
    recth = (float)Player::GetHeight();
  }

  glColor4f(1.0f, 1.0f, 1.0f, opacity / 255.0f);

  glEnable(GL_BLEND);
  switch (blend_type) {
  case 1:
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    break;
  case 2:
    glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE_MINUS_SRC_COLOR);
    break;
  default:
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }
  float bmpw = bitmap->GetWidth() * zoom_x;
  float bmph = bitmap->GetHeight() * zoom_y;
  int r_ox = -ox % (int)bmpw;
  int r_oy = -oy % (int)bmph;
  float tilesx = ceil(rectw / bmpw) + ceil(r_ox / bmpw);
  float tilesy = ceil(recth / bmph) + ceil(r_oy / bmph);
  while (r_ox > 0) r_ox -= (int)bmpw;
  while (r_oy > 0) r_oy -= (int)bmph;
  glBegin(GL_QUADS);
    for (float i = r_ox / bmpw; i < tilesx; i++) {
      for (float j = r_oy / bmph; j < tilesy; j++) {
        glTexCoord2f(0.0f, 0.0f); glVertex2f(i * bmpw, j * bmph);
        glTexCoord2f(1.0f, 0.0f); glVertex2f((i + 1) * bmpw, j * bmph);
        glTexCoord2f(1.0f, 1.0f); glVertex2f((i + 1) * bmpw, (j + 1) * bmph);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(i * bmpw, (j + 1) * bmph);
      }
    }
  glEnd();

  glDisable(GL_SCISSOR_TEST);
}

///////////////////////////////////////////////////////////
/// Properties
///////////////////////////////////////////////////////////
ViewportRef const& Plane::viewport() const { return viewport_; }
void Plane::viewport(ViewportRef const& nviewport) {
  if (viewport_ != nviewport) {
    if (nviewport) {
      Graphics::RemoveZObj(*this);
      nviewport->RegisterZObj(0, *this);
    } else {
      if (viewport_) viewport_->RemoveZObj(*this);
      Graphics::RegisterZObj(0, *this);
    }
  }
  viewport_ = nviewport;
}
int Plane::z() const {
  return z_;
}
void Plane::z(int nz) {
  if (z_ != nz) {
    if (viewport_) {
      viewport_->UpdateZObj(*this, nz);
    } else {
      Graphics::UpdateZObj(*this, nz);
    }
  }
  z_ = nz;
}
