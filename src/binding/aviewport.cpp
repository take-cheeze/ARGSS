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
#include "binding/aviewport.h"
#include "binding/acolor.h"
#include "binding/aerror.h"
#include "binding/arect.h"
#include "binding/atone.h"
#include "viewport.h"

///////////////////////////////////////////////////////////
// ARGSS Viewport instance methods
///////////////////////////////////////////////////////////
VALUE ARGSS::AViewport::rinitialize(int argc, VALUE* argv, VALUE self) {
  set_ptr(self, boost::make_shared<Viewport>());

  if (argc == 0) raise_argn(argc, 1);
  else if (argc > 1 && argc < 4) raise_argn(argc, 4);
  if (argc == 1) get<Viewport>(self).rect = get_ptr<Rect>(argv[0]);
  else get<Viewport>(self).rect = boost::make_shared<Rect>
           (NUM2INT(argv[0]), NUM2INT(argv[1]), NUM2INT(argv[2]), NUM2INT(argv[3]));
  return self;
}
VALUE ARGSS::AViewport::rdispose(VALUE self) {
  get_ptr<Viewport>(self).reset();
  return self;
}
VALUE ARGSS::AViewport::rdisposedQ(VALUE self) {
  return BOOL2NUM(not get_ptr<Viewport>(self));
}
VALUE ARGSS::AViewport::rflash(VALUE self, VALUE color, VALUE duration) {
  check_disposed<Viewport>(self);
  if (color == Qnil) {
    get<Viewport>(self).Flash(NUM2INT(duration));
  } else {
    get<Viewport>(self).Flash(get<Color>(color), NUM2INT(duration));
  }
  return Qnil;
}
VALUE ARGSS::AViewport::rupdate(VALUE self) {
  check_disposed<Viewport>(self);
  get<Viewport>(self).Update();
  return Qnil;
}
VALUE ARGSS::AViewport::rrect(VALUE self) {
  check_disposed<Viewport>(self);
  return create(get<Viewport>(self).rect);
}
VALUE ARGSS::AViewport::rrectE(VALUE self, VALUE rect) {
  check_disposed<Viewport>(self);
  Check_Class(rect, ruby_class<Rect>());
  get<Viewport>(self).rect = get_ptr<Rect>(rect);
  return rect;
}
VALUE ARGSS::AViewport::rvisible(VALUE self) {
  check_disposed<Viewport>(self);
  return get<Viewport>(self).visible;
}
VALUE ARGSS::AViewport::rvisibleE(VALUE self, VALUE visible) {
  check_disposed<Viewport>(self);
  get<Viewport>(self).visible = NUM2BOOL(visible);
  return visible;
}
VALUE ARGSS::AViewport::rz(VALUE self) {
  check_disposed<Viewport>(self);
  return INT2NUM(get<Viewport>(self).z());
}
VALUE ARGSS::AViewport::rzE(VALUE self, VALUE z) {
  check_disposed<Viewport>(self);
  return get<Viewport>(self).z(NUM2INT(z)), z;
}
VALUE ARGSS::AViewport::rox(VALUE self) {
  check_disposed<Viewport>(self);
  return get<Viewport>(self).ox;
}
VALUE ARGSS::AViewport::roxE(VALUE self, VALUE ox) {
  check_disposed<Viewport>(self);
  return get<Viewport>(self).ox = NUM2INT(ox), ox;
}
VALUE ARGSS::AViewport::roy(VALUE self) {
  check_disposed<Viewport>(self);
  return INT2NUM(get<Viewport>(self).oy);
}
VALUE ARGSS::AViewport::royE(VALUE self, VALUE oy) {
  check_disposed<Viewport>(self);
  return get<Viewport>(self).oy = NUM2INT(oy), oy;
}
VALUE ARGSS::AViewport::rcolor(VALUE self) {
  check_disposed<Viewport>(self);
  return create(get<Viewport>(self).color);
}
VALUE ARGSS::AViewport::rcolorE(VALUE self, VALUE color) {
  check_disposed<Viewport>(self);
  Check_Class(color, ruby_class<Color>());
  return get<Viewport>(self).color = get_ptr<Color>(color), color;
}
VALUE ARGSS::AViewport::rtone(VALUE self) {
  check_disposed<Viewport>(self);
  return create(get<Viewport>(self).tone);
}
VALUE ARGSS::AViewport::rtoneE(VALUE self, VALUE tone) {
  check_disposed<Viewport>(self);
  Check_Class(tone, ruby_class<Tone>());
  return get<Viewport>(self).tone = get_ptr<Tone>(tone), tone;
}

///////////////////////////////////////////////////////////
// ARGSS Viewport initialize
///////////////////////////////////////////////////////////
void ARGSS::AViewport::Init() {
  rb_method const methods[] = {
    rb_method("initialize", rinitialize),
    rb_method("dispose", rdispose),
    rb_method("disposed?", rdisposedQ),
    rb_method("flash", rflash),
    rb_method("update", rupdate),
    rb_method("rect", rrect),
    rb_method("rect=", rrectE),
    rb_method("visible", rvisible),
    rb_method("visible=", rvisibleE),
    rb_method("z", rz),
    rb_method("z=", rzE),
    rb_method("ox", rox),
    rb_method("ox=", roxE),
    rb_method("oy", roy),
    rb_method("oy=", royE),
    rb_method("color", rcolor),
    rb_method("color=", rcolorE),
    rb_method("tone", rtone),
    rb_method("tone=", rtoneE),
    rb_method() };
  define_class<Viewport>("Viewport", methods);
}
