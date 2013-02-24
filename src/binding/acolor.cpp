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
#include "binding/acolor.h"
#include "color.h"
#include "bitmap_fwd.h"
#include <algorithm>

///////////////////////////////////////////////////////////
// Limit color value between 0 and 255
///////////////////////////////////////////////////////////
static double LimitColorValue(double v) {
  return std::max(0.0, std::min(255.0, v));
}

///////////////////////////////////////////////////////////
// ARGSS Color instance methods
///////////////////////////////////////////////////////////
VALUE ARGSS::AColor::rinitialize(int argc, VALUE* argv, VALUE self) {
  if (argc < 3) raise_argn(argc, 3);
  else if (argc > 4) raise_argn(argc, 4);
  set_ptr(self, boost::make_shared<Color>(LimitColorValue(NUM2DBL(argv[0])),
                                          LimitColorValue(NUM2DBL(argv[1])),
                                          LimitColorValue(NUM2DBL(argv[2])), 255));
  if (argc == 4) get<Color>(self).alpha = NUM2DBL(argv[3]);
  return self;
}
VALUE ARGSS::AColor::rset(int argc, VALUE* argv, VALUE self) {
  if (argc < 3) raise_argn(argc, 3);
  else if (argc > 4) raise_argn(argc, 4);
  get<Color>(self).red = LimitColorValue(NUM2DBL(argv[0]));
  get<Color>(self).green = LimitColorValue(NUM2DBL(argv[1]));
  get<Color>(self).blue = LimitColorValue(NUM2DBL(argv[2]));
  if (argc == 4)
    get<Color>(self).alpha = NUM2DBL(argv[3]);
  else
    get<Color>(self).alpha = 255;
  return self;
}
VALUE ARGSS::AColor::rred(VALUE self) {
  return rb_float_new(get<Color>(self).red);
}
VALUE ARGSS::AColor::rredE(VALUE self, VALUE red) {
  return rb_float_new(get<Color>(self).red = LimitColorValue(NUM2DBL(red)));
}
VALUE ARGSS::AColor::rgreen(VALUE self) {
  return rb_float_new(get<Color>(self).green);
}
VALUE ARGSS::AColor::rgreenE(VALUE self, VALUE green) {
  return rb_float_new(get<Color>(self).green = LimitColorValue(NUM2DBL(green)));
}
VALUE ARGSS::AColor::rblue(VALUE self) {
  return rb_float_new(get<Color>(self).blue);
}
VALUE ARGSS::AColor::rblueE(VALUE self, VALUE blue) {
 return rb_float_new(get<Color>(self).blue = LimitColorValue(NUM2DBL(blue)));
}
VALUE ARGSS::AColor::ralpha(VALUE self) {
  return rb_float_new(get<Color>(self).alpha);
}
VALUE ARGSS::AColor::ralphaE(VALUE self, VALUE alpha) {
 return rb_float_new(get<Color>(self).alpha = LimitColorValue(NUM2DBL(alpha)));
}
VALUE ARGSS::AColor::rinspect(VALUE self) {
  char str[255];
  Color const& col = get<Color>(self);
  long str_size = sprintf(
    str, "(%f, %f, %f, %f)",
    col.red, col.green, col.blue, col.alpha);
  return rb_str_new(str, str_size);
}
VALUE ARGSS::AColor::rdump(int argc, VALUE* /* argv */, VALUE self) {
  if (argc > 1) raise_argn(argc, 1);
  Color const& c = get<Color>(self);
  return rb_funcall(rb_ary_new3(4, rb_float_new(c.red), rb_float_new(c.green),
                                rb_float_new(c.blue), rb_float_new(c.alpha)),
                    rb_intern("pack"), 1, rb_str_new2("E4"));
}

///////////////////////////////////////////////////////////
// ARGSS Color class methods
///////////////////////////////////////////////////////////
VALUE ARGSS::AColor::rload(VALUE /* self */, VALUE str) {
  VALUE arr = rb_funcall(str, rb_intern("unpack"), 1, rb_str_new2("E4"));
  assert(RARRAY_LEN(arr) == 4);
  return create(boost::make_shared<Color>
                (RFLOAT_VALUE(rb_ary_entry(arr, 0)), RFLOAT_VALUE(rb_ary_entry(arr, 1)),
                 RFLOAT_VALUE(rb_ary_entry(arr, 2)), RFLOAT_VALUE(rb_ary_entry(arr, 3))));
}

///////////////////////////////////////////////////////////
// ARGSS Color initialize
///////////////////////////////////////////////////////////
void ARGSS::AColor::Init() {
  rb_method const methods[] = {
    rb_method("initialize", rinitialize),
    rb_method("set", rset),
    rb_method("red", rred),
    rb_method("red=", rredE),
    rb_method("green", rgreen),
    rb_method("green=", rgreenE),
    rb_method("blue", rblue),
    rb_method("blue=", rblueE),
    rb_method("alpha", ralpha),
    rb_method("alpha=", ralphaE),
    rb_method("inspect", rinspect),
    rb_method("_dump", rdump),
    rb_method("_load", rload, true),
    rb_method() };
  define_class<Color, true>("Color", methods);
}
