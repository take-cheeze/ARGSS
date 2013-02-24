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
#include "binding/atone.h"
#include "tone.h"

///////////////////////////////////////////////////////////
// Limit tone value between -255 and 255
///////////////////////////////////////////////////////////
static double LimitToneValue(double v) {
  return (v > 255) ? 255 : (v < -255) ? -255 : v;
}

///////////////////////////////////////////////////////////
// ARGSS Tone instance methods
///////////////////////////////////////////////////////////
VALUE ARGSS::ATone::rinitialize(int argc, VALUE* argv, VALUE self) {
  if (argc < 3) raise_argn(argc, 3);
  else if (argc > 4) raise_argn(argc, 4);
  set_ptr(self, boost::make_shared<Tone>(LimitToneValue(NUM2DBL(argv[0])),
                                         LimitToneValue(NUM2DBL(argv[1])),
                                         LimitToneValue(NUM2DBL(argv[2]))));
  get<Tone>(self).gray = argc == 4? LimitToneValue(NUM2DBL(argv[3])) : 0;
  return self;
}
VALUE ARGSS::ATone::rset(int argc, VALUE* argv, VALUE self) {
  if (argc < 3) raise_argn(argc, 3);
  else if (argc > 4) raise_argn(argc, 4);
  get<Tone>(self).red = LimitToneValue(NUM2DBL(argv[0]));
  get<Tone>(self).green = LimitToneValue(NUM2DBL(argv[1]));
  get<Tone>(self).blue = LimitToneValue(NUM2DBL(argv[2]));
  get<Tone>(self).gray = argc == 4? LimitToneValue(NUM2DBL(argv[3])) : 0;
  return self;
}
VALUE ARGSS::ATone::rred(VALUE self) {
  return rb_float_new(get<Tone>(self).red);
}
VALUE ARGSS::ATone::rredE(VALUE self, VALUE r) {
  return get<Tone>(self).red = NUM2DBL(r), r;
}
VALUE ARGSS::ATone::rgreen(VALUE self) {
  return rb_float_new(get<Tone>(self).green);
}
VALUE ARGSS::ATone::rgreenE(VALUE self, VALUE g) {
  return get<Tone>(self).red = NUM2DBL(g), g;
}
VALUE ARGSS::ATone::rblue(VALUE self) {
  return rb_float_new(get<Tone>(self).blue);
}
VALUE ARGSS::ATone::rblueE(VALUE self, VALUE b) {
  return get<Tone>(self).red = NUM2DBL(b), b;
}
VALUE ARGSS::ATone::rgray(VALUE self) {
  return rb_float_new(get<Tone>(self).gray);
}
VALUE ARGSS::ATone::rgrayE(VALUE self, VALUE g) {
  return get<Tone>(self).red = NUM2DBL(g), g;
}
VALUE ARGSS::ATone::rinspect(VALUE self) {
  char str[255];
  long str_size = sprintf(
    str, "(%f, %f, %f, %f)",
    get<Tone>(self).red, get<Tone>(self).green, get<Tone>(self).blue, get<Tone>(self).gray);
  return rb_str_new(str, str_size);
}
VALUE ARGSS::ATone::rdump(int argc, VALUE* /* argv */, VALUE self) {
  if (argc > 1) raise_argn(argc, 1);
  Tone const& t = get<Tone>(self);
  VALUE arr = rb_ary_new3(4, rb_float_new(t.red), rb_float_new(t.green),
                          rb_float_new(t.blue), rb_float_new(t.gray));
  return rb_funcall(arr, rb_intern("pack"), 1, rb_str_new2("e4"));
}

///////////////////////////////////////////////////////////
// ARGSS Tone class methods
///////////////////////////////////////////////////////////
VALUE ARGSS::ATone::rload(VALUE /* self */, VALUE str) {
  VALUE arr = rb_funcall(str, rb_intern("unpack"), 1, rb_str_new2("e4"));
  return create(boost::make_shared<Tone>
                (NUM2DBL(rb_ary_entry(arr, 0)), NUM2DBL(rb_ary_entry(arr, 1)),
                 NUM2DBL(rb_ary_entry(arr, 2)), NUM2DBL(rb_ary_entry(arr, 3))));
}

///////////////////////////////////////////////////////////
// ARGSS Tone initialize
///////////////////////////////////////////////////////////
void ARGSS::ATone::Init() {
  rb_method const methods[] = {
    rb_method("initialize", rinitialize),
    rb_method("set", rset),
    rb_method("red", rred),
    rb_method("red=", rredE),
    rb_method("green", rgreen),
    rb_method("green=", rgreenE),
    rb_method("blue", rblue),
    rb_method("blue=", rblueE),
    rb_method("gray", rgray),
    rb_method("gray=", rgrayE),
    rb_method("inspect", rinspect),
    rb_method("_dump", rdump),
    rb_method("_load", rload, true),
    rb_method() };
  define_class<Tone, true>("Tone", methods);
}
