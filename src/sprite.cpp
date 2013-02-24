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
#include <algorithm>
#include <utility>

#include "sprite.h"
#include "viewport.h"
#include "player.h"
#include "graphics.h"
#include "config.h"
#include "rect.h"
#include "bitmap.h"
#include "texture.h"

///////////////////////////////////////////////////////////
/// Constructor
///////////////////////////////////////////////////////////
Sprite::Sprite() {
  src_rect = boost::make_shared<Rect>(0, 0, 0, 0);
  visible = true;
  x = 0;
  y = 0;
  z_ = 0;
  ox = 0;
  oy = 0;
  zoom_x = 1.0;
  zoom_y = 1.0;
  angle = 0;
  flipx = false;
  flipy = false;
  bush_depth = 0;
  opacity = 255;
  blend_type = 0;
  color = boost::make_shared<Color>(0, 0, 0, 255);
  tone_ = boost::make_shared<Tone>(0, 0, 0, 0);
  flash_duration = 0;
  flash_needs_refresh = false;

  if (viewport_) {
    viewport_->RegisterZObj(0, *this);
  } else {
    Graphics::RegisterZObj(0, *this);
  }
}

///////////////////////////////////////////////////////////
/// Destructor
///////////////////////////////////////////////////////////
Sprite::~Sprite() {
  if (flash_texture > 0) {
    glDeleteTextures(1, &flash_texture);
    flash_texture = 0;
  }
}

/*
///////////////////////////////////////////////////////////
/// Class Dispose Sprite
///////////////////////////////////////////////////////////
void Sprite::Dispose(VALUE id) {
  if (Sprite::Get(id)->viewport != Qnil) {
    Viewport::Get(Sprite::Get(id)->viewport)->RemoveZObj(id);
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
void Sprite::RefreshBitmaps() {
  if (sprite != NULL) sprite->Changed();
}

///////////////////////////////////////////////////////////
/// Draw
///////////////////////////////////////////////////////////
void Sprite::Draw(long /* z */) {
  if (not visible or not bitmap_) return;

  src_rect_sprite = *src_rect;

  int width = src_rect_sprite.width;
  int height = src_rect_sprite.height;
  if (width <= 0 || height <= 0)
  if (x < -width || x > Player::GetWidth() || y < -height || y > Player::GetHeight()) return;
  if (zoom_x == 0 || zoom_y == 0 || opacity == 0) return;
  if (flash_duration > 0 && flash_color.red == -1) return;

  Refresh();

  glEnable(GL_TEXTURE_2D);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  Graphics::GetTexture(sprite).Bind();

  glTranslatef((float)x, (float)y, 0.0f);

  if (viewport_) {
    Rect rect = viewport_->GetViewportRect();

    glEnable(GL_SCISSOR_TEST);
    glScissor(rect.x, Player::GetHeight() - (rect.y + rect.height), rect.width, rect.height);

    glTranslatef((float)rect.x, (float)rect.y, 0.0f);
  }

  glRotatef((float)angle, 0.0f, 0.0f, 1.0f);
  glTranslatef((float)-ox * zoom_x, (float)-oy * zoom_y, 0.0f);

  glColor4f(1.0f, 1.0f, 1.0f, opacity / 255.0f);

  float corners[4][2] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};
  if (src_rect_sprite != sprite->GetRect()) {
    float rx = (float)src_rect_sprite.x / (float)sprite->GetWidth();
    float ry = (float)src_rect_sprite.y / (float)sprite->GetHeight();
    float rw = (float)src_rect_sprite.width / (float)sprite->GetWidth();
    float rh = (float)src_rect_sprite.height / (float)sprite->GetHeight();
    corners[0][0] = rx;      corners[0][1] = ry;
    corners[1][0] = rx + rw;  corners[1][1] = ry;
    corners[2][0] = rx + rw;  corners[2][1] = ry + rh;
    corners[3][0] = rx;      corners[3][1] = ry + rh;
  }
  if (flipx && flipy) {
    corners[0][0] = 1.0f - corners[0][0]; corners[0][1] = 1.0f - corners[0][1];
    corners[1][0] = 1.0f - corners[1][0]; corners[1][1] = 1.0f - corners[1][1];
    corners[2][0] = 1.0f - corners[2][0]; corners[2][1] = 1.0f - corners[2][1];
    corners[3][0] = 1.0f - corners[3][0]; corners[3][1] = 1.0f - corners[3][1];
  } else if (flipx) {
    corners[0][0] = 1.0f - corners[0][0];
    corners[1][0] = 1.0f - corners[1][0];
    corners[2][0] = 1.0f - corners[2][0];
    corners[3][0] = 1.0f - corners[3][0];
  } else if (flipy) {
    corners[0][1] = 1.0f - corners[0][1];
    corners[1][1] = 1.0f - corners[1][1];
    corners[2][1] = 1.0f - corners[2][1];
    corners[3][1] = 1.0f - corners[3][1];
  }

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

  if (bush_depth == 0) {
    glBegin(GL_QUADS);
      glTexCoord2f(corners[0][0], corners[0][1]); glVertex2f(0.0f, 0.0f);
      glTexCoord2f(corners[1][0], corners[1][1]); glVertex2f(width * zoom_x, 0.0f);
      glTexCoord2f(corners[2][0], corners[2][1]); glVertex2f(width * zoom_x, height * zoom_y);
      glTexCoord2f(corners[3][0], corners[3][1]); glVertex2f(0.0f, height * zoom_y);
    glEnd();
  } else {
    if (flipy) {
      glBegin(GL_QUADS);
        glTexCoord2f(corners[0][0], corners[0][1] - bush_depth / (float)sprite->GetHeight());  glVertex2f(0.0f, bush_depth * zoom_y);
        glTexCoord2f(corners[1][0], corners[1][1] - bush_depth / (float)sprite->GetHeight());  glVertex2f(width * zoom_x, bush_depth * zoom_y);
        glTexCoord2f(corners[2][0], corners[2][1]);                        glVertex2f(width * zoom_x, height * zoom_y);
        glTexCoord2f(corners[3][0], corners[3][1]);                        glVertex2f(0.0f, height * zoom_y);
      glEnd();

      glColor4f(1.0f, 1.0f, 1.0f, 0.5f * (opacity / 255.0f));
      glBegin(GL_QUADS);
        glTexCoord2f(corners[0][0], corners[0][1]);                        glVertex2f(0.0f, 0.0f);
        glTexCoord2f(corners[1][0], corners[1][1]);                        glVertex2f(width * zoom_x, 0.0f);
        glTexCoord2f(corners[2][0], corners[0][1] - bush_depth / (float)sprite->GetHeight());  glVertex2f(width * zoom_x, bush_depth * zoom_y);
        glTexCoord2f(corners[3][0], corners[1][1] - bush_depth / (float)sprite->GetHeight());  glVertex2f(0.0f, bush_depth * zoom_y);
      glEnd();
    } else {
      glBegin(GL_QUADS);
        glTexCoord2f(corners[0][0], corners[0][1]);                        glVertex2f(0.0f, 0.0f);
        glTexCoord2f(corners[1][0], corners[1][1]);                        glVertex2f(width * zoom_x, 0.0f);
        glTexCoord2f(corners[2][0], corners[2][1] - bush_depth / (float)sprite->GetHeight());  glVertex2f(width * zoom_x, (height - bush_depth) * zoom_y);
        glTexCoord2f(corners[3][0], corners[3][1] - bush_depth / (float)sprite->GetHeight());  glVertex2f(0.0f, (height - bush_depth) * zoom_y);
      glEnd();

      glColor4f(1.0f, 1.0f, 1.0f, 0.5f * (opacity / 255.0f));
      glBegin(GL_QUADS);
        glTexCoord2f(corners[0][0], corners[2][1] - bush_depth / (float)sprite->GetHeight());  glVertex2f(0.0f, (height - bush_depth) * zoom_y);
        glTexCoord2f(corners[1][0], corners[3][1] - bush_depth / (float)sprite->GetHeight());  glVertex2f(width * zoom_x, (height - bush_depth) * zoom_y);
        glTexCoord2f(corners[2][0], corners[2][1]);                        glVertex2f(width * zoom_x, height * zoom_y);
        glTexCoord2f(corners[3][0], corners[3][1]);                        glVertex2f(0.0f, height * zoom_y);
      glEnd();
    }
  }

  if (flash_duration > 0) {
    glBindTexture(GL_TEXTURE_2D, flash_texture);
    GLfloat alpha = ((float)flash_color.alpha / 255.0f) * (1.0f - flash_frame / (float)flash_duration);
    glColor4f((float)flash_color.red / 255.0f, (GLfloat)flash_color.green / 255.0f, (float)flash_color.blue / 255.0f, alpha);
    glBegin(GL_QUADS);
      glTexCoord2f(corners[0][0], corners[0][1]); glVertex2f(0.0f, 0.0f);
      glTexCoord2f(corners[1][0], corners[1][1]); glVertex2f(width * zoom_x, 0.0f);
      glTexCoord2f(corners[2][0], corners[2][1]); glVertex2f(width * zoom_x, height * zoom_y);
      glTexCoord2f(corners[3][0], corners[3][1]); glVertex2f(0.0f, height * zoom_y);
    glEnd();
  }

  glDisable(GL_SCISSOR_TEST);
  /*if (!visible) return;
  if (bitmap == Qnil) return;

  src_rect_sprite = Rect(src_rect);

  int width = src_rect_sprite.width;
  int height = src_rect_sprite.height;
  if (width <= 0 || height <= 0)
  if (x < -width || x > Player::GetWidth() || y < -height || y > Player::GetHeight()) return;
  if (zoom_x == 0 || zoom_y == 0 || opacity == 0) return;
  if (flash_duration > 0 && flash_color.red == -1) return;

  Refresh();

  glEnable(GL_TEXTURE_2D);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  sprite->BindBitmap();

  glTranslatef((float)x, (float)y, 0.0f);

  if (viewport != Qnil) {
    Rect rect = Viewport::Get(viewport)->GetViewportRect();

    glEnable(GL_SCISSOR_TEST);
    glScissor(rect.x, Player::GetHeight() - (rect.y + rect.height), rect.width, rect.height);

    glTranslatef((float)rect.x, (float)rect.y, 0.0f);
  }

  glRotatef((float)angle, 0.0f, 0.0f, 1.0f);
  glTranslatef((float)-ox * zoom_x, (float)-oy * zoom_y, 0.0f);

  glColor4f(1.0f, 1.0f, 1.0f, opacity / 255.0f);

  float corners[4][2] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};
  if (flipx && flipy) {
    corners[0][0] = 1.0f; corners[0][1] = 1.0f;
    corners[1][0] = 0.0f; corners[1][1] = 1.0f;
    corners[2][0] = 0.0f; corners[2][1] = 0.0f;
    corners[3][0] = 1.0f; corners[3][1] = 0.0f;
  } else if (flipx) {
    corners[0][0] = 1.0f; corners[0][1] = 0.0f;
    corners[1][0] = 0.0f; corners[1][1] = 0.0f;
    corners[2][0] = 0.0f; corners[2][1] = 1.0f;
    corners[3][0] = 1.0f; corners[3][1] = 1.0f;
  } else if (flipy) {
    corners[0][0] = 0.0f; corners[0][1] = 1.0f;
    corners[1][0] = 1.0f; corners[1][1] = 1.0f;
    corners[2][0] = 1.0f; corners[2][1] = 0.0f;
    corners[3][0] = 0.0f; corners[3][1] = 0.0f;
  }

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

  if (bush_depth == 0) {
    glBegin(GL_QUADS);
      glTexCoord2f(corners[0][0], corners[0][1]); glVertex2f(0.0f, 0.0f);
      glTexCoord2f(corners[1][0], corners[1][1]); glVertex2f(width * zoom_x, 0.0f);
      glTexCoord2f(corners[2][0], corners[2][1]); glVertex2f(width * zoom_x, height * zoom_y);
      glTexCoord2f(corners[3][0], corners[3][1]); glVertex2f(0.0f, height * zoom_y);
    glEnd();
  } else {
    glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 0.0f);                glVertex2f(0.0f, 0.0f);
      glTexCoord2f(1.0f, 0.0f);                glVertex2f(width * zoom_x, 0.0f);
      glTexCoord2f(1.0f, 1.0f - bush_depth / (float)height);  glVertex2f(width * zoom_x, (height - bush_depth) * zoom_y);
      glTexCoord2f(0.0f, 1.0f - bush_depth / (float)height);  glVertex2f(0.0f, (height - bush_depth) * zoom_y);
    glEnd();

    glColor4f(1.0f, 1.0f, 1.0f, 0.5f * (opacity / 255.0f));
    glBegin(GL_QUADS);
      glTexCoord2f(0.0f, (1.0f - bush_depth / (float)height));  glVertex2f(0.0f, (height - bush_depth) * zoom_y);
      glTexCoord2f(1.0f, (1.0f - bush_depth / (float)height));  glVertex2f(width * zoom_x, (height - bush_depth) * zoom_y);
      glTexCoord2f(1.0f, 1.0f);                  glVertex2f(width * zoom_x, height * zoom_y);
      glTexCoord2f(0.0f, 1.0f);                  glVertex2f(0.0f, height * zoom_y);
    glEnd();
  }

  if (flash_duration > 0) {
    glBindTexture(GL_TEXTURE_2D, flash_texture);
    GLfloat alpha = ((float)flash_color.alpha / 255.0f) * (1.0f - flash_frame / (float)flash_duration);
    glColor4f((float)flash_color.red / 255.0f, (GLfloat)flash_color.green / 255.0f, (float)flash_color.blue / 255.0f, alpha);
    glBegin(GL_QUADS);
      glTexCoord2f(corners[0][0], corners[0][1]); glVertex2f(0.0f, 0.0f);
      glTexCoord2f(corners[1][0], corners[1][1]); glVertex2f(width * zoom_x, 0.0f);
      glTexCoord2f(corners[2][0], corners[2][1]); glVertex2f(width * zoom_x, height * zoom_y);
      glTexCoord2f(corners[3][0], corners[3][1]); glVertex2f(0.0f, height * zoom_y);
    glEnd();
  }

  glDisable(GL_SCISSOR_TEST);*/
}

///////////////////////////////////////////////////////////
/// Refresh
///////////////////////////////////////////////////////////
void Sprite::Refresh() {
  if (needs_refresh) {
    needs_refresh = false;

    sprite = boost::make_shared<Bitmap>(*bitmap_, bitmap_->GetRect());

    sprite->ToneChange(*tone_);
  }
  if (flash_needs_refresh) {
    flash_needs_refresh = false;
    RefreshFlash();
  }

  /*Tone tone_sprite(tone);

  if (src_rect_sprite != src_rect_last) {
    src_rect_last = src_rect_sprite;
    if (tone_sprite.red != 0 || tone_sprite.green != 0 || tone_sprite.blue != 0 || tone_sprite.gray != 0) {
      needs_refresh = true;
    }
    flash_needs_refresh = true;
  }

  if (needs_refresh) {
    needs_refresh = false;

    if (sprite) delete sprite;

    sprite = new Bitmap(Bitmap::Get(bitmap), src_rect_sprite);

    sprite->ToneChange(tone_sprite);
  }
  if (flash_needs_refresh) {
    flash_needs_refresh = false;
    RefreshFlash();
  }

  sprite->Refresh();*/
}

///////////////////////////////////////////////////////////
/// Update
///////////////////////////////////////////////////////////
void Sprite::Update() {
  if (flash_duration != 0) {
    flash_frame += 1;
    if (flash_duration == flash_frame) {
      flash_duration = 0;
      if (flash_texture > 0) {
        glDeleteTextures(1, &flash_texture);
        flash_texture = 0;
      }
    }
  }
}

///////////////////////////////////////////////////////////
/// Flash
///////////////////////////////////////////////////////////
void Sprite::Flash(int duration){
  flash_color = Color(-1, -1, -1, -1);
  flash_duration = duration;
  flash_frame = 0;
}
void Sprite::Flash(Color color, int duration){
  flash_color = color;
  flash_duration = duration;
  flash_frame = 0;
  flash_needs_refresh = true;
}

void Sprite::RefreshFlash() {
  if (flash_texture > 0) glDeleteTextures(1, &flash_texture);

  glEnable(GL_TEXTURE_2D);
  glGenTextures(1, &flash_texture);
  glBindTexture(GL_TEXTURE_2D, flash_texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glPixelTransferf(GL_RED_BIAS, 255.0f);
  glPixelTransferf(GL_GREEN_BIAS, 255.0f);
  glPixelTransferf(GL_BLUE_BIAS, 255.0f);

  glTexImage2D(GL_TEXTURE_2D, 0, 4, sprite->GetWidth(), sprite->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, sprite->GetPixels());

  glPixelTransferf(GL_RED_BIAS, 0.0f);
  glPixelTransferf(GL_GREEN_BIAS, 0.0f);
  glPixelTransferf(GL_BLUE_BIAS, 0.0f);
}

///////////////////////////////////////////////////////////
/// Properties
///////////////////////////////////////////////////////////
void Sprite::viewport(ViewportRef const& v) {
  if (viewport_ != v) {
    if (v) {
      Graphics::RemoveZObj(*this);
      v->RegisterZObj(0, *this);
    } else {
      if (viewport_) viewport_->RemoveZObj(*this);
      Graphics::RegisterZObj(0, *this);
    }
  }
  viewport_ = v;
}

void Sprite::bitmap(BitmapRef const& bmp) {
  if (bitmap_ != bmp) {
    needs_refresh = true;
    flash_needs_refresh = true;
  }
  bitmap_ = bmp;
}

void Sprite::z(int nz) {
  if (z_ != nz) {
    if (viewport_) {
      viewport_->UpdateZObj(*this, nz);
    } else {
      Graphics::UpdateZObj(*this, nz);
    }
  }
  z_ = nz;
}
void Sprite::tone(ToneRef const& ntone) {
  if (*tone_ != *ntone) needs_refresh = true;
  tone_ = ntone;
}
