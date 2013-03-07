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
#include "binding/rpg.h"
#include "binding/abitmap.h"
#include "binding/rpg_cache.h"
#include "bitmap.h"
#include "rect.h"

VALUE ARGSS::ARPG::ACache::id = Qnil;

///////////////////////////////////////////////////////////
// ARGSS RPG::Cache module methods
///////////////////////////////////////////////////////////
VALUE ARGSS::ARPG::ACache::rload_bitmap(int argc, VALUE* argv, VALUE self) {
  if (argc > 3) raise_argn(argc, 3);
  else if (argc < 2) raise_argn(argc, 2);
  VALUE hue = argv[2];
  if (argc == 2) hue = INT2NUM(0);
  VALUE path = rb_str_concat(rb_str_dup(argv[0]), argv[1]);
  VALUE cache = rb_iv_get(self, "@cache");
  VALUE cache_path;
  if ((rb_funcall(cache, rb_intern("include?"), 1, path) == Qfalse) ||
    (ARGSS::ABitmap::rdisposedQ(rb_hash_aref(cache, path)) == Qtrue)) {
    if (RSTRING_LEN(argv[0]) > 0) {
      cache_path = rb_hash_aset(cache, path, rb_class_new_instance(1, &path, ruby_class<Bitmap>()));
    } else {
      VALUE args[2] = {INT2NUM(32), INT2NUM(32)};
      cache_path = rb_hash_aset(cache, path, rb_class_new_instance(2, args, ruby_class<Bitmap>()));
    }
  } else {
    cache_path = rb_hash_aref(cache, path);
  }
  if (hue == INT2NUM(0)) {
    return cache_path;
  } else {
    VALUE key = rb_ary_new3(2, path, hue);
    VALUE cache_key;
    if ((rb_funcall(cache, rb_intern("include?"), 1, key) == Qfalse) ||
      (ARGSS::ABitmap::rdisposedQ(rb_hash_aref(cache, key)) == Qtrue)) {
      cache_key = rb_hash_aset(cache, key, rb_obj_clone(cache_path));
      ARGSS::ABitmap::rhue_change(cache_key, hue);
    } else {
      cache_key = rb_hash_aref(cache, key);
    }
    return cache_key;
  }
}
VALUE ARGSS::ARPG::ACache::ranimation(VALUE self, VALUE filename, VALUE hue) {
  VALUE args[3] = {rb_str_new2("Graphics/Animations/"), filename, hue};
  return rload_bitmap(3, args, self);
}
VALUE ARGSS::ARPG::ACache::rautotile(VALUE self, VALUE filename) {
  VALUE args[3] = {rb_str_new2("Graphics/Autotiles/"), filename, INT2NUM(0)};
  return rload_bitmap(3, args, self);
}
VALUE ARGSS::ARPG::ACache::rbattleback(VALUE self, VALUE filename) {
  VALUE args[3] = {rb_str_new2("Graphics/Battlebacks/"), filename, INT2NUM(0)};
  return rload_bitmap(3, args, self);
}
VALUE ARGSS::ARPG::ACache::rbattler(VALUE self, VALUE filename, VALUE hue) {
  VALUE args[3] = {rb_str_new2("Graphics/Battlers/"), filename, hue};
  return rload_bitmap(3, args, self);
}
VALUE ARGSS::ARPG::ACache::rcharacter(VALUE self, VALUE filename, VALUE hue) {
  VALUE args[3] = {rb_str_new2("Graphics/Characters/"), filename, hue};
  return rload_bitmap(3, args, self);
}
VALUE ARGSS::ARPG::ACache::rfog(VALUE self, VALUE filename, VALUE hue) {
  VALUE args[3] = {rb_str_new2("Graphics/Fogs/"), filename, hue};
  return rload_bitmap(3, args, self);
}
VALUE ARGSS::ARPG::ACache::rgameover(VALUE self, VALUE filename) {
  VALUE args[3] = {rb_str_new2("Graphics/Gameovers/"), filename, INT2NUM(0)};
  return rload_bitmap(3, args, self);
}
VALUE ARGSS::ARPG::ACache::ricon(VALUE self, VALUE filename) {
  VALUE args[3] = {rb_str_new2("Graphics/Icons/"), filename, INT2NUM(0)};
  return rload_bitmap(3, args, self);
}
VALUE ARGSS::ARPG::ACache::rpanorama(VALUE self, VALUE filename, VALUE hue) {
  VALUE args[3] = {rb_str_new2("Graphics/Panoramas/"), filename, hue};
  return rload_bitmap(3, args, self);
}
VALUE ARGSS::ARPG::ACache::rpicture(VALUE self, VALUE filename) {
  VALUE args[3] = {rb_str_new2("Graphics/Pictures/"), filename, INT2NUM(0)};
  return rload_bitmap(3, args, self);
}
VALUE ARGSS::ARPG::ACache::rtileset(VALUE self, VALUE filename) {
  VALUE args[3] = {rb_str_new2("Graphics/Tilesets/"), filename, INT2NUM(0)};
  return rload_bitmap(3, args, self);
}
VALUE ARGSS::ARPG::ACache::rtitle(VALUE self, VALUE filename) {
  VALUE args[3] = {rb_str_new2("Graphics/Titles/"), filename, INT2NUM(0)};
  return rload_bitmap(3, args, self);
}
VALUE ARGSS::ARPG::ACache::rwindowskin(VALUE self, VALUE filename) {
  VALUE args[3] = {rb_str_new2("Graphics/Windowskins/"), filename, INT2NUM(0)};
  return rload_bitmap(3, args, self);
}
VALUE ARGSS::ARPG::ACache::rtile(VALUE self, VALUE filename, VALUE tile_id, VALUE hue) {
  VALUE key = rb_ary_new3(3, filename, tile_id, hue);
  VALUE cache = rb_iv_get(self, "@cache");
  VALUE cache_key;
  if ((rb_funcall(cache, rb_intern("include?"), 1, key) == Qfalse) ||
    (ARGSS::ABitmap::rdisposedQ(rb_hash_aref(cache, key)) == Qtrue)) {
    VALUE args[2] = {INT2NUM(32), INT2NUM(32)};
    cache_key = rb_hash_aset(cache, key, rb_class_new_instance(2, args, ruby_class<Bitmap>()));
    double x = (NUM2INT(tile_id) - 384) % 8 * 32;
    double y = (NUM2INT(tile_id) - 384) / 8 * 32;
    VALUE rect = create(boost::make_shared<Rect>(x, y, 32, 32));
    VALUE values[4] = {INT2NUM(0), INT2NUM(0), rtileset(self, filename), rect};
    ARGSS::ABitmap::rblt(4, values, cache_key);
    ARGSS::ABitmap::rhue_change(cache_key, hue);
  } else {
    cache_key = rb_hash_aref(cache, key);
  }
  return cache_key;
}
VALUE ARGSS::ARPG::ACache::rclear(VALUE self) {
  VALUE cache = rb_iv_get(self, "@cache");
  VALUE values = rb_funcall(cache, rb_intern("values"), 0);
  for (int i = 0; i < RARRAY_LEN(values); i++) {
    if (ARGSS::ABitmap::rdisposedQ(rb_ary_entry(values, i)) == Qfalse) {
      ARGSS::ABitmap::rdispose(rb_ary_entry(values, i));
    }
  }
  rb_iv_set(self, "@cache", rb_hash_new());
  return rb_gc_start();
}

///////////////////////////////////////////////////////////
// ARGSS RPG::Cache initialize
///////////////////////////////////////////////////////////
void ARGSS::ARPG::ACache::Init() {
  rb_method const methods[] = {
    rb_method("load_bitmap", rload_bitmap, true),
    rb_method("animation", ranimation, true),
    rb_method("autotile", rautotile, true),
    rb_method("battleback", rbattleback, true),
    rb_method("battler", rbattler, true),
    rb_method("character", rcharacter, true),
    rb_method("fog", rfog, true),
    rb_method("gameover", rgameover, true),
    rb_method("icon", ricon, true),
    rb_method("panorama", rpanorama, true),
    rb_method("picture", rpicture, true),
    rb_method("tileset", rtileset, true),
    rb_method("title", rtitle, true),
    rb_method("windowskin", rwindowskin, true),
    rb_method("tile", rtile, true),
    rb_method("clear", rclear, true),
    rb_method() };
  id = define_module_under(ARGSS::ARPG::id, "Cache", methods);
  rb_iv_set(id, "@cache", rb_hash_new());
}
