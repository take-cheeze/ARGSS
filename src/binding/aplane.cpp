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
#include "binding/aplane.h"
#include "binding/abitmap.h"
#include "binding/acolor.h"
#include "binding/aerror.h"
#include "binding/atone.h"
#include "binding/aviewport.h"
#include "plane.h"
#include "output.h"


///////////////////////////////////////////////////////////
// ARGSS Plane instance functions
///////////////////////////////////////////////////////////
VALUE ARGSS::APlane::rinitialize(int argc, VALUE* argv, VALUE self) {
  set_ptr(self, boost::make_shared<Plane>());
  if (argc == 1) {
    Check_Classes_N(argv[0], ruby_class<Viewport>());
    get<Plane>(self).viewport(get_ptr<Viewport>(argv[0]));
  } else if (argc == 0) {
  } else {
    raise_argn(argc, 1);
  }
  return self;
}
VALUE ARGSS::APlane::rdispose(VALUE self) {
  get_ptr<Plane>(self).reset();
  return self;
}
VALUE ARGSS::APlane::rdisposedQ(VALUE self) {
  return get_ptr<Plane>(self)? Qfalse : Qtrue;
}
VALUE ARGSS::APlane::rviewport(VALUE self) {
  check_disposed<Plane>(self);
  return create(get<Plane>(self).viewport());
}
VALUE ARGSS::APlane::rviewportE(VALUE self, VALUE viewport) {
  check_disposed<Plane>(self);
  Check_Classes_N(viewport, ruby_class<Viewport>());
  get<Plane>(self).viewport(get_ptr<Viewport>(viewport));
  return create(get<Plane>(self).viewport());
}
VALUE ARGSS::APlane::rbitmap(VALUE self) {
  check_disposed<Plane>(self);
  return create(get<Plane>(self).bitmap);
}
VALUE ARGSS::APlane::rbitmapE(VALUE self, VALUE bitmap) {
  check_disposed<Plane>(self);
  Check_Classes_N(bitmap, ruby_class<Bitmap>());
  return create(get<Plane>(self).bitmap = get_ptr<Bitmap>(bitmap));
}
VALUE ARGSS::APlane::rvisible(VALUE self) {
  check_disposed<Plane>(self);
  return get<Plane>(self).visible? Qtrue : Qfalse;
}
VALUE ARGSS::APlane::rvisibleE(VALUE self, VALUE visible) {
  check_disposed<Plane>(self);
  return get<Plane>(self).visible = visible == Qtrue, visible;
}
VALUE ARGSS::APlane::rz(VALUE self) {
  check_disposed<Plane>(self);
  return INT2NUM(get<Plane>(self).z());
}
VALUE ARGSS::APlane::rzE(VALUE self, VALUE z) {
  check_disposed<Plane>(self);
  return get<Plane>(self).z(NUM2INT(z)), z;
}
VALUE ARGSS::APlane::rox(VALUE self) {
  check_disposed<Plane>(self);
  return INT2NUM(get<Plane>(self).ox);
}
VALUE ARGSS::APlane::roxE(VALUE self, VALUE ox) {
  check_disposed<Plane>(self);
  return get<Plane>(self).ox = NUM2INT(ox), ox;
}
VALUE ARGSS::APlane::roy(VALUE self) {
  check_disposed<Plane>(self);
  return INT2NUM(get<Plane>(self).oy);
}
VALUE ARGSS::APlane::royE(VALUE self, VALUE oy) {
  check_disposed<Plane>(self);
  get<Plane>(self).oy = NUM2INT(oy);
  return oy;
}
VALUE ARGSS::APlane::rzoom_x(VALUE self) {
  check_disposed<Plane>(self);
  return rb_float_new(get<Plane>(self).zoom_x);
}
VALUE ARGSS::APlane::rzoom_xE(VALUE self, VALUE zoom_x) {
  check_disposed<Plane>(self);
  return get<Plane>(self).zoom_x = NUM2DBL(zoom_x), zoom_x;
}
VALUE ARGSS::APlane::rzoom_y(VALUE self) {
  check_disposed<Plane>(self);
  return rb_float_new(get<Plane>(self).zoom_y);
}
VALUE ARGSS::APlane::rzoom_yE(VALUE self, VALUE zoom_y) {
  check_disposed<Plane>(self);
  get<Plane>(self).zoom_y = NUM2DBL(zoom_y);
  return zoom_y;
}
VALUE ARGSS::APlane::ropacity(VALUE self) {
  check_disposed<Plane>(self);
  return INT2NUM(get<Plane>(self).opacity);
}
VALUE ARGSS::APlane::ropacityE(VALUE self, VALUE opacity) {
  check_disposed<Plane>(self);
  return get<Plane>(self).opacity = NUM2INT(opacity), opacity;
}
VALUE ARGSS::APlane::rblend_type(VALUE self) {
  check_disposed<Plane>(self);
  return INT2NUM(get<Plane>(self).blend_type);
}
VALUE ARGSS::APlane::rblend_typeE(VALUE self, VALUE blend_type) {
  check_disposed<Plane>(self);
  return INT2NUM(get<Plane>(self).blend_type = NUM2INT(blend_type));
}
VALUE ARGSS::APlane::rcolor(VALUE self) {
  check_disposed<Plane>(self);
  return create(get<Plane>(self).color);
}
VALUE ARGSS::APlane::rcolorE(VALUE self, VALUE color) {
  check_disposed<Plane>(self);
  Check_Class(color, ruby_class<Color>());
  return get<Plane>(self).color = get_ptr<Color>(color), color;
}
VALUE ARGSS::APlane::rtone(VALUE self) {
  check_disposed<Plane>(self);
  return create(get<Plane>(self).tone);
}
VALUE ARGSS::APlane::rtoneE(VALUE self, VALUE tone) {
  check_disposed<Plane>(self);
  Check_Class(tone, ruby_class<Tone>());
  return get<Plane>(self).tone = get_ptr<Tone>(tone), tone;
}

///////////////////////////////////////////////////////////
// ARGSS Plane initialize
///////////////////////////////////////////////////////////
void ARGSS::APlane::Init() {
  rb_method const methods[] = {
    rb_method("initialize", rinitialize),
    rb_method("dispose", rdispose),
    rb_method("disposed?", rdisposedQ),
    rb_method("viewport", rviewport),
    rb_method("viewport=", rviewportE),
    rb_method("bitmap", rbitmap),
    rb_method("bitmap=", rbitmapE),
    rb_method("visible", rvisible),
    rb_method("visible=", rvisibleE),
    rb_method("z", rz),
    rb_method("z=", rzE),
    rb_method("ox", rox),
    rb_method("ox=", roxE),
    rb_method("oy", roy),
    rb_method("oy=", royE),
    rb_method("zoom_x", rzoom_x),
    rb_method("zoom_x=", rzoom_xE),
    rb_method("zoom_y", rzoom_y),
    rb_method("zoom_y=", rzoom_yE),
    rb_method("opacity", ropacity),
    rb_method("opacity=", ropacityE),
    rb_method("blend_type", rblend_type),
    rb_method("blend_type=", rblend_typeE),
    rb_method("color", rcolor),
    rb_method("color=", rcolorE),
    rb_method("tone", rtone),
    rb_method("tone=", rtoneE),
    rb_method() };
  define_class<Plane>("Plane", methods);
}
