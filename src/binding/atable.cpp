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
#include "binding/atable.h"
#include "table.h"

///////////////////////////////////////////////////////////
// ARGSS Table instance methods
///////////////////////////////////////////////////////////
VALUE ARGSS::ATable::rinitialize(int argc, VALUE* argv, VALUE self) {
  if (argc < 1) raise_argn(argc, 1);
  else if (argc > 3) raise_argn(argc, 3);
  int xsize = 1;
  int ysize = 1;
  int zsize = 1;
  switch (argc) {
  case 3:
    zsize = NUM2INT(argv[2]);
  case 2:
    ysize = NUM2INT(argv[1]);
  case 1:
    xsize = NUM2INT(argv[0]);
  }
  set_ptr(self, boost::make_shared<Table>(xsize, ysize, zsize));
  return self;
}
VALUE ARGSS::ATable::rresize(int argc, VALUE* argv, VALUE self) {
  int xsize = 1;
  int ysize = 1;
  int zsize = 1;
  switch (argc) {
  case 3:
    zsize = NUM2INT(argv[2]);
  case 2:
    ysize = NUM2INT(argv[1]);
  case 1:
    xsize = NUM2INT(argv[0]);
  }
  get<Table>(self).resize(xsize, ysize, zsize);
  return self;
}
VALUE ARGSS::ATable::rxsize(VALUE self) {
  return INT2NUM(get<Table>(self).xsize());
}
VALUE ARGSS::ATable::rysize(VALUE self) {
  return INT2NUM(get<Table>(self).ysize());
}
VALUE ARGSS::ATable::rzsize(VALUE self) {
  return INT2NUM(get<Table>(self).zsize());
}
VALUE ARGSS::ATable::raref(int argc, VALUE* argv, VALUE self) {
  int dim = get<Table>(self).dim();
  if (argc != dim) raise_argn(argc, dim);
  int x = 0;
  int y = 0;
  int z = 0;
  switch (argc) {
  case 3:
    z = NUM2INT(argv[2]);
  case 2:
    y = NUM2INT(argv[1]);
  case 1:
    x = NUM2INT(argv[0]);
  }
  int xsize = get<Table>(self).xsize();
  int ysize = get<Table>(self).ysize();
  int zsize = get<Table>(self).zsize();
  if (x >= xsize || y >= ysize || z >= zsize) {
    return Qnil;
  } else {
    return INT2NUM(get<Table>(self)(x, y, z));
  }
}
VALUE ARGSS::ATable::raset(int argc, VALUE* argv, VALUE self) {
  int dim = get<Table>(self).dim();
  if (argc != (dim + 1)) raise_argn(argc, dim + 1);
  int x = 0;
  int y = 0;
  int z = 0;
  switch (argc) {
  case 3:
    z = NUM2INT(argv[2]);
  case 2:
    y = NUM2INT(argv[1]);
  case 1:
    x = NUM2INT(argv[0]);
  }
  int val = std::min<int>(std::numeric_limits<int16_t>::max(),
                     std::max<int>(std::numeric_limits<int16_t>::min(),
                              NUM2INT(argv[argc - 1])));
  int xsize = get<Table>(self).xsize();
  int ysize = get<Table>(self).ysize();
  int zsize = get<Table>(self).zsize();
  if (x < xsize && y < ysize && z < zsize) {
    get<Table>(self)(x, y, z) = val;
  }
  return argv[argc - 1];
}

static VALUE to_ruby(uint16_t v) {
  return INT2NUM(v);
}

VALUE ARGSS::ATable::rdump(int argc, VALUE* /* argv */, VALUE self) {
  if (argc > 1) raise_argn(argc, 1);
  Table const& t = get<Table>(self);
  VALUE arr = rb_ary_new3(5, INT2NUM(t.dim()), INT2NUM(t.xsize()), INT2NUM(t.ysize()),
                          INT2NUM(t.zsize()), INT2NUM(t.num_elements()));
  VALUE const str = rb_funcall(arr, rb_intern("pack"), 1, rb_str_new2("V5"));

  std::vector<VALUE> data(t.num_elements());
  std::transform(t.data(), t.data() + t.num_elements(), data.begin(), to_ruby);
  rb_str_concat(str, rb_funcall(rb_ary_new4(data.size(), &data.front()),
                                rb_intern("pack"), 1, rb_str_new2("v*")));
  return str;
}

///////////////////////////////////////////////////////////
// ARGSS Table class methods
///////////////////////////////////////////////////////////
VALUE ARGSS::ATable::rload(VALUE /* self */, VALUE str) {
  VALUE arr = rb_funcall(str, rb_intern("unpack"), 1, rb_str_new2("V5"));
  int const dim = NUM2INT(rb_ary_entry(arr, 0));
  boost::shared_ptr<Table> const tbl =
      dim == 3? boost::make_shared<Table>(NUM2INT(rb_ary_entry(arr, 1)),
                                          NUM2INT(rb_ary_entry(arr, 2)),
                                          NUM2INT(rb_ary_entry(arr, 3))):
      dim == 2? boost::make_shared<Table>(NUM2INT(rb_ary_entry(arr, 1)),
                                          NUM2INT(rb_ary_entry(arr, 2))):
      dim == 1? boost::make_shared<Table>(NUM2INT(rb_ary_entry(arr, 1))):
      boost::shared_ptr<Table>();
  VALUE data = rb_funcall(rb_str_substr(str, 20, tbl->num_elements() * 2),
                          rb_intern("unpack"), 1, rb_str_new2("v*"));
  for(size_t i = 0; i < size_t(RARRAY_LEN(data)); ++i) {
    tbl->data()[i] = NUM2INT(rb_ary_entry(data, i));
  }
  return create(tbl);
}

///////////////////////////////////////////////////////////
// ARGSS Table initialize
///////////////////////////////////////////////////////////
void ARGSS::ATable::Init() {
  rb_method const methods[] = {
    rb_method("initialize", rinitialize),
    rb_method("resize", rresize),
    rb_method("xsize", rxsize),
    rb_method("ysize", rysize),
    rb_method("zsize", rzsize),
    rb_method("[]", raref),
    rb_method("[]=", raset),
    rb_method("_dump", rdump),
    rb_method("_load", rload, true),
    rb_method() };
  define_class<Table, false>("Table", methods);
}
