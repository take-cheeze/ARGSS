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
#include "binding/awindow_xp.h"
#include "binding/abitmap.h"
#include "binding/acolor.h"
#include "binding/aerror.h"
#include "binding/arect.h"
#include "binding/aviewport.h"
#include "window_xp.h"
#include "output.h"
#include <algorithm>

///////////////////////////////////////////////////////////
// Limit opacity value between 0 and 255
///////////////////////////////////////////////////////////
static int LimitOpacityValue(int v) {
  return std::max(0, std::min(255, v));
}

///////////////////////////////////////////////////////////
// ARGSS Window instance methods
///////////////////////////////////////////////////////////
VALUE ARGSS::AWindow::rinitialize(int argc, VALUE* argv, VALUE self) {
  set_ptr(self, boost::make_shared<Window>());
  if (argc == 1) {
    Check_Classes_N(argv[0], ruby_class<Viewport>());
    get<Window>(self).viewport(get_ptr<Viewport>(argv[0]));
  } else if (argc == 0) {
  } else raise_argn(argc, 1);
  return self;
}
VALUE ARGSS::AWindow::rdispose(VALUE self) {
  get_ptr<Window>(self).reset();
  return self;
}
VALUE ARGSS::AWindow::rdisposedQ(VALUE self) {
  return BOOL2NUM(not get_ptr<Window>(self));
}
VALUE ARGSS::AWindow::rupdate(VALUE self) {
  check_disposed<Window>(self);
  get<Window>(self).Update();
  return Qnil;
}
VALUE ARGSS::AWindow::rviewport(VALUE self) {
  check_disposed<Window>(self);
  return create(get<Window>(self).viewport());
}
VALUE ARGSS::AWindow::rviewportE(VALUE self, VALUE viewport) {
  check_disposed<Window>(self);
  Check_Classes_N(viewport, ruby_class<Viewport>());
  return get<Window>(self).viewport(get_ptr<Viewport>(viewport)), viewport;
}
VALUE ARGSS::AWindow::rwindowskin(VALUE self) {
  check_disposed<Window>(self);
  return create(get<Window>(self).windowskin);
}
VALUE ARGSS::AWindow::rwindowskinE(VALUE self, VALUE windowskin) {
  check_disposed<Window>(self);
  Check_Classes_N(windowskin, ruby_class<Bitmap>());
  return get<Window>(self).windowskin = get_ptr<Bitmap>(windowskin), windowskin;
}
VALUE ARGSS::AWindow::rcontents(VALUE self) {
  check_disposed<Window>(self);
  return create(get<Window>(self).contents);
}
VALUE ARGSS::AWindow::rcontentsE(VALUE self, VALUE contents) {
  check_disposed<Window>(self);
  Check_Classes_N(contents, ruby_class<Bitmap>());
  return get<Window>(self).contents = get_ptr<Bitmap>(contents), contents;
}
VALUE ARGSS::AWindow::rstretch(VALUE self) {
  check_disposed<Window>(self);
  return BOOL2NUM(get<Window>(self).stretch);
}
VALUE ARGSS::AWindow::rstretchE(VALUE self, VALUE stretch) {
  check_disposed<Window>(self);
  return get<Window>(self).stretch = NUM2BOOL(stretch), stretch;
}
VALUE ARGSS::AWindow::rcursor_rect(VALUE self) {
  check_disposed<Window>(self);
  return create(get<Window>(self).cursor_rect);
}
VALUE ARGSS::AWindow::rcursor_rectE(VALUE self, VALUE cursor_rect) {
  check_disposed<Window>(self);
  Check_Class(cursor_rect, ruby_class<Rect>());
  return get<Window>(self).cursor_rect = get_ptr<Rect>(cursor_rect), cursor_rect;
}
VALUE ARGSS::AWindow::ractive(VALUE self) {
  check_disposed<Window>(self);
  return BOOL2NUM(get<Window>(self).active);
}
VALUE ARGSS::AWindow::ractiveE(VALUE self, VALUE active) {
  check_disposed<Window>(self);
  return get<Window>(self).active = NUM2BOOL(active), active;
}
VALUE ARGSS::AWindow::rvisible(VALUE self) {
  check_disposed<Window>(self);
  return BOOL2NUM(get<Window>(self).visible);
}
VALUE ARGSS::AWindow::rvisibleE(VALUE self, VALUE visible) {
  check_disposed<Window>(self);
  return get<Window>(self).visible = NUM2BOOL(visible), visible;
}
VALUE ARGSS::AWindow::rpause(VALUE self) {
  check_disposed<Window>(self);
  return BOOL2NUM(get<Window>(self).pause);
}
VALUE ARGSS::AWindow::rpauseE(VALUE self, VALUE pause) {
  check_disposed<Window>(self);
  return get<Window>(self).pause = NUM2BOOL(pause), pause;
}
VALUE ARGSS::AWindow::rx(VALUE self) {
  check_disposed<Window>(self);
  return INT2NUM(get<Window>(self).x);
}
VALUE ARGSS::AWindow::rfx(VALUE self) {
  check_disposed<Window>(self);
  return rb_float_new(get<Window>(self).x);
}
VALUE ARGSS::AWindow::rxE(VALUE self, VALUE x) {
  check_disposed<Window>(self);
  return get<Window>(self).x = NUM2INT(x), x;
}
VALUE ARGSS::AWindow::ry(VALUE self) {
  check_disposed<Window>(self);
  return INT2NUM(get<Window>(self).y);
}
VALUE ARGSS::AWindow::rfy(VALUE self) {
  check_disposed<Window>(self);
  return rb_float_new(get<Window>(self).y);
}
VALUE ARGSS::AWindow::ryE(VALUE self, VALUE y) {
  check_disposed<Window>(self);
  return get<Window>(self).y = NUM2INT(y), y;
}
VALUE ARGSS::AWindow::rwidth(VALUE self) {
  check_disposed<Window>(self);
  return NUM2INT(get<Window>(self).width);
}
VALUE ARGSS::AWindow::rwidthE(VALUE self, VALUE width) {
  check_disposed<Window>(self);
  return INT2NUM(get<Window>(self).width = std::max<int>(0, NUM2INT(width)));
}
VALUE ARGSS::AWindow::rheight(VALUE self) {
  check_disposed<Window>(self);
  return INT2NUM(get<Window>(self).height);
}
VALUE ARGSS::AWindow::rheightE(VALUE self, VALUE height) {
  check_disposed<Window>(self);
  get<Window>(self).height = std::max<int>(0, NUM2INT(height));
  return height;
}
VALUE ARGSS::AWindow::rz(VALUE self) {
  check_disposed<Window>(self);
  return INT2NUM(get<Window>(self).z());
}
VALUE ARGSS::AWindow::rzE(VALUE self, VALUE z) {
  check_disposed<Window>(self);
  return get<Window>(self).z(NUM2INT(z)), z;
}
VALUE ARGSS::AWindow::rox(VALUE self) {
  check_disposed<Window>(self);
  return INT2NUM(get<Window>(self).ox);
}
VALUE ARGSS::AWindow::roxE(VALUE self, VALUE ox) {
  check_disposed<Window>(self);
  return get<Window>(self).ox = NUM2INT(ox), ox;
}
VALUE ARGSS::AWindow::roy(VALUE self) {
  check_disposed<Window>(self);
  return INT2NUM(get<Window>(self).oy);
}
VALUE ARGSS::AWindow::royE(VALUE self, VALUE oy) {
  check_disposed<Window>(self);
  get<Window>(self).oy = NUM2INT(oy);
  return get<Window>(self).oy = NUM2INT(oy), oy;
}
VALUE ARGSS::AWindow::ropacity(VALUE self) {
  check_disposed<Window>(self);
  return INT2NUM(get<Window>(self).opacity);
}
VALUE ARGSS::AWindow::ropacityE(VALUE self, VALUE opacity) {
  check_disposed<Window>(self);
  get<Window>(self).opacity = LimitOpacityValue(NUM2INT(opacity));
  return opacity;
}
VALUE ARGSS::AWindow::rback_opacity(VALUE self) {
  check_disposed<Window>(self);
  return INT2NUM(get<Window>(self).back_opacity);
}
VALUE ARGSS::AWindow::rback_opacityE(VALUE self, VALUE back_opacity) {
  check_disposed<Window>(self);
  return INT2NUM(get<Window>(self).back_opacity
                 = LimitOpacityValue(NUM2INT(back_opacity)));
}
VALUE ARGSS::AWindow::rcontents_opacity(VALUE self) {
  check_disposed<Window>(self);
  return INT2NUM(get<Window>(self).contents_opacity);
}
VALUE ARGSS::AWindow::rcontents_opacityE(VALUE self, VALUE contents_opacity) {
  check_disposed<Window>(self);
  return INT2NUM(get<Window>(self).contents_opacity
                 = LimitOpacityValue(NUM2INT(contents_opacity)));
}

///////////////////////////////////////////////////////////
// ARGSS Window initialize
///////////////////////////////////////////////////////////
void ARGSS::AWindow::Init() {
  rb_method const methods[] = {
    rb_method("initialize", rinitialize),
    rb_method("dispose", rdispose),
    rb_method("disposed?", rdisposedQ),
    rb_method("update", rupdate),
    rb_method("viewport", rviewport),
    rb_method("viewport=", rviewportE),
    rb_method("windowskin", rwindowskin),
    rb_method("windowskin=", rwindowskinE),
    rb_method("contents", rcontents),
    rb_method("contents=", rcontentsE),
    rb_method("stretch", rstretch),
    rb_method("stretch=", rstretchE),
    rb_method("cursor_rect", rcursor_rect),
    rb_method("cursor_rect=", rcursor_rectE),
    rb_method("active", ractive),
    rb_method("active=", ractiveE),
    rb_method("visible", rvisible),
    rb_method("visible=", rvisibleE),
    rb_method("pause", rpause),
    rb_method("pause=", rpauseE),
    rb_method("x", rx),
    rb_method("fx", rx),
    rb_method("x=", rxE),
    rb_method("y", ry),
    rb_method("fy", ry),
    rb_method("y=", ryE),
    rb_method("width", rwidth),
    rb_method("width=", rwidthE),
    rb_method("height", rheight),
    rb_method("height=", rheightE),
    rb_method("z", rz),
    rb_method("z=", rzE),
    rb_method("ox", rox),
    rb_method("ox=", roxE),
    rb_method("oy", roy),
    rb_method("oy=", royE),
    rb_method("opacity", ropacity),
    rb_method("opacity=", ropacityE),
    rb_method("back_opacity", rback_opacity),
    rb_method("back_opacity=", rback_opacityE),
    rb_method("contents_opacity", rcontents_opacity),
    rb_method("contents_opacity=", rcontents_opacityE),
    rb_method() };
  define_class<Window, false>("Window", methods);
}
