#include "sprite.h"
#include <string>
#include "../argss.h"
#include "../sfmlargss.h"
#include "aerror.h"
#include "rect.h"
#include "color.h"
#include "tone.h"
#include "bitmap.h"
#include "../sfml.h"

void argss_sprite_check(VALUE spr) {
    if (ARGSS_mapSprites.count(spr) == 0) {
        rb_raise(ARGSS_Error, "disposed sprite <%i>", spr);
    }
}

static VALUE argss_sprite_initialize(int argc, VALUE *argv, VALUE self) {
    if (argc == 1) {
        Check_Classes_N(argv[0], ARGSS_Viewport);
		rb_iv_set(self, "@viewport", argv[0]);
    }
    else if (argc == 0) {
        rb_iv_set(self, "@viewport", Qnil);
        ARGSS_CreationTime += 1;
        Z_Obj obj(0, ARGSS_CreationTime, ARGSS_Sprite, self);
        ARGSS_ZOrder.push_back(obj);
    }
    else {raise_argn(argc, 1);}
    ARGSS_mapSprites[self];
    rb_iv_set(self, "@bitmap", Qnil);
    rb_iv_set(self, "@src_rect", argss_rect_new2(0, 0, 0, 0));
    rb_iv_set(self, "@visible", Qtrue);
    rb_iv_set(self, "@x", INT2NUM(0));
    rb_iv_set(self, "@y", INT2NUM(0));
    rb_iv_set(self, "@z", INT2NUM(0));
    rb_iv_set(self, "@ox", INT2NUM(0));
    rb_iv_set(self, "@oy", INT2NUM(0));
    rb_iv_set(self, "@zoom_x", rb_float_new(1.0f));
    rb_iv_set(self, "@zoom_y", rb_float_new(1.0f));
    rb_iv_set(self, "@angle", rb_float_new(0.0f));
    rb_iv_set(self, "@flipx", Qfalse);
    rb_iv_set(self, "@flipy", Qfalse);
    rb_iv_set(self, "@bush_depth", INT2NUM(0));
    rb_iv_set(self, "@opacity", INT2NUM(255));
    rb_iv_set(self, "@blend_type", INT2NUM(0));
    rb_iv_set(self, "@color", argss_color_new2(0.0f, 0.0f, 0.0f, 0.0f));
    rb_iv_set(self, "@tone", argss_tone_new2(0.0f, 0.0f, 0.0f, 0.0f));
    return self;
}
static VALUE argss_sprite_dispose(VALUE self) {
    argss_sprite_check(self);
    ARGSS_mapSprites.erase(self);
    ARGSS_ZOrder.remove_if(remove_zobj_id(self));
    return self;
}
static VALUE argss_sprite_disposedQ(VALUE self) {
    return INT2BOOL(ARGSS_mapSprites.count(self) == 0);
}
static VALUE argss_sprite_flash(VALUE self, VALUE color, VALUE duration) {
    argss_sprite_check(self);
    //ToDo
    return Qnil;
}
static VALUE argss_sprite_update(VALUE self) {
    argss_sprite_check(self);
    //ToDo
    return Qnil;
}
static VALUE argss_sprite_viewport(VALUE self) {
    argss_sprite_check(self);
    return rb_iv_get(self, "@viewport");
}
static VALUE argss_sprite_viewportE(VALUE self, VALUE viewport) {
    argss_sprite_check(self);
    Check_Classes_N(viewport, ARGSS_Viewport);
    return rb_iv_set(self, "@viewport", viewport);
}
static VALUE argss_sprite_bitmap(VALUE self) {
    argss_sprite_check(self);
    return rb_iv_get(self, "@bitmap");
}
static VALUE argss_sprite_bitmapE(VALUE self, VALUE bitmap) {
    argss_sprite_check(self);
    Check_Classes_N(bitmap, ARGSS_Bitmap);
    if (bitmap != Qnil) {
        argss_bitmap_check(bitmap);
        ARGSS_mapSprites[self].SetImage(ARGSS_mapBitmaps[bitmap]);
    }
    return rb_iv_set(self, "@bitmap", bitmap);
}
static VALUE argss_sprite_src_rect(VALUE self) {
    argss_sprite_check(self);
    return rb_iv_get(self, "@src_rect");
}
static VALUE argss_sprite_src_rectE(VALUE self, VALUE src_rect) {
    argss_sprite_check(self);
    Check_Class(src_rect, ARGSS_Rect);
    ARGSS_mapSprites[self].SetSubRect(argss_rect_intrect(src_rect));
    return rb_iv_set(self, "@src_rect", src_rect);
}
static VALUE argss_sprite_visible(VALUE self) {
    argss_sprite_check(self);
    return rb_iv_get(self, "@visible");
}
static VALUE argss_sprite_visibleE(VALUE self, VALUE visible) {
    argss_sprite_check(self);
    Check_Bool(visible);
    return rb_iv_set(self, "@visible", visible);
}
static VALUE argss_sprite_x(VALUE self) {
    argss_sprite_check(self);
    return rb_funcall(rb_iv_get(self, "@x"), rb_intern("to_i"), 0, 0);
}
static VALUE argss_sprite_fx(VALUE self) {
    argss_sprite_check(self);
    return rb_iv_get(self, "@x");
}
static VALUE argss_sprite_xE(VALUE self, VALUE x) {
    argss_sprite_check(self);
    Check_Kind(x, rb_cNumeric);
    ARGSS_mapSprites[self].SetX(NUM2DBL(x));
    return rb_iv_set(self, "@x", x);
}
static VALUE argss_sprite_y(VALUE self) {
    argss_sprite_check(self);
    return rb_funcall(rb_iv_get(self, "@y"), rb_intern("to_i"), 0, 0);
}
static VALUE argss_sprite_fy(VALUE self) {
    argss_sprite_check(self);
    return rb_iv_get(self, "@y");
}
static VALUE argss_sprite_yE(VALUE self, VALUE y) {
    argss_sprite_check(self);
    Check_Kind(y, rb_cNumeric);
    ARGSS_mapSprites[self].SetY(NUM2DBL(y));
    return rb_iv_set(self, "@y", y);
}
static VALUE argss_sprite_z(VALUE self) {
    argss_sprite_check(self);
    return rb_iv_get(self, "@z");
}
static VALUE argss_sprite_zE(VALUE self, VALUE z) {
    argss_sprite_check(self);
    Check_Type(z, T_FIXNUM);
    return rb_iv_set(self, "@z", z);
}
static VALUE argss_sprite_ox(VALUE self) {
    argss_sprite_check(self);
    return rb_iv_get(self, "@ox");
}
static VALUE argss_sprite_oxE(VALUE self, VALUE ox) {
    argss_sprite_check(self);
    Check_Kind(ox, rb_cNumeric);
    ARGSS_mapSprites[self].SetCenter(NUM2DBL(ox), NUM2DBL(rb_iv_get(self, "@oy")));
    return rb_iv_set(self, "@ox", ox);
}
static VALUE argss_sprite_oy(VALUE self) {
    argss_sprite_check(self);
    return rb_iv_get(self, "@oy");
}
static VALUE argss_sprite_oyE(VALUE self, VALUE oy) {
    argss_sprite_check(self);
    Check_Kind(oy, rb_cNumeric);
    ARGSS_mapSprites[self].SetCenter(NUM2DBL(rb_iv_get(self, "@ox")), NUM2DBL(oy));
    return rb_iv_set(self, "@oy", oy);
}
static VALUE argss_sprite_zoom_x(VALUE self) {
    argss_sprite_check(self);
    return rb_iv_get(self, "@zoom_x");
}
static VALUE argss_sprite_zoom_xE(VALUE self, VALUE zoom_x) {
    argss_sprite_check(self);
    Check_Kind(zoom_x, rb_cNumeric);
    ARGSS_mapSprites[self].SetScaleX(NUM2DBL(zoom_x));
    return rb_iv_set(self, "@zoom_x", rb_Float(zoom_x));
}
static VALUE argss_sprite_zoom_y(VALUE self) {
    argss_sprite_check(self);
    return rb_iv_get(self, "@zoom_y");
}
static VALUE argss_sprite_zoom_yE(VALUE self, VALUE zoom_y) {
    argss_sprite_check(self);
    Check_Kind(zoom_y, rb_cNumeric);
    ARGSS_mapSprites[self].SetScaleY(NUM2DBL(zoom_y));
    return rb_iv_set(self, "@zoom_y", rb_Float(zoom_y));
}
static VALUE argss_sprite_angle(VALUE self) {
    argss_sprite_check(self);
    return rb_iv_get(self, "@angle");
}
static VALUE argss_sprite_angleE(VALUE self, VALUE angle) {
    argss_sprite_check(self);
    Check_Kind(angle, rb_cNumeric);
    ARGSS_mapSprites[self].SetRotation((NUM2DBL(angle)));
    return rb_iv_set(self, "@angle", rb_Float(angle));
}
static VALUE argss_sprite_mirror(VALUE self) {
    argss_sprite_check(self);
    return rb_iv_get(self, "@flipx");
}
static VALUE argss_sprite_mirrorE(VALUE self, VALUE mirror) {
    argss_sprite_check(self);
    Check_Bool(mirror);
    ARGSS_mapSprites[self].FlipX(NUM2BOOL(mirror));
    return rb_iv_set(self, "@flipx", mirror);
}
static VALUE argss_sprite_flipx(VALUE self) {
    argss_sprite_check(self);
    return rb_iv_get(self, "@flipx");
}
static VALUE argss_sprite_flipxE(VALUE self, VALUE flipx) {
    argss_sprite_check(self);
    Check_Bool(flipx);
    ARGSS_mapSprites[self].FlipX(NUM2BOOL(flipx));
    return rb_iv_set(self, "@flipx", flipx);
}
static VALUE argss_sprite_flipy(VALUE self) {
    argss_sprite_check(self);
    return rb_iv_get(self, "@flipy");
}
static VALUE argss_sprite_flipyE(VALUE self, VALUE flipy) {
    argss_sprite_check(self);
    Check_Bool(flipy);
    ARGSS_mapSprites[self].FlipY(NUM2BOOL(flipy));
    return rb_iv_set(self, "@flipy", flipy);
}
static VALUE argss_sprite_bush_depth(VALUE self) {
    argss_sprite_check(self);
    return rb_iv_get(self, "@bush_depth");
}
static VALUE argss_sprite_bush_depthE(VALUE self, VALUE bush_depth) {
    argss_sprite_check(self);
    Check_Kind(bush_depth, rb_cNumeric);
    //ToDo
    return rb_iv_set(self, "@bush_depth", bush_depth);
}
static VALUE argss_sprite_opacity(VALUE self) {
    argss_sprite_check(self);
    return rb_iv_get(self, "@opacity");
}
static VALUE argss_sprite_opacityE(VALUE self, VALUE opacity) {
    argss_sprite_check(self);
    Check_Kind(opacity, rb_cNumeric);
    //ToDo
    return rb_iv_set(self, "@opacity", opacity);
}
static VALUE argss_sprite_blend_type(VALUE self) {
    argss_sprite_check(self);
    return rb_iv_get(self, "@blend_type");
}
static VALUE argss_sprite_blend_typeE(VALUE self, VALUE blend_type) {
    argss_sprite_check(self);
    Check_Type(blend_type, T_FIXNUM);
    switch (NUM2INT(blend_type))
    {
        case 0:
            ARGSS_mapSprites[self].SetBlendMode(sf::Blend::Alpha);
            break;
        case 1:
            ARGSS_mapSprites[self].SetBlendMode(sf::Blend::Add);
            break;
        case 2:
            //ToDo
            //ARGSS_mapSprites[self].SetBlendMode(sf::Blend::Sub);
            break;
        case 3:
            ARGSS_mapSprites[self].SetBlendMode(sf::Blend::Multiply);
            break;
        default:
            ARGSS_mapSprites[self].SetBlendMode(sf::Blend::None);
            return rb_iv_set(self, "@blend_type", INT2NUM(-1));
    }
    return rb_iv_set(self, "@blend_type", blend_type);
}
static VALUE argss_sprite_color(VALUE self) {
    argss_sprite_check(self);
    return rb_iv_get(self, "@color");
}
static VALUE argss_sprite_colorE(VALUE self, VALUE color) {
    argss_sprite_check(self);
    Check_Class(color, ARGSS_Color);
    ARGSS_mapSprites[self].SetColor(argss_color_getsf(color));
    return rb_iv_set(self, "@color", color);
}
static VALUE argss_sprite_tone(VALUE self) {
    argss_sprite_check(self);
    return rb_iv_get(self, "@tone");
}
static VALUE argss_sprite_toneE(VALUE self, VALUE tone) {
    argss_sprite_check(self);
    Check_Class(tone, ARGSS_Tone);
    //ToDo
    return rb_iv_set(self, "@tone", tone);
}

void Init_Sprite() {
    typedef VALUE (*rubyfunc)(...);
    ARGSS_Sprite = rb_define_class("Sprite", rb_cObject);
    rb_define_method(ARGSS_Sprite, "initialize", (rubyfunc)argss_sprite_initialize, -1);
    rb_define_method(ARGSS_Sprite, "dispose", (rubyfunc)argss_sprite_dispose, 0);
    rb_define_method(ARGSS_Sprite, "disposed?", (rubyfunc)argss_sprite_disposedQ, 0);
    rb_define_method(ARGSS_Sprite, "flash", (rubyfunc)argss_sprite_flash, 2);
    rb_define_method(ARGSS_Sprite, "update", (rubyfunc)argss_sprite_update, 0);
    rb_define_method(ARGSS_Sprite, "viewport", (rubyfunc)argss_sprite_viewport, 0);
    rb_define_method(ARGSS_Sprite, "viewport=", (rubyfunc)argss_sprite_viewportE, 1);
    rb_define_method(ARGSS_Sprite, "bitmap", (rubyfunc)argss_sprite_bitmap, 0);
    rb_define_method(ARGSS_Sprite, "bitmap=", (rubyfunc)argss_sprite_bitmapE, 1);
    rb_define_method(ARGSS_Sprite, "src_rect", (rubyfunc)argss_sprite_src_rect, 0);
    rb_define_method(ARGSS_Sprite, "src_rect=", (rubyfunc)argss_sprite_src_rectE, 1);
    rb_define_method(ARGSS_Sprite, "visible", (rubyfunc)argss_sprite_visible, 0);
    rb_define_method(ARGSS_Sprite, "visible=", (rubyfunc)argss_sprite_visibleE, 1);
    rb_define_method(ARGSS_Sprite, "x", (rubyfunc)argss_sprite_x, 0);
    rb_define_method(ARGSS_Sprite, "fx", (rubyfunc)argss_sprite_fx, 0);
    rb_define_method(ARGSS_Sprite, "x=", (rubyfunc)argss_sprite_xE, 1);
    rb_define_method(ARGSS_Sprite, "y", (rubyfunc)argss_sprite_y, 0);
    rb_define_method(ARGSS_Sprite, "fy", (rubyfunc)argss_sprite_fy, 0);
    rb_define_method(ARGSS_Sprite, "y=", (rubyfunc)argss_sprite_yE, 1);
    rb_define_method(ARGSS_Sprite, "z", (rubyfunc)argss_sprite_z, 0);
    rb_define_method(ARGSS_Sprite, "z=", (rubyfunc)argss_sprite_zE, 1);
    rb_define_method(ARGSS_Sprite, "ox", (rubyfunc)argss_sprite_ox, 0);
    rb_define_method(ARGSS_Sprite, "ox=", (rubyfunc)argss_sprite_oxE, 1);
    rb_define_method(ARGSS_Sprite, "oy", (rubyfunc)argss_sprite_oy, 0);
    rb_define_method(ARGSS_Sprite, "oy=", (rubyfunc)argss_sprite_oyE, 1);
    rb_define_method(ARGSS_Sprite, "zoom_x", (rubyfunc)argss_sprite_zoom_x, 0);
    rb_define_method(ARGSS_Sprite, "zoom_x=", (rubyfunc)argss_sprite_zoom_xE, 1);
    rb_define_method(ARGSS_Sprite, "zoom_y", (rubyfunc)argss_sprite_zoom_y, 0);
    rb_define_method(ARGSS_Sprite, "zoom_y=", (rubyfunc)argss_sprite_zoom_yE, 1);
    rb_define_method(ARGSS_Sprite, "angle", (rubyfunc)argss_sprite_angle, 0);
    rb_define_method(ARGSS_Sprite, "angle=", (rubyfunc)argss_sprite_angleE, 1);
    rb_define_method(ARGSS_Sprite, "mirror", (rubyfunc)argss_sprite_mirror, 0);
    rb_define_method(ARGSS_Sprite, "mirror=", (rubyfunc)argss_sprite_mirrorE, 1);
    rb_define_method(ARGSS_Sprite, "flipx", (rubyfunc)argss_sprite_flipx, 0);
    rb_define_method(ARGSS_Sprite, "flipx=", (rubyfunc)argss_sprite_flipxE, 1);
    rb_define_method(ARGSS_Sprite, "flipy", (rubyfunc)argss_sprite_flipy, 0);
    rb_define_method(ARGSS_Sprite, "flipy=", (rubyfunc)argss_sprite_flipyE, 1);
    rb_define_method(ARGSS_Sprite, "bush_depth", (rubyfunc)argss_sprite_bush_depth, 0);
    rb_define_method(ARGSS_Sprite, "bush_depth=", (rubyfunc)argss_sprite_bush_depthE, 1);
    rb_define_method(ARGSS_Sprite, "opacity", (rubyfunc)argss_sprite_opacity, 0);
    rb_define_method(ARGSS_Sprite, "opacity=", (rubyfunc)argss_sprite_opacityE, 1);
    rb_define_method(ARGSS_Sprite, "blend_type", (rubyfunc)argss_sprite_blend_type, 0);
    rb_define_method(ARGSS_Sprite, "blend_type=", (rubyfunc)argss_sprite_blend_typeE, 1);
    rb_define_method(ARGSS_Sprite, "color", (rubyfunc)argss_sprite_color, 0);
    rb_define_method(ARGSS_Sprite, "color=", (rubyfunc)argss_sprite_colorE, 1);
    rb_define_method(ARGSS_Sprite, "tone", (rubyfunc)argss_sprite_tone, 0);
    rb_define_method(ARGSS_Sprite, "tone=", (rubyfunc)argss_sprite_toneE, 1);
}
