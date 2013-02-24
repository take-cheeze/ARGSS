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
#include <deque>
#include "time.hpp"
#include "graphics.h"
#include "config.h"
#include "player.h"
#include "output.h"
#include "sprite.h"
#include "tilemap.h"
#include "text.h"
#include "opengl.h"
#include "zobj.h"
#include "bitmap.h"
#include "texture.h"

///////////////////////////////////////////////////////////
// Global Variables
///////////////////////////////////////////////////////////
namespace {
int fps;
int framerate;
int framecount;
ColorRef backcolor;
int brightness;
double framerate_interval;
std::list<ZObj> zlist;
long creation;
long last_tics;
long last_tics_wait;
long next_tics_fps;
typedef std::map<Bitmap*, boost::shared_ptr<Texture> > texture_pool_type;
texture_pool_type texture_pool_;
}

int Graphics::GetFPS() {
  return fps;
}

int Graphics::get_creation_id() {
  creation += 1;
  return creation;
}

size_t UpdateTextures() {
  std::deque<Bitmap*> removing;
  size_t updated_count = 0;

  for(texture_pool_type::iterator i = texture_pool_.begin(); i != texture_pool_.end(); ++i) {
    if(i->second->expired()) { removing.push_back(i->first); }
    else if(i->second->Update()) { updated_count++; }
  }

  for(size_t i = 0; i < removing.size(); ++i) {
    texture_pool_.erase(removing[i]);
  }

  return updated_count;
}

Texture const& Graphics::GetTexture(BitmapRef const& bmp) {
  texture_pool_type::const_iterator it = texture_pool_.find(bmp.get());
  if(it == texture_pool_.end()) {
    return *texture_pool_.insert
             (std::make_pair(bmp.get(), boost::make_shared<Texture>(bmp))).first->second;
  } else { return *(it->second); }
}

///////////////////////////////////////////////////////////
/// Initialize
///////////////////////////////////////////////////////////
void Graphics::Init() {
  fps = 0;
  framerate = 40;
  framecount = 0;
  backcolor = boost::make_shared<Color>(0, 0, 0, 0);
  brightness = 255;
  creation = 0;
  framerate_interval = 1000.0 / framerate;
  last_tics = Time::GetTime() + (long)framerate_interval;
  next_tics_fps = Time::GetTime() + 1000;

  InitOpenGL();

  Text::Init();
}

///////////////////////////////////////////////////////////
/// Initialize OpengGL
///////////////////////////////////////////////////////////
void Graphics::InitOpenGL() {
  glViewport(0, 0, Player::GetWidth(), Player::GetHeight());
  glShadeModel(GL_FLAT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, Player::GetWidth(), Player::GetHeight(), 0, -1, 1);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glEnable(GL_BLEND);

  glClearColor(
        (GLclampf)(backcolor->red / 255.0f),
        (GLclampf)(backcolor->green / 255.0f),
        (GLclampf)(backcolor->blue / 255.0f),
        (GLclampf)(backcolor->alpha / 255.0f)
  );
  glClearDepth(1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  Player::SwapBuffers();
}

///////////////////////////////////////////////////////////
/// Exit
///////////////////////////////////////////////////////////
void Graphics::Exit() {
  zlist.clear();
  // Bitmap::DisposeBitmaps();
}

///////////////////////////////////////////////////////////
/// Refresh all graphic objects
///////////////////////////////////////////////////////////
void Graphics::RefreshAll() {
  for (std::list<ZObj>::iterator i = zlist.begin(); i != zlist.end(); i++) {
    // i->GetId()->RefreshBitmaps();
  }
  // Bitmap::RefreshBitmaps();
}

///////////////////////////////////////////////////////////
/// Wait
///////////////////////////////////////////////////////////
void Graphics::TimerWait(){
  last_tics_wait = Time::GetTime();
}

///////////////////////////////////////////////////////////
/// Continue
///////////////////////////////////////////////////////////
void Graphics::TimerContinue() {
  last_tics += Time::GetTime() - last_tics_wait;
  next_tics_fps += Time::GetTime() - last_tics_wait;
}

///////////////////////////////////////////////////////////
/// Update
///////////////////////////////////////////////////////////
void Graphics::Update() {
  static long tics;
  // static long tics_fps = Time::GetTime();
  static long frames = 0;
  // static double waitframes = 0;
  // static double cyclesleftover;

  Player::Update();
  /*if (waitframes >= 1) {
    waitframes -= 1;
    return;
  }*/
  tics = Time::GetTime();

  if ((tics - last_tics) >= framerate_interval) {// || (framerate_interval - tics + last_tics) < 12) {
    //cyclesleftover = waitframes;
    //waitframes = (double)(tics - last_tics) / framerate_interval - cyclesleftover;
    //last_tics += (tics - last_tics);
    last_tics = tics;

    DrawFrame();

    framecount++;
    frames++;

    if (tics >= next_tics_fps) {
      next_tics_fps += 1000;
      fps = frames;
      frames = 0;

      char title[255];
      sprintf(title, "%s - %d FPS", Config::Title.c_str(), fps);
      Player::main_window->SetTitle(title);
    }
  } else {
    Time::SleepMs((long)(framerate_interval) - (tics - last_tics));
  }
}

///////////////////////////////////////////////////////////
/// Draw Frame
///////////////////////////////////////////////////////////
void Graphics::DrawFrame() {
  glClear(GL_COLOR_BUFFER_BIT);

  for (std::list<ZObj>::iterator i = zlist.begin(); i != zlist.end(); i++) {
    i->GetId()->Draw(i->GetZ());
  }

  if (brightness < 255) {
    glDisable(GL_TEXTURE_2D);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor4f(0.0f, 0.0f, 0.0f, (float)(1.0f - brightness / 255.0f));
    glBegin(GL_QUADS);
      glVertex2i(0, 0);
      glVertex2i(0, Player::GetHeight());
      glVertex2i(Player::GetWidth(), Player::GetHeight());
      glVertex2i(Player::GetWidth(), 0);
    glEnd();
  }

  Player::SwapBuffers();
}

///////////////////////////////////////////////////////////
/// Freeze screen
///////////////////////////////////////////////////////////
void Graphics::Freeze() {
  // TODO
}

///////////////////////////////////////////////////////////
/// Transition effect
///////////////////////////////////////////////////////////
void Graphics::Transition(int /* duration */, std::string const& /* filename */, int /* vague */) {
  // TODO
}

///////////////////////////////////////////////////////////
/// Reset frames
///////////////////////////////////////////////////////////
void Graphics::FrameReset() {
  last_tics = Time::GetTime();
  next_tics_fps = Time::GetTime() + 1000;
}

///////////////////////////////////////////////////////////
/// Wait frames
///////////////////////////////////////////////////////////
void Graphics::Wait(int duration) {
  for (int i = duration; i > 0; i--) {
    Update();
  }
}

///////////////////////////////////////////////////////////
/// Resize screen
///////////////////////////////////////////////////////////
void Graphics::ResizeScreen(int width, int height) {
  Player::ResizeWindow(width, height);

  glViewport(0, 0, width, height);
  glShadeModel(GL_FLAT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, width, height, 0, -1, 1);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClear(GL_COLOR_BUFFER_BIT);

  Player::SwapBuffers();
}

///////////////////////////////////////////////////////////
/// Snap scree to bitmap
///////////////////////////////////////////////////////////
BitmapRef Graphics::SnapToBitmap() {
  return BitmapRef();
}

///////////////////////////////////////////////////////////
/// Fade out
///////////////////////////////////////////////////////////
void Graphics::FadeOut(int duration) {
  int n = brightness / duration;
  for (;duration > 0; duration--) {
    brightness -= n;
    Update();
  }
  if (brightness > 0) {
    brightness = 0;
    Update();
  }
}


///////////////////////////////////////////////////////////
/// Fade in
///////////////////////////////////////////////////////////
void Graphics::FadeIn(int duration) {
  int n = 255 / duration;
  for (;duration > 0; duration--) {
    brightness += n;
    Update();
  }
  if (brightness < 255) {
    brightness = 255;
    Update();
  }
}

///////////////////////////////////////////////////////////
/// Properties
///////////////////////////////////////////////////////////
int Graphics::GetFrameRate() {
  return framerate;
}
void Graphics::SetFrameRate(int nframerate) {
  framerate = nframerate;
  framerate_interval = 1000.0 / framerate;
}
int Graphics::GetFrameCount() {
  return framecount;
}
void Graphics::SetFrameCount(int nframecount) {
  framecount = nframecount;
}
ColorRef const& Graphics::GetBackColor() {
  return backcolor;
}
void Graphics::SetBackColor(ColorRef const& nbackcolor) {
  backcolor = nbackcolor;
  glClearColor(
        (GLclampf)(backcolor->red / 255.0f),
        (GLclampf)(backcolor->green / 255.0f),
        (GLclampf)(backcolor->blue / 255.0f),
        (GLclampf)(backcolor->alpha / 255.0f)
  );
}
int Graphics::GetBrightness() {
  return brightness;
}
void Graphics::SetBrightness(int nbrightness) {
  brightness = nbrightness;
}

///////////////////////////////////////////////////////////
/// Sort ZObj
///////////////////////////////////////////////////////////
bool Graphics::SortZObj(ZObj &first, ZObj &second) {
  return (first.GetZ() == second.GetZ())
      ? first.GetCreation() < second.GetCreation()
      : (first.GetZ() < second.GetZ());
}

///////////////////////////////////////////////////////////
/// Register ZObj
///////////////////////////////////////////////////////////
void Graphics::RegisterZObj(long z, Drawable& id) {
  ZObj zobj(z, get_creation_id(), id);

  zlist.push_back(zobj);
  zlist.sort(SortZObj);
}
void Graphics::RegisterZObj(long z, Drawable& id, bool /* multiz */) {
  ZObj zobj(z, 999999, id);
  zlist.push_back(zobj);
  zlist.sort(SortZObj);
}

///////////////////////////////////////////////////////////
/// Remove ZObj
///////////////////////////////////////////////////////////
struct remove_zobj_id : public std::binary_function<ZObj, ZObj, bool> {
  remove_zobj_id(Drawable& val) : id(&val) {}
  bool operator () (ZObj &obj) const {return obj.GetId() == id;}
  Drawable* const id;
};
void Graphics::RemoveZObj(Drawable& id) {
  zlist.remove_if (remove_zobj_id(id));
}

///////////////////////////////////////////////////////////
/// Update ZObj Z
///////////////////////////////////////////////////////////
void Graphics::UpdateZObj(Drawable& id, long z) {
  for (std::list<ZObj>::iterator i = zlist.begin(); i != zlist.end(); i++) {
    if (i->GetId() == &id) {
      i->SetZ(z);
      break;
    }
  }
  zlist.sort(SortZObj);
}
