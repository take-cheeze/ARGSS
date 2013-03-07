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
#include "binding/abitmap.h"
#include "binding/acolor.h"
#include "binding/aerror.h"
#include "binding/afont.h"
#include "bitmap.h"
#include "color.h"
#include "rect.h"
#include "filefinder.h"
#include "output.h"
#include "SOIL.h"

VALUE ARGSS::ABitmap::rinitialize(int argc, VALUE* argv, VALUE self) {
  if (argc == 0) raise_argn(argc, 1);
  else if (argc == 1) {
    std::string const file = StringValuePtr(argv[0]);
    set_ptr(self, Bitmap::Create(file));
    if(not get_ptr<Bitmap>(self)) {
      if(FileFinder::FindImage(file).empty()) { Output::FileNotFound(file); }
      else {
        Output::raise(boost::format("couldn't load %s image.\n%s\n") % file % SOIL_last_result());
      }
    }
  } else if (argc == 2) {
    int const w = NUM2INT(argv[0]), h = NUM2INT(argv[1]);
    if (w <= 0 && h <= 0) {
      Output::raise(boost::format("cant't create %dx%d image.\nWidth and height must be bigger than 0.\n") % w % h);
    }
    set_ptr(self, boost::make_shared<Bitmap>(w, h));
  }
  else raise_argn(argc, 2);
  return self;
}
VALUE ARGSS::ABitmap::rdispose(VALUE self) {
  get_ptr<Bitmap>(self).reset();
  return self;
}
VALUE ARGSS::ABitmap::rdisposedQ(VALUE self) {
  return get_ptr<Bitmap>(self)? Qfalse : Qtrue;
}

VALUE ARGSS::ABitmap::rwidth(VALUE self) {
  check_disposed<Bitmap>(self);
  return INT2NUM(get<Bitmap>(self).GetWidth());
}
VALUE ARGSS::ABitmap::rheight(VALUE self) {
  check_disposed<Bitmap>(self);
  return INT2NUM(get<Bitmap>(self).GetHeight());
}
VALUE ARGSS::ABitmap::rrect(VALUE self) {
  check_disposed<Bitmap>(self);
  return create(boost::make_shared<Rect>(get<Bitmap>(self).GetRect()));
}
VALUE ARGSS::ABitmap::rblt(int argc, VALUE* argv, VALUE self) {
  check_disposed<Bitmap>(self);
  check_disposed<Bitmap>(argv[2]);
  switch(argc) {
    default:
      raise_argn(argc, 4);
      break;
    case 5:
      get<Bitmap>(self).Blit(NUM2INT(argv[0]), NUM2INT(argv[1]),
                                      get<Bitmap>(argv[2]),
                                      get<Rect>(argv[3]), NUM2INT(argv[4]));
      break;
    case 4:
      get<Bitmap>(self).Blit(NUM2INT(argv[0]), NUM2INT(argv[1]),
                                      get<Bitmap>(argv[2]),
                                      get<Rect>(argv[3]), 255);
      break;
  }
  return self;
}
VALUE ARGSS::ABitmap::rstretch_blt(int argc, VALUE* argv, VALUE self) {
  check_disposed<Bitmap>(self);
  if (argc < 3) raise_argn(argc, 3);
  else if (argc > 4) raise_argn(argc, 4);
  check_disposed<Bitmap>(argv[1]);
  if (argc == 4) {
    get<Bitmap>(self).StretchBlit(get<Rect>(argv[0]), get<Bitmap>(argv[1]), get<Rect>(argv[2]), NUM2INT(argv[3]));
  } else {
    get<Bitmap>(self).StretchBlit(get<Rect>(argv[0]), get<Bitmap>(argv[1]), get<Rect>(argv[2]), 255);
  }
  return self;
}
VALUE ARGSS::ABitmap::rfill_rect(int argc, VALUE* argv, VALUE self) {
  check_disposed<Bitmap>(self);
  if (argc < 2) raise_argn(argc, 2);
  else if (argc == 2) {
    get<Bitmap>(self).FillRect(get<Rect>(argv[0]), get<Color>(argv[1]));
  } else if (argc == 5) {
    get<Bitmap>(self).FillRect(Rect(NUM2INT(argv[0]), NUM2INT(argv[1]), NUM2INT(argv[2]), NUM2INT(argv[3])), get<Color>(argv[4]));
  }
  else raise_argn(argc, 5);
  return self;
}
VALUE ARGSS::ABitmap::rclear(int argc, VALUE* argv, VALUE self) {
  check_disposed<Bitmap>(self);
  if (argc > 1) raise_argn(argc, 1);
  else if (argc == 1) {
    get<Bitmap>(self).Clear(get<Color>(argv[0]));
  } else {
    get<Bitmap>(self).Clear();
  }
  return self;
}
VALUE ARGSS::ABitmap::rget_pixel(VALUE self, VALUE x, VALUE y) {
  check_disposed<Bitmap>(self);
  return create(boost::make_shared<Color>(get<Bitmap>(self).GetPixel(NUM2INT(x), NUM2INT(y))));
}
VALUE ARGSS::ABitmap::rset_pixel(VALUE self, VALUE x, VALUE y, VALUE color) {
  check_disposed<Bitmap>(self);
  get<Bitmap>(self).SetPixel(NUM2INT(x), NUM2INT(y), get<Color>(color));
  return self;
}
VALUE ARGSS::ABitmap::rhue_change(VALUE self, VALUE hue) {
  check_disposed<Bitmap>(self);
  get<Bitmap>(self).HueChange(NUM2DBL(hue));
  return self;
}
VALUE ARGSS::ABitmap::rsaturation_change(VALUE self, VALUE saturation) {
  check_disposed<Bitmap>(self);
  get<Bitmap>(self).SatChange(NUM2DBL(saturation));
  return self;
}
VALUE ARGSS::ABitmap::rluminance_change(VALUE self, VALUE luminance) {
  check_disposed<Bitmap>(self);
  get<Bitmap>(self).LumChange(NUM2DBL(luminance));
  return self;
}
VALUE ARGSS::ABitmap::rhsl_change(int argc, VALUE* argv, VALUE self) {
  check_disposed<Bitmap>(self);
  if (argc < 3) raise_argn(argc, 3);
  else if (argc > 4) raise_argn(argc, 4);
  if (argc == 4) {
    get<Bitmap>(self).HSLChange(NUM2DBL(argv[0]), NUM2DBL(argv[1]), NUM2DBL(argv[2]), get<Rect>(argv[3]));
  } else {
    get<Bitmap>(self).HSLChange(NUM2DBL(argv[0]), NUM2DBL(argv[1]), NUM2DBL(argv[2]));
  }
  return self;
}
VALUE ARGSS::ABitmap::rdraw_text(int argc, VALUE* argv, VALUE self) {
  check_disposed<Bitmap>(self);
  VALUE const font = create(get<Bitmap>(self).font);
  rb_iv_set(font, "@name", rb_iv_get(font, "@name"));
  int align = 0;
  if (argc < 2) raise_argn(argc, 2);
  else if (argc < 4) {
    if (argc == 3) {
      align = NUM2INT(argv[2]);
    }
    get<Bitmap>(self).TextDraw(get<Rect>(argv[0]), StringValuePtr(argv[1]), align);
  } else if (argc == 4) raise_argn(argc, 3);
  else if (argc < 7) {
    if (argc == 6) {
      align = NUM2INT(argv[5]);
    }
    get<Bitmap>(self).TextDraw(Rect(NUM2INT(argv[0]), NUM2INT(argv[1]), NUM2INT(argv[2]), NUM2INT(argv[3])), StringValuePtr(argv[4]), align);
  }
  else raise_argn(argc, 6);
  return self;
}
VALUE ARGSS::ABitmap::rtext_size(VALUE self, VALUE str) {
  check_disposed<Bitmap>(self);
  VALUE const font = create(get<Bitmap>(self).font);
  rb_iv_set(font, "@name", rb_iv_get(font, "@name"));
  return create(boost::make_shared<Rect>(get<Bitmap>(self).GetTextSize(StringValuePtr(str))));
}
VALUE ARGSS::ABitmap::rgradient_fill_rect(int argc, VALUE* argv, VALUE self) {
  check_disposed<Bitmap>(self);
  if (argc < 3) raise_argn(argc, 3);
  else if (argc < 5) {
    bool vertical = false;
    if (argc == 4) {
      vertical = NUM2BOOL(argv[3]);
    }
    get<Bitmap>(self).GradientFillRect(get<Rect>(argv[0]), get<Color>(argv[1]), get<Color>(argv[2]), vertical);
  } else if (argc < 6) raise_argn(argc, 6);
  else if (argc < 8) {
    bool vertical = false;
    if (argc == 4) {
      vertical = NUM2BOOL(argv[6]);
    }
    get<Bitmap>(self).GradientFillRect(Rect(NUM2INT(argv[0]), NUM2INT(argv[1]), NUM2INT(argv[2]), NUM2INT(argv[3])), get<Color>(argv[4]), get<Color>(argv[5]), vertical);
  }
  else raise_argn(argc, 7);
  return self;
}
VALUE ARGSS::ABitmap::rclear_rect(int argc, VALUE* argv, VALUE self) {
  check_disposed<Bitmap>(self);
  if (argc < 1) raise_argn(argc, 1);
  if (argc == 1) {
    get<Bitmap>(self).ClearRect(get<Rect>(argv[0]));
  } else if (argc == 4) {
    get<Bitmap>(self).ClearRect(Rect(NUM2INT(argv[0]), NUM2INT(argv[1]), NUM2INT(argv[2]), NUM2INT(argv[3])));
  }
  else raise_argn(argc, 4);
  return self;
}
VALUE ARGSS::ABitmap::rblur(VALUE self) {
  check_disposed<Bitmap>(self);
  get<Bitmap>(self).Blur();
  return self;
}
VALUE ARGSS::ABitmap::rradial_blur(VALUE self, VALUE angle, VALUE division) {
  check_disposed<Bitmap>(self);
  get<Bitmap>(self).RadialBlur(NUM2INT(angle), NUM2INT(division));
  return self;
}
VALUE ARGSS::ABitmap::rfont(VALUE self) {
  check_disposed<Bitmap>(self);
  return create(get<Bitmap>(self).font);
}
VALUE ARGSS::ABitmap::rfontE(VALUE self, VALUE font) {
  check_disposed<Bitmap>(self);
  assert(check_class<Font>(font));
  get<Bitmap>(self).font = get_ptr<Font>(font);
  return create(get<Bitmap>(self).font);;
}
VALUE ARGSS::ABitmap::rdup(VALUE self) {
  check_disposed<Bitmap>(self);
  Bitmap const& bmp = get<Bitmap>(self);
  return create(boost::make_shared<Bitmap>(bmp, bmp.GetRect()));
}

///////////////////////////////////////////////////////////
// ARGSS Bitmap initialize
///////////////////////////////////////////////////////////
void ARGSS::ABitmap::Init() {
  rb_method const methods[] = {
    rb_method("initialize", rinitialize),
    rb_method("dispose", rdispose),
    rb_method("disposed?", rdisposedQ),
    rb_method("width", rwidth),
    rb_method("height", rheight),
    rb_method("rect", rrect),
    rb_method("blt", rblt),
    rb_method("stretch_blt", rstretch_blt),
    rb_method("fill_rect", rfill_rect),
    rb_method("clear", rclear),
    rb_method("get_pixel", rget_pixel),
    rb_method("set_pixel", rset_pixel),
    rb_method("hue_change", rhue_change),
    rb_method("saturation_change", rsaturation_change),
    rb_method("luminance_change", rluminance_change),
    rb_method("hsl_change", rhsl_change),
    rb_method("draw_text", rdraw_text),
    rb_method("text_size", rtext_size),
    rb_method("gradient_fill_rect", rgradient_fill_rect),
    rb_method("clear_rect", rclear_rect),
    rb_method("blur", rblur),
    rb_method("radial_blur", rradial_blur),
    rb_method("font", rfont),
    rb_method("font=", rfontE),
    rb_method("clone", rdup),
    rb_method("dup", rdup),
    rb_method()
  };
  define_class<Bitmap, false>("Bitmap", methods);
}
