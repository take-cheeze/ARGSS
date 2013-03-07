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
#include "graphics.h"
#include "binding/atilemap_xp.h"
#include "binding/atilemapautotiles_xp.h"
#include "binding/abitmap.h"
#include "binding/aerror.h"
#include "binding/atable.h"
#include "binding/aviewport.h"
#include "tilemap_xp.h"
#include "output.h"

///////////////////////////////////////////////////////////
// ARGSS Tilemap instance methods
///////////////////////////////////////////////////////////
VALUE ARGSS::ATilemap::rinitialize(int argc, VALUE* argv, VALUE self) {
  set_ptr(self, boost::make_shared<Tilemap>());

  if (argc == 1) {
    Check_Classes_N(argv[0], ruby_class<Viewport>());
    get<Tilemap>(self).viewport(get_ptr<Viewport>(argv[0]));
  } else if (argc == 0) {
  } else raise_argn(argc, 1);
  rb_iv_set(self, "@autotiles", ATilemapAutotiles::New(get_ptr<Tilemap>(self)));
  return self;
}
VALUE ARGSS::ATilemap::rdispose(VALUE self) {
  if (get_ptr<Tilemap>(self)) {
    get_ptr<Tilemap>(self).reset();
  }
  return self;
}
VALUE ARGSS::ATilemap::rdisposeQ(VALUE self) {
  return BOOL2NUM(not get_ptr<Tilemap>(self));
}
VALUE ARGSS::ATilemap::rupdate(VALUE self) {
  check_disposed<Tilemap>(self);
  get<Tilemap>(self).Update();
  return Qnil;
}
VALUE ARGSS::ATilemap::rviewport(VALUE self) {
  check_disposed<Tilemap>(self);
  return create(get<Tilemap>(self).viewport());
}
VALUE ARGSS::ATilemap::rviewportE(VALUE self, VALUE viewport) {
  check_disposed<Tilemap>(self);
  Check_Classes_N(viewport, ruby_class<Viewport>());
  get<Tilemap>(self).viewport(get_ptr<Viewport>(viewport));
  return viewport;
}
VALUE ARGSS::ATilemap::rtileset(VALUE self) {
  check_disposed<Tilemap>(self);
  return create(get<Tilemap>(self).tileset);
}
VALUE ARGSS::ATilemap::rtilesetE(VALUE self, VALUE tileset) {
  check_disposed<Tilemap>(self);
  Check_Classes_N(tileset, ruby_class<Bitmap>());
  get<Tilemap>(self).tileset = get_ptr<Bitmap>(tileset);
  return tileset;
}
VALUE ARGSS::ATilemap::rautotiles(VALUE self) {
  check_disposed<Tilemap>(self);
  return rb_iv_get(self, "@autotiles");
}
VALUE ARGSS::ATilemap::rmap_data(VALUE self) {
  check_disposed<Tilemap>(self);
  return create(get<Tilemap>(self).map_data());
}
VALUE ARGSS::ATilemap::rmap_dataE(VALUE self, VALUE map_data) {
  check_disposed<Tilemap>(self);
  Check_Classes_N(map_data, ruby_class<Table>());
  get<Tilemap>(self).map_data(get_ptr<Table>(map_data));
  return map_data;
}
VALUE ARGSS::ATilemap::rflash_data(VALUE self) {
  check_disposed<Tilemap>(self);
  return create(get<Tilemap>(self).flash_data);
}
VALUE ARGSS::ATilemap::rflash_dataE(VALUE self, VALUE flash_data) {
  check_disposed<Tilemap>(self);
  Check_Classes_N(flash_data, ruby_class<Table>());
  get<Tilemap>(self).flash_data = get_ptr<Table>(flash_data);
  return flash_data;
}
VALUE ARGSS::ATilemap::rpriorities(VALUE self) {
  check_disposed<Tilemap>(self);
  return create(get<Tilemap>(self).priorities);
}
VALUE ARGSS::ATilemap::rprioritiesE(VALUE self, VALUE priorities) {
  check_disposed<Tilemap>(self);
  Check_Classes_N(priorities, ruby_class<Table>());
  get<Tilemap>(self).priorities = get_ptr<Table>(priorities);
  return priorities;
}
VALUE ARGSS::ATilemap::rvisible(VALUE self) {
  check_disposed<Tilemap>(self);
  return BOOL2NUM(get<Tilemap>(self).visible);
}
VALUE ARGSS::ATilemap::rvisibleE(VALUE self, VALUE visible) {
  check_disposed<Tilemap>(self);
  return get<Tilemap>(self).visible = NUM2BOOL(visible), visible;
}
VALUE ARGSS::ATilemap::rox(VALUE self) {
  check_disposed<Tilemap>(self);
  return INT2NUM(get<Tilemap>(self).ox);
}
VALUE ARGSS::ATilemap::roxE(VALUE self, VALUE ox) {
  check_disposed<Tilemap>(self);
  return get<Tilemap>(self).ox = NUM2INT(ox), ox;
}
VALUE ARGSS::ATilemap::roy(VALUE self) {
  check_disposed<Tilemap>(self);
  return INT2NUM(get<Tilemap>(self).oy);
}
VALUE ARGSS::ATilemap::royE(VALUE self, VALUE oy) {
  check_disposed<Tilemap>(self);
  return get<Tilemap>(self).oy = NUM2INT(oy), oy;
}

///////////////////////////////////////////////////////////
// ARGSS Tilemap initialize
///////////////////////////////////////////////////////////
void ARGSS::ATilemap::Init() {
  ATilemapAutotiles::Init();

  rb_method const methods[] = {
    rb_method("initialize", rinitialize),
    rb_method("dispose", rdispose),
    rb_method("disposed?", rdisposeQ),
    rb_method("update", rupdate),
    rb_method("viewport", rviewport),
    rb_method("viewport=", rviewportE),
    rb_method("tileset", rtileset),
    rb_method("tileset=", rtilesetE),
    rb_method("autotiles", rautotiles),
    rb_method("map_data", rmap_data),
    rb_method("map_data=", rmap_dataE),
    rb_method("flash_data", rflash_data),
    rb_method("flash_data=", rflash_dataE),
    rb_method("priorities", rpriorities),
    rb_method("priorities=", rprioritiesE),
    rb_method("visible", rvisible),
    rb_method("visible=", rvisibleE),
    rb_method("ox", rox),
    rb_method("ox=", roxE),
    rb_method("oy", roy),
    rb_method("oy=", royE),
    rb_method() };
  define_class<Tilemap, false>("Tilemap", methods);
}
