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
#include "binding/asprite.h"
#include "binding/abitmap.h"
#include "binding/acolor.h"
#include "binding/aerror.h"
#include "binding/arect.h"
#include "binding/atone.h"
#include "binding/aviewport.h"
#include "color.h"
#include "sprite.h"
#include "bitmap.h"
#include <boost/format.hpp>
#include "output.h"

///////////////////////////////////////////////////////////
// ARGSS Sprite instance methods
///////////////////////////////////////////////////////////
VALUE ARGSS::ASprite::rinitialize(int argc, VALUE* argv, VALUE self) {
  set_ptr(self, boost::make_shared<Sprite>());
  if (argc == 1) {
    Check_Classes_N(argv[0], ruby_class<Viewport>());
    get<Sprite>(self).viewport(get_ptr<Viewport>(argv[0]));
  } else if (argc == 0) {}
  else raise_argn(argc, 1);
  return self;
}
VALUE ARGSS::ASprite::rdispose(VALUE self) {
  get_ptr<Sprite>(self).reset();
  return self;
}
VALUE ARGSS::ASprite::rdisposedQ(VALUE self) {
  return BOOL2NUM(get_ptr<Sprite>(self));
}
VALUE ARGSS::ASprite::rflash(VALUE self, VALUE color, VALUE duration) {
  check_disposed<Sprite>(self);
  if (color == Qnil) {
    get<Sprite>(self).Flash(NUM2INT(duration));
  } else {
    get<Sprite>(self).Flash(get<Color>(color), NUM2INT(duration));
  }
  return Qnil;
}
VALUE ARGSS::ASprite::rupdate(VALUE self) {
  check_disposed<Sprite>(self);
  get<Sprite>(self).Update();
  return Qnil;
}
VALUE ARGSS::ASprite::rwidth(VALUE self) {
  check_disposed<Sprite>(self);
  return INT2NUM(get<Sprite>(self).src_rect->width);
}
VALUE ARGSS::ASprite::rheight(VALUE self) {
  check_disposed<Sprite>(self);
  return INT2NUM(get<Sprite>(self).src_rect->height);
}
VALUE ARGSS::ASprite::rviewport(VALUE self) {
  check_disposed<Sprite>(self);
  return create(get<Sprite>(self).viewport());
}
VALUE ARGSS::ASprite::rviewportE(VALUE self, VALUE viewport) {
  check_disposed<Sprite>(self);
  Check_Classes_N(viewport, ruby_class<Viewport>());
  get<Sprite>(self).viewport(get_ptr<Viewport>(viewport));
  return create(get<Sprite>(self).viewport());
}
VALUE ARGSS::ASprite::rbitmap(VALUE self) {
  check_disposed<Sprite>(self);
  return create(get<Sprite>(self).bitmap());
}
VALUE ARGSS::ASprite::rbitmapE(VALUE self, VALUE bitmap) {
  check_disposed<Sprite>(self);
  Check_Classes_N(bitmap, ruby_class<Bitmap>());
  if (bitmap != Qnil) {
    bool const disp = bool(get_ptr<Bitmap>(bitmap));
    get<Sprite>(self).src_rect =
        boost::make_shared<Rect>(0, 0, disp? 0 : get<Bitmap>(bitmap).GetWidth(),
                                 disp? 0 : get<Bitmap>(bitmap).GetHeight());
  }
  get<Sprite>(self).bitmap(get_ptr<Bitmap>(bitmap));
  return bitmap;
}
VALUE ARGSS::ASprite::rsrc_rect(VALUE self) {
  check_disposed<Sprite>(self);
  return create(get<Sprite>(self).src_rect);
}
VALUE ARGSS::ASprite::rsrc_rectE(VALUE self, VALUE src_rect) {
  check_disposed<Sprite>(self);
  Check_Class(src_rect, ruby_class<Rect>());
  get<Sprite>(self).src_rect = get_ptr<Rect>(src_rect);
  return src_rect;
}
VALUE ARGSS::ASprite::rvisible(VALUE self) {
  check_disposed<Sprite>(self);
  return BOOL2NUM(get<Sprite>(self).visible);
}
VALUE ARGSS::ASprite::rvisibleE(VALUE self, VALUE visible) {
  check_disposed<Sprite>(self);
  get<Sprite>(self).visible = NUM2BOOL(visible);
  return visible;
}
VALUE ARGSS::ASprite::rx(VALUE self) {
  check_disposed<Sprite>(self);
  return INT2NUM(get<Sprite>(self).x);
}
VALUE ARGSS::ASprite::rfx(VALUE self) {
  check_disposed<Sprite>(self);
  return rb_float_new(get<Sprite>(self).x);
}
VALUE ARGSS::ASprite::rxE(VALUE self, VALUE x) {
  check_disposed<Sprite>(self);
  get<Sprite>(self).x = NUM2INT(x);
  return x;
}
VALUE ARGSS::ASprite::ry(VALUE self) {
  check_disposed<Sprite>(self);
  return INT2NUM(get<Sprite>(self).y);
}
VALUE ARGSS::ASprite::rfy(VALUE self) {
  check_disposed<Sprite>(self);
  return rb_float_new(get<Sprite>(self).y);
}
VALUE ARGSS::ASprite::ryE(VALUE self, VALUE y) {
  check_disposed<Sprite>(self);
  return get<Sprite>(self).y = NUM2INT(y), y;
}
VALUE ARGSS::ASprite::rz(VALUE self) {
  check_disposed<Sprite>(self);
  return INT2NUM(get<Sprite>(self).z());
}
VALUE ARGSS::ASprite::rzE(VALUE self, VALUE z) {
  check_disposed<Sprite>(self);
  return get<Sprite>(self).z(NUM2INT(z)), z;
}
VALUE ARGSS::ASprite::rox(VALUE self) {
  check_disposed<Sprite>(self);
  return NUM2INT(get<Sprite>(self).ox);
}
VALUE ARGSS::ASprite::roxE(VALUE self, VALUE ox) {
  check_disposed<Sprite>(self);
  return get<Sprite>(self).ox = NUM2INT(ox), ox;
}
VALUE ARGSS::ASprite::roy(VALUE self) {
  check_disposed<Sprite>(self);
  return INT2NUM(get<Sprite>(self).oy);
}
VALUE ARGSS::ASprite::royE(VALUE self, VALUE oy) {
  check_disposed<Sprite>(self);
  return get<Sprite>(self).oy = NUM2INT(oy), oy;
}
VALUE ARGSS::ASprite::rzoom_x(VALUE self) {
  check_disposed<Sprite>(self);
  return rb_float_new(get<Sprite>(self).zoom_x);
}
VALUE ARGSS::ASprite::rzoom_xE(VALUE self, VALUE zoom_x) {
  check_disposed<Sprite>(self);
  return get<Sprite>(self).zoom_x = NUM2DBL(zoom_x), zoom_x;
}
VALUE ARGSS::ASprite::rzoom_y(VALUE self) {
  check_disposed<Sprite>(self);
  return rb_float_new(get<Sprite>(self).zoom_y);
}
VALUE ARGSS::ASprite::rzoom_yE(VALUE self, VALUE zoom_y) {
  check_disposed<Sprite>(self);
  get<Sprite>(self).zoom_y = NUM2DBL(zoom_y);
  return zoom_y;
}
VALUE ARGSS::ASprite::rangle(VALUE self) {
  check_disposed<Sprite>(self);
  return rb_float_new(get<Sprite>(self).angle);
}
VALUE ARGSS::ASprite::rangleE(VALUE self, VALUE angle) {
  check_disposed<Sprite>(self);
  get<Sprite>(self).angle = NUM2DBL(angle);
  return angle;
}
VALUE ARGSS::ASprite::rmirror(VALUE self) {
  check_disposed<Sprite>(self);
  return BOOL2NUM(get<Sprite>(self).flipx);
}
VALUE ARGSS::ASprite::rmirrorE(VALUE self, VALUE mirror) {
  check_disposed<Sprite>(self);
  return get<Sprite>(self).flipx = NUM2BOOL(mirror), mirror;
}
VALUE ARGSS::ASprite::rflipx(VALUE self) {
  check_disposed<Sprite>(self);
  return BOOL2NUM(get<Sprite>(self).flipx);
}
VALUE ARGSS::ASprite::rflipxE(VALUE self, VALUE flipx) {
  check_disposed<Sprite>(self);
  get<Sprite>(self).flipx = NUM2BOOL(flipx);
  return flipx;
}
VALUE ARGSS::ASprite::rflipy(VALUE self) {
  check_disposed<Sprite>(self);
  return BOOL2NUM(get<Sprite>(self).flipy);
}
VALUE ARGSS::ASprite::rflipyE(VALUE self, VALUE flipy) {
  check_disposed<Sprite>(self);
  get<Sprite>(self).flipy = NUM2BOOL(flipy);
  return flipy;
}
VALUE ARGSS::ASprite::rbush_depth(VALUE self) {
  check_disposed<Sprite>(self);
  return INT2NUM(get<Sprite>(self).bush_depth);
}
VALUE ARGSS::ASprite::rbush_depthE(VALUE self, VALUE bush_depth) {
  check_disposed<Sprite>(self);
  get<Sprite>(self).bush_depth = NUM2INT(bush_depth);
  return bush_depth;
}
VALUE ARGSS::ASprite::ropacity(VALUE self) {
  check_disposed<Sprite>(self);
  return INT2NUM(get<Sprite>(self).opacity);
}
VALUE ARGSS::ASprite::ropacityE(VALUE self, VALUE opacity) {
  check_disposed<Sprite>(self);
  int opacity_temp = std::min(255, std::max<int>(0, NUM2INT(opacity)));
  get<Sprite>(self).opacity = opacity_temp;
  return INT2NUM(opacity_temp);
}
VALUE ARGSS::ASprite::rblend_type(VALUE self) {
  check_disposed<Sprite>(self);
  return INT2NUM(get<Sprite>(self).blend_type);
}
VALUE ARGSS::ASprite::rblend_typeE(VALUE self, VALUE blend_type) {
  check_disposed<Sprite>(self);
  int type = NUM2INT(blend_type);
  if (type < 0 || type > 3) type = 0;
  get<Sprite>(self).blend_type = type;
  return INT2NUM(type);
}
VALUE ARGSS::ASprite::rcolor(VALUE self) {
  check_disposed<Sprite>(self);
  return create(get<Sprite>(self).color);
}
VALUE ARGSS::ASprite::rcolorE(VALUE self, VALUE color) {
  check_disposed<Sprite>(self);
  Check_Class(color, ruby_class<Color>());
  return create(get<Sprite>(self).color = get_ptr<Color>(color));
}
VALUE ARGSS::ASprite::rtone(VALUE self) {
  check_disposed<Sprite>(self);
  return create(get<Sprite>(self).tone());
}
VALUE ARGSS::ASprite::rtoneE(VALUE self, VALUE tone) {
  check_disposed<Sprite>(self);
  Check_Class(tone, ruby_class<Color>());
  get<Sprite>(self).tone(get_ptr<Tone>(tone));
  return create(get<Sprite>(self).tone());
}

///////////////////////////////////////////////////////////
// ARGSS Sprite initialize
///////////////////////////////////////////////////////////
void ARGSS::ASprite::Init() {
  rb_method const methods[] = {
    rb_method("initialize", rinitialize),
    rb_method("dispose", rdispose),
    rb_method("disposed?", rdisposedQ),
    rb_method("flash", rflash),
    rb_method("update", rupdate),
    rb_method("width", rwidth),
    rb_method("height", rheight),
    rb_method("viewport", rviewport),
    rb_method("viewport=", rviewportE),
    rb_method("bitmap", rbitmap),
    rb_method("bitmap=", rbitmapE),
    rb_method("src_rect", rsrc_rect),
    rb_method("src_rect=", rsrc_rectE),
    rb_method("visible", rvisible),
    rb_method("visible=", rvisibleE),
    rb_method("x", rx),
    rb_method("fx", rfx),
    rb_method("x=", rxE),
    rb_method("y", ry),
    rb_method("fy", rfy),
    rb_method("y=", ryE),
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
    rb_method("angle", rangle),
    rb_method("angle=", rangleE),
    rb_method("mirror", rmirror),
    rb_method("mirror=", rmirrorE),
    rb_method("flipx", rflipx),
    rb_method("flipx=", rflipxE),
    rb_method("flipy", rflipy),
    rb_method("flipy=", rflipyE),
    rb_method("bush_depth", rbush_depth),
    rb_method("bush_depth=", rbush_depthE),
    rb_method("opacity", ropacity),
    rb_method("opacity=", ropacityE),
    rb_method("blend_type", rblend_type),
    rb_method("blend_type=", rblend_typeE),
    rb_method("color", rcolor),
    rb_method("color=", rcolorE),
    rb_method("tone", rtone),
    rb_method("tone=", rtoneE),
    rb_method() };
  define_class<Sprite, false>("Sprite", methods);
}
