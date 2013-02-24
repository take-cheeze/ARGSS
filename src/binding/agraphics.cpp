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
#include "binding/agraphics.h"
#include "binding/acolor.h"
#include "graphics.h"
#include "config.h"

///////////////////////////////////////////////////////////
// ARGSS Graphics module methods
///////////////////////////////////////////////////////////
VALUE ARGSS::AGraphics::rupdate(VALUE /* self */) {
  Graphics::Update();
  return Qnil;
}
VALUE ARGSS::AGraphics::rfreeze(VALUE /* self */) {
  Graphics::Freeze();
  return Qnil;
}
VALUE ARGSS::AGraphics::rtransition(int argc, VALUE* argv, VALUE /* self */) {
  if (argc > 3) raise_argn(argc, 3);
  int duration = 8;
  std::string filename = "";
  int vague = 40;
  if (argc > 0) {
    Check_Kind(argv[0], rb_cNumeric);
    duration = NUM2INT(argv[0]);
  }
  if (argc > 1) {
    Check_Type(argv[1], T_STRING);
    filename = StringValuePtr(argv[1]);
  }
  if (argc > 2) {
    Check_Kind(argv[2], rb_cNumeric);
    vague = NUM2INT(argv[2]);
  }
  Graphics::Transition(duration, filename, vague);
  return Qnil;
}
VALUE ARGSS::AGraphics::rframe_reset(VALUE /* self */) {
  Graphics::FrameReset();
  return Qnil;
}
VALUE ARGSS::AGraphics::rframe_rate(VALUE /* self */) {
  return INT2FIX(Graphics::GetFrameRate());
}
VALUE ARGSS::AGraphics::rframe_rateE(VALUE /* self */, VALUE framerate) {
  Check_Kind(framerate, rb_cNumeric);
  Graphics::SetFrameRate(NUM2INT(framerate));
  return framerate;
}
VALUE ARGSS::AGraphics::rframe_count(VALUE /* self */) {
  return INT2FIX(Graphics::GetFrameCount());
}
VALUE ARGSS::AGraphics::rframe_countE(VALUE /* self */, VALUE framecount) {
  Check_Kind(framecount, rb_cNumeric);
  Graphics::SetFrameCount(NUM2INT(framecount));
  return framecount;
}
VALUE ARGSS::AGraphics::rfps(VALUE /* self */) {
  return INT2FIX(Graphics::GetFPS());
}
VALUE ARGSS::AGraphics::rbackcolor(VALUE /* self */) {
  return create(Graphics::GetBackColor());
}
VALUE ARGSS::AGraphics::rbackcolorE(VALUE /* self */, VALUE backcolor) {
  Check_Class(backcolor, ruby_class<Color>());
  Graphics::SetBackColor(get_ptr<Color>(backcolor));
  return backcolor;
}
VALUE ARGSS::AGraphics::rwait(VALUE /* self */, VALUE duration) {
  Check_Kind(duration, rb_cNumeric);
  Graphics::Wait(NUM2INT(duration));
  return Qnil;
}
VALUE ARGSS::AGraphics::rwidth(VALUE /* self */) {
  return INT2NUM(Config::Width);
}
VALUE ARGSS::AGraphics::rheight(VALUE /* self */) {
  return INT2NUM(Config::Height);
}
VALUE ARGSS::AGraphics::rresize_screen(VALUE /* self */, VALUE width, VALUE height) {
  Check_Kind(width, rb_cNumeric);
  Check_Kind(height, rb_cNumeric);
  Graphics::ResizeScreen(NUM2INT(width), NUM2INT(height));
  return Qnil;
}
VALUE ARGSS::AGraphics::rsnap_to_bitmap(VALUE /* self */) {
  return create(Graphics::SnapToBitmap());
}
VALUE ARGSS::AGraphics::rfadeout(VALUE /* self */, VALUE duration) {
  Check_Kind(duration, rb_cNumeric);
  Graphics::FadeOut(NUM2INT(duration));
  return Qnil;
}
VALUE ARGSS::AGraphics::rfadein(VALUE /* self */, VALUE duration) {
  Check_Kind(duration, rb_cNumeric);
  Graphics::FadeIn(NUM2INT(duration));
  return Qnil;
}
VALUE ARGSS::AGraphics::rbrightness(VALUE /* self */) {
  return INT2FIX(Graphics::GetBrightness());
}
VALUE ARGSS::AGraphics::rbrightnessE(VALUE /* self */, VALUE brightness) {
  Check_Kind(brightness, rb_cNumeric);
  Graphics::SetBrightness(NUM2INT(brightness));
  return brightness;
}

///////////////////////////////////////////////////////////
// ARGSS Graphics initialize
///////////////////////////////////////////////////////////
void ARGSS::AGraphics::Init() {
  rb_method const methods[] = {
    rb_method("update", rupdate, true),
    rb_method("freeze", rfreeze, true),
    rb_method("transition", rtransition, true),
    rb_method("frame_reset", rframe_reset, true),
    rb_method("frame_rate", rframe_rate, true),
    rb_method("frame_rate=", rframe_rateE, true),
    rb_method("frame_count", rframe_count, true),
    rb_method("frame_count=", rframe_countE, true),
    rb_method("fps", rfps, true),
    rb_method("backcolor", rbackcolor, true),
    rb_method("backcolor=", rbackcolorE, true),
    rb_method("wait", rwait, true),
    rb_method("width", rwidth, true),
    rb_method("height", rheight, true),
    rb_method("resize_screen", rresize_screen, true),
    rb_method("snap_to_bitmap", rsnap_to_bitmap, true),
    rb_method("fadeout", rfadeout, true),
    rb_method("fadein", rfadein, true),
    rb_method("brightness", rbrightness, true),
    rb_method("brightness=", rbrightnessE, true),
    rb_method()
  };
  define_module("Graphics", methods);
}
