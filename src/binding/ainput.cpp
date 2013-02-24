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
#include "binding/ainput.h"
#include "input.h"

///////////////////////////////////////////////////////////
// ARGSS Input module methods
///////////////////////////////////////////////////////////
VALUE ARGSS::AInput::rupdate(VALUE /* self */) {
  Input::Update();
  return Qnil;
}
VALUE ARGSS::AInput::rpressQ(VALUE /* self */, VALUE button) {
  Check_Type(button, T_FIXNUM);
  return BOOL2NUM(Input::IsPressed(NUM2INT(button)));
}
VALUE ARGSS::AInput::rtriggerQ(VALUE /* self */, VALUE button) {
  Check_Type(button, T_FIXNUM);
  return BOOL2NUM(Input::IsTriggered(NUM2INT(button)));
}
VALUE ARGSS::AInput::rrepeatQ(VALUE /* self */, VALUE button) {
  Check_Type(button, T_FIXNUM);
  return BOOL2NUM(Input::IsRepeated(NUM2INT(button)));
}
VALUE ARGSS::AInput::rreleaseQ(VALUE /* self */, VALUE button) {
  Check_Type(button, T_FIXNUM);
  return BOOL2NUM(Input::IsReleased(NUM2INT(button)));
}
VALUE ARGSS::AInput::rdir4(VALUE /* self */) {
  return INT2NUM(Input::dir4);
}
VALUE ARGSS::AInput::rdir8(VALUE /* self */) {
  return INT2NUM(Input::dir8);
}

static VALUE to_ruby(std::vector<Input::rgss_key> const& key) {
  VALUE ret = rb_ary_new2(key.size());
  for(size_t i = 0; i < key.size(); ++i) {
    rb_ary_store(ret, i, INT2NUM(key[i].value));
  }
  return ret;
}

VALUE ARGSS::AInput::rpressed(VALUE /* self */) {
  return to_ruby(Input::GetPressed());
}
VALUE ARGSS::AInput::rtriggered(VALUE /* self */) {
  return to_ruby(Input::GetTriggered());
}
VALUE ARGSS::AInput::rrepeated(VALUE /* self */) {
  return to_ruby(Input::GetRepeated());
}
VALUE ARGSS::AInput::rreleased(VALUE /* self */) {
  return to_ruby(Input::GetReleased());
}

///////////////////////////////////////////////////////////
// ARGSS Input initialize
///////////////////////////////////////////////////////////
void ARGSS::AInput::Init() {
  rb_method const methods[] = {
    rb_method("update", rupdate, true),
    rb_method("press?", rpressQ, true),
    rb_method("trigger?", rtriggerQ, true),
    rb_method("repeat?", rrepeatQ, true),
    rb_method("release?", rreleaseQ, true),
    rb_method("dir4", rdir4, true),
    rb_method("dir8", rdir8, true),
    rb_method("pressed", rpressed, true),
    rb_method("triggered", rtriggered, true),
    rb_method("repeated", rrepeated, true),
    rb_method("released", rreleased, true),
    rb_method() };
  VALUE const id = define_module("Input", methods);

  rb_define_const(id, "DOWN", INT2FIX(2));
  rb_define_const(id, "LEFT", INT2FIX(4));
  rb_define_const(id, "RIGHT", INT2FIX(6));
  rb_define_const(id, "UP", INT2FIX(8));
  rb_define_const(id, "A", INT2FIX(11));
  rb_define_const(id, "B", INT2FIX(12));
  rb_define_const(id, "C", INT2FIX(13));
  rb_define_const(id, "X", INT2FIX(14));
  rb_define_const(id, "Y", INT2FIX(15));
  rb_define_const(id, "Z", INT2FIX(16));
  rb_define_const(id, "L", INT2FIX(17));
  rb_define_const(id, "R", INT2FIX(18));
  rb_define_const(id, "SHIFT", INT2FIX(21));
  rb_define_const(id, "CTRL", INT2FIX(22));
  rb_define_const(id, "ALT", INT2FIX(23));
  rb_define_const(id, "F5", INT2FIX(25));
  rb_define_const(id, "F6", INT2FIX(26));
  rb_define_const(id, "F7", INT2FIX(27));
  rb_define_const(id, "F8", INT2FIX(28));
  rb_define_const(id, "F9", INT2FIX(29));
}
