//////////////////////////////////////////////////////////////////////////////////
/// ARGSS - Copyright (c) 2009 - 2010, Alejandro Marzini (vgvgf)
///         All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
///     * Redistributions of source code must retain the above copyright
///       notice, this list of conditions and the following disclaimer.
///     * Redistributions in binary form must reproduce the above copyright
///       notice, this list of conditions and the following disclaimer in the
///       documentation and/or other materials provided with the distribution.
///
/// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ''AS IS'' AND ANY
/// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
/// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
/// DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
/// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
/// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
/// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
/// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
/// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
/// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include "argss_rpg.h"
#include "argss_rpg_weather.h"
#include "argss_color.h"
#include "argss_rect.h"
#include "argss_bitmap.h"
#include "argss_sprite.h"

////////////////////////////////////////////////////////////
/// Global Variables
////////////////////////////////////////////////////////////
VALUE ARGSS::ARPG::AWeather::id;

////////////////////////////////////////////////////////////
/// ARGSS RPG::Weather ruby functions
////////////////////////////////////////////////////////////
static VALUE argss_rpg_weather_initialize(int argc, VALUE* argv, VALUE self) {
	if (argc > 1) raise_argn(argc, 1);
	VALUE viewport = Qnil;
	if (argc == 1) viewport = argv[0];
	rb_iv_set(self, "@type", INT2NUM(0));
	rb_iv_set(self, "@max", INT2NUM(0));
	rb_iv_set(self, "@ox", INT2NUM(0));
	rb_iv_set(self, "@oy", INT2NUM(0));
	VALUE color1 = ARGSS::AColor::New(255, 255, 255, 255);
	VALUE color2 = ARGSS::AColor::New(255, 255, 255, 128);
	VALUE args[2] = {INT2NUM(7), INT2NUM(56)};
	VALUE rain_bitmap = rb_iv_set(self, "@rain_bitmap", rb_class_new_instance(2, args, ARGSS::ABitmap::id));
	for (int i = 0; i < 7; i++) {
		rb_funcall(rain_bitmap, rb_intern("fill_rect"), 5, INT2NUM(6 - i), INT2NUM(i * 8), INT2NUM(1), INT2NUM(8), color1);
	}
	args[0] = INT2NUM(34);
	args[1] = INT2NUM(64);
	VALUE storm_bitmap = rb_iv_set(self, "@storm_bitmap", rb_class_new_instance(2, args, ARGSS::ABitmap::id));
	for (int i = 0; i < 32; i++) {
		rb_funcall(storm_bitmap, rb_intern("fill_rect"), 5, INT2NUM(33 - i), INT2NUM(i * 2), INT2NUM(1), INT2NUM(2), color2);
		rb_funcall(storm_bitmap, rb_intern("fill_rect"), 5, INT2NUM(32 - i), INT2NUM(i * 2), INT2NUM(1), INT2NUM(2), color1);
		rb_funcall(storm_bitmap, rb_intern("fill_rect"), 5, INT2NUM(31 - i), INT2NUM(i * 2), INT2NUM(1), INT2NUM(2), color2);
	}
	args[0] = INT2NUM(6);
	args[1] = INT2NUM(6);
	VALUE snow_bitmap = rb_iv_set(self, "@snow_bitmap", rb_class_new_instance(2, args, ARGSS::ABitmap::id));
	rb_funcall(snow_bitmap, rb_intern("fill_rect"), 5, INT2NUM(0), INT2NUM(1), INT2NUM(6), INT2NUM(4), color2);
	rb_funcall(snow_bitmap, rb_intern("fill_rect"), 5, INT2NUM(1), INT2NUM(0), INT2NUM(4), INT2NUM(6), color2);
	rb_funcall(snow_bitmap, rb_intern("fill_rect"), 5, INT2NUM(1), INT2NUM(2), INT2NUM(4), INT2NUM(2), color2);
	rb_funcall(snow_bitmap, rb_intern("fill_rect"), 5, INT2NUM(2), INT2NUM(1), INT2NUM(2), INT2NUM(4), color2);
	VALUE sprites = rb_iv_set(self, "@sprites", rb_ary_new());
	for (int i = 0; i < 40; i++) {
		VALUE sprite = rb_class_new_instance(1, &viewport, ARGSS::ASprite::id);
		rb_funcall(sprite, rb_intern("z="), 1, INT2NUM(1000));
		rb_funcall(sprite, rb_intern("visible="), 1, Qfalse);
		rb_funcall(sprite, rb_intern("opacity="), 1, INT2NUM(0));
		rb_ary_push(sprites, sprite);
	}
	return self;
}
static VALUE argss_rpg_weather_dispose(VALUE self) {
	VALUE sprites = rb_iv_get(self, "@sprites");
	for (int i = 0; i < RARRAY(sprites)->len; i++) {
		rb_funcall(rb_ary_entry(sprites, i), rb_intern("dispose"), 0);
	}
	rb_funcall(rb_iv_get(self, "@rain_bitmap"), rb_intern("dispose"), 0);
	rb_funcall(rb_iv_get(self, "@storm_bitmap"), rb_intern("dispose"), 0);
	rb_funcall(rb_iv_get(self, "@snow_bitmap"), rb_intern("dispose"), 0);
	return Qnil;
}
static VALUE argss_rpg_weather_typeE(VALUE self, VALUE type) {
	if (rb_iv_get(self, "@type") == type) return Qnil;
	rb_iv_set(self, "@type", type);
	VALUE bitmap;
	if (type == INT2NUM(1)) {
		bitmap = rb_iv_get(self, "@rain_bitmap");
	}
	else if (type == INT2NUM(2)) {
		bitmap = rb_iv_get(self, "@storm_bitmap");
	}
	else if (type == INT2NUM(3)) {
		bitmap = rb_iv_get(self, "@snow_bitmap");
	}
	else {
		bitmap = Qnil;
	}
	int val_max = NUM2INT(rb_iv_get(self, "@max"));
	VALUE sprites = rb_iv_get(self, "@sprites");
	for (int i = 0; i < 40; i++) {
		VALUE sprite = rb_ary_entry(sprites, i);
		if (sprite != Qnil) {
			rb_funcall(sprite, rb_intern("visible="), 1, BOOL2NUM(i <= val_max));
			rb_funcall(sprite, rb_intern("bitmap="), 1, bitmap);
		}
	}
	return Qnil;
}
static VALUE argss_rpg_weather_oxE(VALUE self, VALUE ox) {
	if (rb_iv_get(self, "@ox") == ox) return Qnil;
	rb_iv_set(self, "@ox", ox);
	VALUE sprites = rb_iv_get(self, "@sprites");
	for (int i = 0; i < 40; i++) {
		VALUE sprite = rb_ary_entry(sprites, i);
		if (sprite != Qnil) {
			rb_funcall(sprite, rb_intern("ox="), 1, ox);
		}
	}
	return Qnil;
}
static VALUE argss_rpg_weather_oyE(VALUE self, VALUE oy) {
	if (rb_iv_get(self, "@oy") == oy) return Qnil;
	rb_iv_set(self, "@oy", oy);
	VALUE sprites = rb_iv_get(self, "@sprites");
	for (int i = 0; i < 40; i++) {
		VALUE sprite = rb_ary_entry(sprites, i);
		if (sprite != Qnil) {
			rb_funcall(sprite, rb_intern("oy="), 1, oy);
		}
	}
	return Qnil;
}
static VALUE argss_rpg_weather_maxE(VALUE self, VALUE max) {
	if (rb_iv_get(self, "@max") == max) return Qnil;
	int val_max = NUM2INT(max);
	if (val_max > 4) val_max = 40;
	if (val_max < 0) val_max = 0;
	rb_iv_set(self, "@max", INT2NUM(val_max));
	VALUE sprites = rb_iv_get(self, "@sprites");
	for (int i = 0; i < 40; i++) {
		VALUE sprite = rb_ary_entry(sprites, i);
		if (sprite != Qnil) {
			rb_funcall(sprite, rb_intern("visible="), 1, BOOL2NUM(i <= val_max));
		}
	}
	return Qnil;
}
static VALUE argss_rpg_weather_update(VALUE self) {
	int val_type = NUM2INT(rb_iv_get(self, "@type"));
	if (val_type == 0) return Qnil;
	for (int i = 0; i < NUM2INT(rb_iv_get(self, "@max")); i++) {
		VALUE sprite = rb_ary_entry(rb_iv_get(self, "@sprites"), i);
		if (sprite == Qnil) continue;
		int val_x = NUM2INT(rb_iv_get(self, "@x"));
		int val_y = NUM2INT(rb_iv_get(self, "@y"));
		int val_opacity = NUM2INT(rb_iv_get(self, "@opacity"));
		if (val_type == 1) {
			val_x -= 2;
			val_y += 16;
			val_opacity -= 8;
		}
		else if (val_type == 2) {
			val_x -= 8;
			val_y += 16;
			val_opacity -= 12;
		}
		else if (val_type == 3) {
			val_x -= 2;
			val_y += 8;
			val_opacity -= 8;
		}
		int x = val_x - NUM2INT(rb_iv_get(self, "@ox"));
		int y = val_y - NUM2INT(rb_iv_get(self, "@oy"));
		if (val_opacity < 64 || x < -50 || x > 750 || y < -300 || y > 500) {
		  val_x = rand() % 800 - 50 + NUM2INT(rb_iv_get(self, "@ox"));
		  val_y = rand() % 800 - 200 + NUM2INT(rb_iv_get(self, "@oy"));
		  val_opacity = 255;
		}
		rb_funcall(sprite, rb_intern("x="), 1, INT2NUM(val_x));
		rb_funcall(sprite, rb_intern("y="), 1, INT2NUM(val_y));
		rb_funcall(sprite,rb_intern( "opacity="), 1, INT2NUM(val_opacity));
	}
	return Qnil;
}

////////////////////////////////////////////////////////////
/// ARGSS RPG::Weather initialize
////////////////////////////////////////////////////////////
void ARGSS::ARPG::AWeather::Init() {
    typedef VALUE (*rubyfunc)(...);
	id = rb_define_class_under(ARGSS::ARPG::id, "Weather", rb_cObject);
	rb_define_method(id, "initialize", (rubyfunc)argss_rpg_weather_initialize, -1);
	rb_define_method(id, "dispose", (rubyfunc)argss_rpg_weather_dispose, 0);
	rb_define_method(id, "type=", (rubyfunc)argss_rpg_weather_typeE, 1);
	rb_define_method(id, "ox=", (rubyfunc)argss_rpg_weather_oxE, 1);
	rb_define_method(id, "oy=", (rubyfunc)argss_rpg_weather_oyE, 1);
	rb_define_method(id, "max=", (rubyfunc)argss_rpg_weather_maxE, 1);
	rb_define_method(id, "update", (rubyfunc)argss_rpg_weather_update, 0);

	rb_define_attr(id, "type", 1, 0);
	rb_define_attr(id, "ox", 1, 0);
	rb_define_attr(id, "oy", 1, 0);
	rb_define_attr(id, "max", 1, 0);
}
