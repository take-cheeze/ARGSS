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
#include "binding/arect.h"
#include "rect.h"

///////////////////////////////////////////////////////////
// ARGSS Rect instance methods
///////////////////////////////////////////////////////////
VALUE ARGSS::ARect::rinitialize(VALUE self, VALUE x, VALUE y, VALUE w, VALUE h) {
  Check_Kind(x, rb_cNumeric);
  Check_Kind(y, rb_cNumeric);
  Check_Kind(w, rb_cNumeric);
  Check_Kind(h, rb_cNumeric);
  set_ptr(self, boost::make_shared<Rect>(NUM2INT(x), NUM2INT(y), NUM2INT(w), NUM2INT(h)));
  return self;
}
VALUE ARGSS::ARect::rset(VALUE self, VALUE x, VALUE y, VALUE w, VALUE h) {
  Check_Kind(x, rb_cNumeric);
  Check_Kind(y, rb_cNumeric);
  Check_Kind(w, rb_cNumeric);
  Check_Kind(h, rb_cNumeric);
  get<Rect>(self) = Rect(NUM2INT(x), NUM2INT(y), NUM2INT(w), NUM2INT(h));
  return self;
}
VALUE ARGSS::ARect::rx(VALUE self) {
  return INT2NUM(get<Rect>(self).x);
}
VALUE ARGSS::ARect::rxE(VALUE self, VALUE x) {
  Check_Kind(x, rb_cNumeric);
  return INT2NUM(get<Rect>(self).x = NUM2INT(x));
}
VALUE ARGSS::ARect::ry(VALUE self) {
  return INT2NUM(get<Rect>(self).y);
}
VALUE ARGSS::ARect::ryE(VALUE self, VALUE y) {
  Check_Kind(y, rb_cNumeric);
  return INT2NUM(get<Rect>(self).y = NUM2INT(y));
}
VALUE ARGSS::ARect::rwidth(VALUE self) {
  return INT2NUM(get<Rect>(self).width);
}
VALUE ARGSS::ARect::rwidthE(VALUE self, VALUE w) {
  Check_Kind(w, rb_cNumeric);
  return INT2NUM(get<Rect>(self).width = NUM2INT(w));
}
VALUE ARGSS::ARect::rheight(VALUE self) {
  return INT2NUM(get<Rect>(self).height);
}
VALUE ARGSS::ARect::rheightE(VALUE self, VALUE h) {
  Check_Kind(h, rb_cNumeric);
  return INT2NUM(get<Rect>(self).height = NUM2INT(h));
}
VALUE ARGSS::ARect::rempty(VALUE self) {
  Rect& re = get<Rect>(self);
  re.x = re.y = re.width = re.height = 0;
  return self;
}
VALUE ARGSS::ARect::rinspect(VALUE self) {
  char str[255];
  Rect const& re = get<Rect>(self);
  long str_size = sprintf(str, "(%i, %i, %i, %i)", re.x, re.y, re.width, re.height);
  return rb_str_new(str, str_size);
}

VALUE ARGSS::ARect::rdump(int argc, VALUE* /* argv */, VALUE self) {
  if (argc > 1) raise_argn(argc, 1);
  Rect const& re = get<Rect>(self);
  VALUE arr = rb_ary_new3(4, INT2NUM(re.x), INT2NUM(re.y), INT2NUM(re.width), INT2NUM(re.height));
  return rb_funcall(arr, rb_intern("pack"), 1, rb_str_new2("V4"));
}

///////////////////////////////////////////////////////////
// ARGSS Rect class methods
///////////////////////////////////////////////////////////
VALUE ARGSS::ARect::rload(VALUE /* self */, VALUE str) {
  VALUE arr = rb_funcall(str, rb_intern("unpack"), 1, rb_str_new2("V4"));
  return create(boost::make_shared<Rect>
                (NUM2INT(rb_ary_entry(arr, 0)), NUM2INT(rb_ary_entry(arr, 1)),
                 NUM2INT(rb_ary_entry(arr, 2)), NUM2INT(rb_ary_entry(arr, 3))));
}

///////////////////////////////////////////////////////////
// ARGSS Rect initialize
///////////////////////////////////////////////////////////
void ARGSS::ARect::Init() {
  rb_method const methods[] = {
    rb_method("initialize", rinitialize),
    rb_method("set", rset),
    rb_method("x", rx),
    rb_method("x=", rxE),
    rb_method("y", ry),
    rb_method("y=", ryE),
    rb_method("width", rwidth),
    rb_method("width=", rwidthE),
    rb_method("height", rheight),
    rb_method("height=", rheightE),
    rb_method("empty", rempty),
    rb_method("inspect", rinspect),
    rb_method("_dump", rdump),
    rb_method("_load", rload, true),
    rb_method() };
  define_class<Rect, true>("Rect", methods);
}
