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
#include <algorithm>
#include "window_xp.h"
#include "graphics.h"
#include "viewport.h"
#include "player.h"
#include "rect.h"
#include "texture.h"
#include "bitmap.h"

using std::max;
using std::min;

///////////////////////////////////////////////////////////
/// Constructor
///////////////////////////////////////////////////////////
Window::Window() {
  stretch = true;
  cursor_rect = boost::make_shared<Rect>(0, 0, 0, 0);
  active = true;
  visible = true;
  pause = false;
  x = 0;
  y = 0;
  width = 0;
  height = 0;
  ox = 0;
  oy = 0;
  opacity = 255;
  back_opacity = 255;
  contents_opacity = 255;
  cursor_frame = 0;
  pause_frame = 0;
  pause_id = 0;
}

///////////////////////////////////////////////////////////
/// Draw
///////////////////////////////////////////////////////////
void Window::Draw(long /* z */) {
  if (!visible) return;
  if (width <= 0 || height <= 0) return;
  if (x < -width || x > Player::GetWidth() || y < -height || y > Player::GetHeight()) return;

  glEnable(GL_TEXTURE_2D);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  if (viewport()) {
    Rect rect = viewport()->GetViewportRect();

    glEnable(GL_SCISSOR_TEST);
    glScissor(rect.x, Player::GetHeight() - (rect.y + rect.height), rect.width, rect.height);

    glTranslatef((float)rect.x, (float)rect.y, 0.0f);
  }

  glTranslatef((GLfloat)x, (GLfloat)y, 0.0f);

  if (windowskin) {
    Bitmap* bmp = windowskin.get();

    glPushMatrix();

    Graphics::GetTexture(windowskin).Bind();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    float bmpw = (float)bmp->GetWidth();
    float bmph = (float)bmp->GetHeight();
    float widthf = (float)width;
    float heightf = (float)height;

    // Background
    if (width > 4 && height > 4 && (back_opacity * opacity / 255 > 0)) {
      glColor4f(1.0f, 1.0f, 1.0f, (back_opacity * opacity / 255.0f) / 255.0f);
      if (stretch) {
        glBegin(GL_QUADS);
          glTexCoord2f(0.0f, 0.0f);          glVertex2f(2.0f, 2.0f);
          glTexCoord2f(128.0f / bmpw, 0.0f);      glVertex2f(widthf - 4.0f, 2.0f);
          glTexCoord2f(128.0f / bmpw, 128.0f / bmph); glVertex2f(widthf - 4.0f, heightf - 4.0f);
          glTexCoord2f(0.0f, 128.0f / bmph);      glVertex2f(2.0f, heightf - 4.0f);
        glEnd();
      } else {
        glEnable(GL_SCISSOR_TEST);

        Rect dstrect(x + 2, y + 2, width - 4, height - 4);

        if (viewport()) {
          Rect rect = viewport()->GetViewportRect();

          dstrect.x -= rect.x;
          dstrect.y -= rect.y;

          dstrect.Adjust(rect.width, rect.height);
        }
        //glScissor(x + 2, Player::GetHeight() - (y + 2 + height - 4), width - 4, height - 4);
        glScissor(dstrect.x, Player::GetHeight() - (dstrect.y + dstrect.height), dstrect.width, dstrect.height);

        float tilesx = ceil(widthf / 128.0f);
        float tilesy = ceil(heightf / 128.0f);
        for (float i = 0; i < tilesx; i++) {
          for (float j = 0; j < tilesy; j++) {
            glBegin(GL_QUADS);
              glTexCoord2f(0.0f, 0.0f);          glVertex2f(2.0f + i * 128.0f, 2.0f + j * 128.0f);
              glTexCoord2f(128.0f / bmpw, 0.0f);      glVertex2f(2.0f + (i + 1) * 128.0f, 2.0f + j * 128.0f);
              glTexCoord2f(128.0f / bmpw, 128.0f / bmph); glVertex2f(2.0f + (i + 1) * 128.0f, 2.0f + (j + 1) * 128.0f);
              glTexCoord2f(0.0f, 128.0f / bmph);      glVertex2f(2.0f + i * 128.0f, 2.0f + (j + 1) * 128.0f);
            glEnd();
          }
        }
        if (viewport()) glDisable(GL_SCISSOR_TEST);
      }
    }

    // Frame
    if (width > 0 && height > 0 && opacity > 0) {
      glColor4f(1.0f, 1.0f, 1.0f, opacity / 255.0f);

      glBegin(GL_QUADS);
        // Corner upper left
        glTexCoord2f(128.0f / bmpw, 0.0f);      glVertex2f(0.0f, 0.0f);
        glTexCoord2f(144.0f / bmpw, 0.0f);      glVertex2f(16.0f, 0.0f);
        glTexCoord2f(144.0f / bmpw, 16.0f / bmph);  glVertex2f(16.0f, 16.0f);
        glTexCoord2f(128.0f / bmpw, 16.0f / bmph);  glVertex2f(0.0f, 16.0f);
        // Corner upper right
        glTexCoord2f(176.0f / bmpw, 0.0f);      glVertex2f(widthf - 16.0f, 0.0f);
        glTexCoord2f(192.0f / bmpw, 0.0f);      glVertex2f(widthf, 0.0f);
        glTexCoord2f(192.0f / bmpw, 16.0f / bmph);  glVertex2f(widthf, 16.0f);
        glTexCoord2f(176.0f / bmpw, 16.0f / bmph);  glVertex2f(widthf - 16.0f, 16.0f);
        // Corner lower right
        glTexCoord2f(176.0f / bmpw, 48.0f / bmph);  glVertex2f(widthf - 16.0f, height - 16.0f);
        glTexCoord2f(192.0f / bmpw, 48.0f / bmph);  glVertex2f(widthf, heightf - 16.0f);
        glTexCoord2f(192.0f / bmpw, 64.0f / bmph);  glVertex2f(widthf, heightf);
        glTexCoord2f(176.0f / bmpw, 64.0f / bmph);  glVertex2f(widthf - 16.0f, heightf);
        // Corner lower left
        glTexCoord2f(128.0f / bmpw, 48.0f / bmph);  glVertex2f(0.0f, heightf - 16.0f);
        glTexCoord2f(144.0f / bmpw, 48.0f / bmph);  glVertex2f(16.0f, heightf - 16.0f);
        glTexCoord2f(144.0f / bmpw, 64.0f / bmph);  glVertex2f(16.0f, heightf);
        glTexCoord2f(128.0f / bmpw, 64.0f / bmph);  glVertex2f(0.0f, heightf);
        // Border up
        glTexCoord2f(144.0f / bmpw, 0.0f);      glVertex2f(16.0f, 0.0f);
        glTexCoord2f(176.0f / bmpw, 0.0f);      glVertex2f(max(widthf - 16.0f, 1.0f), 0.0f);
        glTexCoord2f(176.0f / bmpw, 16.0f / bmph);  glVertex2f(max(widthf - 16.0f, 1.0f), 16.0f);
        glTexCoord2f(144.0f / bmpw, 16.0f / bmph);  glVertex2f(16.0f, 16.0f);
        // Border down
        glTexCoord2f(144.0f / bmpw, 48.0f / bmph);  glVertex2f(16.0f, height - 16.0f);
        glTexCoord2f(176.0f / bmpw, 48.0f / bmph);  glVertex2f(max(widthf - 16.0f, 1.0f), heightf - 16.0f);
        glTexCoord2f(176.0f / bmpw, 64.0f / bmph);  glVertex2f(max(widthf - 16.0f, 1.0f), heightf);
        glTexCoord2f(144.0f / bmpw, 64.0f / bmph);  glVertex2f(16.0f, heightf);
        // Border left
        glTexCoord2f(128.0f / bmpw, 16.0f / bmph);  glVertex2f(0.0f, 16.0f);
        glTexCoord2f(144.0f / bmpw, 16.0f / bmph);  glVertex2f(16.0f, 16.0f);
        glTexCoord2f(144.0f / bmpw, 48.0f / bmph);  glVertex2f(16.0f, max(heightf - 16.0f, 1.0f));
        glTexCoord2f(128.0f / bmpw, 48.0f / bmph);  glVertex2f(0.0f, max(heightf - 16.0f, 1.0f));
        // Border right
        glTexCoord2f(176.0f / bmpw, 16.0f / bmph);  glVertex2f(widthf - 16.0f, 16.0f);
        glTexCoord2f(192.0f / bmpw, 16.0f / bmph);  glVertex2f(widthf, 16.0f);
        glTexCoord2f(192.0f / bmpw, 48.0f / bmph);  glVertex2f(widthf, max(heightf - 16.0f, 1.0f));
        glTexCoord2f(176.0f / bmpw, 48.0f / bmph);  glVertex2f(widthf - 16.0f, max(heightf - 16.0f, 1.0f));
      glEnd();
    }

    // Cursor
    if (width > 32 && height > 32) {
      Rect rect(*cursor_rect);
      if (rect.width > 0 && rect.height > 0) {
        float cursor_opacity = 255.0f;
        if (cursor_frame <= 16) {
          cursor_opacity -= (92.0f / 16.0f) * cursor_frame;
        } else {
          cursor_opacity -= (92.0f / 16.0f) * (32 - cursor_frame);
        }
        glColor4f(1.0f, 1.0f, 1.0f, cursor_opacity / 255.0f);

        float left = (float)rect.x + 16;
        float top = (float)rect.y + 16;
        float rigth = left + rect.width;
        float bottom = top + rect.height;

        glBegin(GL_QUADS);
          // Background
          glTexCoord2f(130.0f / bmpw, 66.0f / bmph);  glVertex2f(left + 2.0f, top + 2.0f);
          glTexCoord2f(158.0f / bmpw, 66.0f / bmph);  glVertex2f(rigth - 2.0f, top + 2.0f);
          glTexCoord2f(158.0f / bmpw, 94.0f / bmph);  glVertex2f(rigth - 2.0f, bottom - 2.0f);
          glTexCoord2f(130.0f / bmpw, 94.0f / bmph);  glVertex2f(left + 2.0f, bottom - 2.0f);
          // Corner upper left
          glTexCoord2f(128.0f / bmpw, 64.0f / bmph);  glVertex2f(left, top);
          glTexCoord2f(130.0f / bmpw, 64.0f / bmph);  glVertex2f(left + 2.0f, top);
          glTexCoord2f(130.0f / bmpw, 66.0f / bmph);  glVertex2f(left + 2.0f, top + 2.0f);
          glTexCoord2f(128.0f / bmpw, 66.0f / bmph);  glVertex2f(left, top + 2.0f);
          // Corner upper right
          glTexCoord2f(158.0f / bmpw, 64.0f / bmph);  glVertex2f(rigth - 2.0f, top);
          glTexCoord2f(160.0f / bmpw, 64.0f / bmph);  glVertex2f(rigth, top);
          glTexCoord2f(160.0f / bmpw, 66.0f / bmph);  glVertex2f(rigth, top + 2.0f);
          glTexCoord2f(158.0f / bmpw, 66.0f / bmph);  glVertex2f(rigth - 2.0f, top + 2.0f);
          // Corner lower right
          glTexCoord2f(158.0f / bmpw, 94.0f / bmph);  glVertex2f(rigth - 2.0f, bottom - 2.0f);
          glTexCoord2f(160.0f / bmpw, 94.0f / bmph);  glVertex2f(rigth, bottom - 2.0f);
          glTexCoord2f(160.0f / bmpw, 96.0f / bmph);  glVertex2f(rigth, bottom);
          glTexCoord2f(158.0f / bmpw, 96.0f / bmph);  glVertex2f(rigth - 2.0f, bottom);
          // Corner lower left
          glTexCoord2f(128.0f / bmpw, 94.0f / bmph);  glVertex2f(left, bottom - 2.0f);
          glTexCoord2f(130.0f / bmpw, 94.0f / bmph);  glVertex2f(left + 2.0f, bottom - 2.0f);
          glTexCoord2f(130.0f / bmpw, 96.0f / bmph);  glVertex2f(left + 2.0f, bottom);
          glTexCoord2f(128.0f / bmpw, 96.0f / bmph);  glVertex2f(left, bottom);
          // Border up
          glTexCoord2f(130.0f / bmpw, 64.0f / bmph);  glVertex2f(left + 2.0f, top);
          glTexCoord2f(158.0f / bmpw, 64.0f / bmph);  glVertex2f(rigth - 2.0f, top);
          glTexCoord2f(158.0f / bmpw, 66.0f / bmph);  glVertex2f(rigth - 2.0f, top + 2.0f);
          glTexCoord2f(130.0f / bmpw, 66.0f / bmph);  glVertex2f(left + 2.0f, top + 2.0f);
          // Border down
          glTexCoord2f(130.0f / bmpw, 94.0f / bmph);  glVertex2f(left + 2.0f, bottom - 2.0f);
          glTexCoord2f(158.0f / bmpw, 94.0f / bmph);  glVertex2f(rigth - 2.0f, bottom - 2.0f);
          glTexCoord2f(158.0f / bmpw, 96.0f / bmph);  glVertex2f(rigth - 2.0f, bottom);
          glTexCoord2f(130.0f / bmpw, 96.0f / bmph);  glVertex2f(left + 2.0f, bottom);
          // Border left
          glTexCoord2f(128.0f / bmpw, 66.0f / bmph);  glVertex2f(left, top + 2.0f);
          glTexCoord2f(130.0f / bmpw, 66.0f / bmph);  glVertex2f(left + 2.0f, top + 2.0f);
          glTexCoord2f(130.0f / bmpw, 94.0f / bmph);  glVertex2f(left + 2.0f, bottom - 2.0f);
          glTexCoord2f(128.0f / bmpw, 94.0f / bmph);  glVertex2f(left, bottom - 2.0f);
          // Border right
          glTexCoord2f(158.0f / bmpw, 66.0f / bmph);  glVertex2f(rigth - 2.0f, top + 2.0f);
          glTexCoord2f(160.0f / bmpw, 66.0f / bmph);  glVertex2f(rigth, top + 2.0f);
          glTexCoord2f(160.0f / bmpw, 94.0f / bmph);  glVertex2f(rigth, bottom - 2.0f);
          glTexCoord2f(158.0f / bmpw, 94.0f / bmph);  glVertex2f(rigth - 2.0f, bottom - 2.0f);
        glEnd();
      }
    }
    if (pause) {
      float dstx = (float)max(width / 2 - 8, 0);
      float dsty = (float)max(height - 16, 0);
      float w = (float)min(16, width);
      float h = (float)min(16, height);
      float srcx = 176.0f;
      float srcy = 64.0f;
      switch (pause_id) {
      case 0:
        glColor4f(1.0f, 1.0f, 1.0f, (255.0f / 8.0f) * pause_frame / 255.0f);
      case 4:
        srcx = 160.0f;
        srcy = 64.0f;
        break;
      case 1:
        srcx = 176.0f;
        srcy = 64.0f;
        break;
      case 2:
        srcx = 160.0f;
        srcy = 80.0f;
        break;
      case 3:
        srcx = 176.0f;
        srcy = 80.0f;
      }
      glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
      glBegin(GL_QUADS);
        glTexCoord2f(srcx / bmpw, srcy / bmph);        glVertex2f(dstx, dsty);
        glTexCoord2f((srcx + w) / bmpw, srcy / bmph);    glVertex2f(dstx + w, dsty);
        glTexCoord2f((srcx + w) / bmpw, (srcy + h) / bmph);  glVertex2f(dstx + w, dsty + h);
        glTexCoord2f(srcx / bmpw, (srcy + h) / bmph);    glVertex2f(dstx, dsty + h);
      glEnd();
    }
  }

  if (contents) {
    if (width > 32 && height > 32 && -ox < width - 32 && -oy < height - 32 && contents_opacity > 0) {
      Bitmap* bmp = contents.get();

      glPushMatrix();

      Graphics::GetTexture(contents).Bind();

      glMatrixMode(GL_MODELVIEW);
      glPopMatrix();

      Rect dstrect(x + 16, y + 16, width - 32, height - 32);

      glEnable(GL_SCISSOR_TEST);
      if (viewport()) {
        Rect rect = viewport()->GetViewportRect();

        dstrect.x -= rect.x;
        dstrect.y -= rect.y;

        dstrect.Adjust(rect.width, rect.height);
      }

      glScissor(dstrect.x, Player::GetHeight() - (dstrect.y + dstrect.height), dstrect.width, dstrect.height);

      glColor4f(1.0f, 1.0f, 1.0f, contents_opacity / 255.0f);

      glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);  glVertex2f(16.0f - ox, 16.0f - oy);
        glTexCoord2f(1.0f, 0.0f);  glVertex2f(16.0f - ox + bmp->GetWidth(), 16.0f - oy);
        glTexCoord2f(1.0f, 1.0f);  glVertex2f(16.0f - ox + bmp->GetWidth(), 16.0f - oy + bmp->GetHeight());
        glTexCoord2f(0.0f, 1.0f);  glVertex2f(16.0f - ox, 16.0f - oy + bmp->GetHeight());
      glEnd();

      glViewport(0, 0, Player::GetWidth(), Player::GetHeight());

      if (viewport()) glDisable(GL_SCISSOR_TEST);
    }

    if (windowskin) {
      Bitmap* bmp = windowskin.get();

      float bmpw = (float)bmp->GetWidth();
      float bmph = (float)bmp->GetHeight();
      float widthf = (float)width;
      float heightf = (float)height;

      Graphics::GetTexture(windowskin).Bind();

      glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

      glBegin(GL_QUADS);
        if (ox > 0) {
          glTexCoord2f(144.0f / bmpw, 24.0f / bmph);  glVertex2f(4.0f, heightf / 2.0f - 8.0f);
          glTexCoord2f(152.0f / bmpw, 24.0f / bmph);  glVertex2f(12.0f, heightf / 2.0f - 8.0f);
          glTexCoord2f(152.0f / bmpw, 40.0f / bmph);  glVertex2f(12.0f, heightf / 2.0f + 8.0f);
          glTexCoord2f(144.0f / bmpw, 40.0f / bmph);  glVertex2f(4.0f, heightf / 2.0f + 8.0f);
        }
        if (oy > 0) {
          glTexCoord2f(152.0f / bmpw, 16.0f / bmph);  glVertex2f(widthf / 2.0f - 8.0f, 4.0f);
          glTexCoord2f(168.0f / bmpw, 16.0f / bmph);  glVertex2f(widthf / 2.0f + 8.0f, 4.0f);
          glTexCoord2f(168.0f / bmpw, 24.0f / bmph);  glVertex2f(widthf / 2.0f + 8.0f, 12.0f);
          glTexCoord2f(152.0f / bmpw, 24.0f / bmph);  glVertex2f(widthf / 2.0f - 8.0f, 12.0f);
        }
        if (contents->GetWidth() - ox > width - 32) {
          glTexCoord2f(168.0f / bmpw, 24.0f / bmph);  glVertex2f(widthf - 12.0f, heightf / 2.0f - 8.0f);
          glTexCoord2f(176.0f / bmpw, 24.0f / bmph);  glVertex2f(widthf - 4.0f, heightf / 2.0f - 8.0f);
          glTexCoord2f(176.0f / bmpw, 40.0f / bmph);  glVertex2f(widthf - 4.0f, heightf / 2.0f + 8.0f);
          glTexCoord2f(168.0f / bmpw, 40.0f / bmph);  glVertex2f(widthf - 12.0f, heightf / 2.0f + 8.0f);
        }
        if (contents->GetHeight() - oy > height - 32) {
          glTexCoord2f(152.0f / bmpw, 40.0f / bmph);  glVertex2f(widthf / 2.0f - 8.0f, heightf - 12.0f);
          glTexCoord2f(168.0f / bmpw, 40.0f / bmph);  glVertex2f(widthf / 2.0f + 8.0f, heightf - 12.0f);
          glTexCoord2f(168.0f / bmpw, 48.0f / bmph);  glVertex2f(widthf / 2.0f + 8.0f, heightf - 4.0f);
          glTexCoord2f(152.0f / bmpw, 48.0f / bmph);  glVertex2f(widthf / 2.0f - 8.0f, heightf - 4.0f);
        }
      glEnd();
    }
  }

  glDisable(GL_SCISSOR_TEST);
}

///////////////////////////////////////////////////////////
/// Update
///////////////////////////////////////////////////////////
void Window::Update() {
  if (active) {
    cursor_frame += 1;
    if (cursor_frame == 32) {
      cursor_frame = 0;
    }
    if (pause) {
      pause_frame += 1;
      if (pause_frame == 8) {
        pause_frame = 0;
        pause_id += 1;
        if (pause_id == 5) {
          pause_id = 1;
        }
      }
    }
  }
}
