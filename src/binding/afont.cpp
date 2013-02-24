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
#include "binding/aruby.h"
#include "binding/afont.h"
#include "font.h"
#include "color.h"

namespace {
FontList to_font_list(VALUE const v) {
  FontList ret;
  if(rb_type(v) == T_ARRAY) {
    ret.reserve(RARRAY_LEN(v));
    for(int i = 0; i < RARRAY_LEN(v); ++i) {
      VALUE entry = rb_ary_entry(v, i);
      ret.push_back(std::string(RSTRING_PTR(entry), RSTRING_LEN(entry)));
    }
  } else if(rb_type(v) == T_STRING) {
    ret.push_back(std::string(RSTRING_PTR(v), RSTRING_LEN(v)));
  } else {
    assert(false);
  }
  return ret;
}

void sync_font_list(FontList& ls, VALUE const v) {
  ls = to_font_list(v);
}

VALUE default_name() {
  return rb_cv_get(ARGSS::ruby_class<Font>(), "@@default_name");
}

}

///////////////////////////////////////////////////////////
// ARGSS Font instance methods
///////////////////////////////////////////////////////////
VALUE ARGSS::AFont::rinitialize(int argc, VALUE* argv, VALUE self) {
  sync_font_list(Font::default_name, default_name());

  set_ptr(self, boost::make_shared<Font>());

  if (argc > 2) raise_argn(argc, 2);
  if (argc >= 1) {
    Check_Types2(argv[0], T_STRING, T_ARRAY);
    rb_iv_set(self, "@name", argv[0]);
    sync_font_list(get<Font>(self).name, argv[0]);
  } else {
    rb_iv_set(self, "@name", rb_obj_clone(default_name()));
  }
  if (argc == 2) {
    Check_Kind(argv[1], rb_cNumeric);
    get<Font>(self).size = NUM2INT(argv[1]);
  }
  return self;
}

VALUE ARGSS::AFont::rexistQ(VALUE self) {
  return BOOL2NUM(not get<Font>(self).Find().empty());
}
VALUE ARGSS::AFont::rname(VALUE self) {
  return rb_iv_get(self, "@name");
}
VALUE ARGSS::AFont::rnameE(VALUE self, VALUE name) {
  Check_Types2(name, T_STRING, T_ARRAY);
  sync_font_list(get<Font>(self).name, name);
  rb_iv_set(self, "@name", name);
  return name;
}
VALUE ARGSS::AFont::rsize(VALUE self) {
  return INT2NUM(get<Font>(self).size);
}
VALUE ARGSS::AFont::rsizeE(VALUE self, VALUE size) {
  Check_Kind(size, rb_cNumeric);
  return get<Font>(self).size = NUM2INT(size), size;
}
VALUE ARGSS::AFont::rbold(VALUE self) {
  return BOOL2NUM(get<Font>(self).bold);
}
VALUE ARGSS::AFont::rboldE(VALUE self, VALUE bold) {
  Check_Bool(bold);
  return get<Font>(self).bold = NUM2BOOL(bold), bold;
}
VALUE ARGSS::AFont::ritalic(VALUE self) {
  return BOOL2NUM(get<Font>(self).italic);
}
VALUE ARGSS::AFont::ritalicE(VALUE self, VALUE italic) {
  Check_Bool(italic);
  return get<Font>(self).italic = NUM2BOOL(italic), italic;
}
VALUE ARGSS::AFont::rcolor(VALUE self) {
  return create(get<Font>(self).color);
}
VALUE ARGSS::AFont::rcolorE(VALUE self, VALUE color) {
  Check_Class(color, ruby_class<Color>());
  return create(get<Font>(self).color);
}
VALUE ARGSS::AFont::rshadow(VALUE self) {
  return BOOL2NUM(get<Font>(self).shadow);
}
VALUE ARGSS::AFont::rshadowE(VALUE self, VALUE shadow) {
  Check_Bool(shadow);
  return get<Font>(self).shadow = NUM2BOOL(shadow), shadow;
}

///////////////////////////////////////////////////////////
// ARGSS Font class methods
///////////////////////////////////////////////////////////
VALUE ARGSS::AFont::rdefault_name(VALUE /* self */) {
  return rb_cv_get(ruby_class<Font>(), "@@default_name");
}
VALUE ARGSS::AFont::rdefault_nameE(VALUE /* self */, VALUE default_name) {
  Check_Types2(default_name, T_STRING, T_ARRAY);
  rb_cv_set(ruby_class<Font>(), "@@default_name", default_name);
  return default_name;
}
VALUE ARGSS::AFont::rdefault_size(VALUE /* self */) {
  return INT2NUM(Font::default_size);
}
VALUE ARGSS::AFont::rdefault_sizeE(VALUE /* self */, VALUE default_size) {
  Check_Kind(default_size, rb_cNumeric);
  return Font::default_size = NUM2INT(default_size), default_size;
}
VALUE ARGSS::AFont::rdefault_bold(VALUE /* self */) {
  return BOOL2NUM(Font::default_bold);
}
VALUE ARGSS::AFont::rdefault_boldE(VALUE /* self */, VALUE default_bold) {
  Check_Bool(default_bold);
  return Font::default_bold = NUM2BOOL(default_bold), default_bold;
}
VALUE ARGSS::AFont::rdefault_italic(VALUE /* self */) {
  return BOOL2NUM(Font::default_italic);
}
VALUE ARGSS::AFont::rdefault_italicE(VALUE /* self */, VALUE default_italic) {
  Check_Bool(default_italic);
  return Font::default_italic = NUM2BOOL(default_italic), default_italic;
}
VALUE ARGSS::AFont::rdefault_color(VALUE /* self */) {
  return create(Font::default_color);
}
VALUE ARGSS::AFont::rdefault_colorE(VALUE /* self */, VALUE default_color) {
  Check_Class(default_color, ruby_class<Color>());
  return Font::default_color = get_ptr<Color>(default_color), default_color;
}
VALUE ARGSS::AFont::rdefault_shadow(VALUE /* self */) {
  return BOOL2NUM(Font::default_shadow);
}
VALUE ARGSS::AFont::rdefault_shadowE(VALUE /* self */, VALUE default_shadow) {
  Check_Bool(default_shadow);
  return Font::default_shadow = NUM2BOOL(default_shadow), default_shadow;
}

///////////////////////////////////////////////////////////
/// ARGSS Font initialize
///////////////////////////////////////////////////////////
void ARGSS::AFont::Init() {
  rb_method const methods[] = {
    rb_method("initialize", rinitialize),
    rb_method("exist?", rexistQ),
    rb_method("name", rname),
    rb_method("name=", rnameE),
    rb_method("size", rsize),
    rb_method("size=", rsizeE),
    rb_method("bold", rbold),
    rb_method("bold=", rboldE),
    rb_method("italic", ritalic),
    rb_method("italic=", ritalicE),
    rb_method("color", rcolor),
    rb_method("color=", rcolorE),
    rb_method("shadow", rshadow),
    rb_method("shadow=", rshadowE),
    rb_method("default_name", rdefault_name, true),
    rb_method("default_name=", rdefault_nameE, true),
    rb_method("default_size", rdefault_size, true),
    rb_method("default_size=", rdefault_sizeE, true),
    rb_method("default_bold", rdefault_bold, true),
    rb_method("default_bold=", rdefault_boldE, true),
    rb_method("default_italic", rdefault_italic, true),
    rb_method("default_italic=", rdefault_italicE, true),
    rb_method("default_color", rdefault_color, true),
    rb_method("default_color=", rdefault_colorE, true),
    rb_method("default_shadow", rdefault_shadow, true),
    rb_method("default_shadow=", rdefault_shadowE, true),
    rb_method()
  };
  define_class<Font, true>("Font", methods);
}
