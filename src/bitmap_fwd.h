#ifndef _BITMAP_FWD_H_
#define _BITMAP_FWD_H_

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/make_shared.hpp>

#include <boost/preprocessor/cat.hpp>

#define PP_declare_ref(name)                              \
  class name;                                             \
  typedef boost::shared_ptr<name> BOOST_PP_CAT(name, Ref) \

PP_declare_ref(Bitmap);
PP_declare_ref(Rect);
PP_declare_ref(Viewport);
PP_declare_ref(Tone);
PP_declare_ref(Color);
PP_declare_ref(Font);
PP_declare_ref(Table);
PP_declare_ref(Plane);

#undef PP_declare_ref

#endif
