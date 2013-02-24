#ifndef _ARUBY_CXX_H_
#define _ARUBY_CXX_H_

#include <boost/mpl/if.hpp>
#include <boost/mpl/int.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/function_traits.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/unordered_map.hpp>

#include "output.h"

namespace ARGSS {

template<class T>
void mark_func(void* /* self */) {}

template<class T>
void delete_func(void* const ptr) {
  typedef boost::shared_ptr<T> ref_type;
  reinterpret_cast<ref_type*>(ptr)->~ref_type();
}

inline void no_delete_func(void*) {}

template<class T>
struct RubyClass {
  struct reference {
    VALUE ruby_ref;
    boost::weak_ptr<T> cxx_ref;
  };
  typedef boost::unordered_map<T*, reference> table_type;
  static table_type table_;
  static VALUE value;

  static void clear();
};
template<class T> VALUE RubyClass<T>::value = Qnil;
template<class T> typename RubyClass<T>::table_type RubyClass<T>::table_;
template<class T>
void RubyClass<T>::clear() {
  for(typename table_type::iterator i = table_.begin(); i != table_.end(); ++i) {
    if(i->second.cxx_ref.expired()) { i = table_.erase(i); }
  }
}

template<class T>
bool check_class(VALUE const klass) {
  return rb_class_inherited_p(klass, RubyClass<T>::value) == Qtrue;
}
template<class T>
VALUE ruby_class() {
  return RubyClass<T>::value;
}

template<class T>
boost::shared_ptr<T>& get_ptr(VALUE const v) {
  static boost::shared_ptr<T> nil;
  if(v == Qnil) { nil.reset(); return nil; }

  assert(check_class<T>(rb_class_of(v)));
  boost::shared_ptr<T>* ptr = NULL;
  Data_Get_Struct(v, boost::shared_ptr<T>, ptr);
  assert(ptr);
  return (*ptr);
}

template<class T>
T& get(VALUE const v) { return *get_ptr<T>(v); }

template<class T>
VALUE alloc_func(VALUE klass) {
  assert(check_class<T>(klass));
  boost::shared_ptr<T>* ptr;
  VALUE const data =
      Data_Make_Struct(klass, boost::shared_ptr<T>,
                       &mark_func<T>, &delete_func<T>, ptr);
  new(ptr) boost::shared_ptr<T>();
  return data;
}

template<class T>
void set_ptr(VALUE obj, boost::shared_ptr<T> const& s_ptr) {
  typedef RubyClass<T> k;
  assert(k::table_.find(s_ptr.get()) == k::table_.end());

  typename k::reference const ref = { obj, s_ptr };
  k::table_[s_ptr.get()] = ref;
  get_ptr<T>(obj) = s_ptr;
}

template<class T>
VALUE create(boost::shared_ptr<T> const& s_ptr) {
  assert(s_ptr);

  typedef RubyClass<T> k;

  typename k::table_type::iterator i = k::table_.find(s_ptr.get());
  if(i != k::table_.end() and not i->second.cxx_ref.expired()) {
    return i->second.ruby_ref;
  } else {
    boost::shared_ptr<T>* ptr;
    VALUE const data =
        Data_Make_Struct(ruby_class<T>(), boost::shared_ptr<T>,
                         &mark_func<T>, &delete_func<T>, ptr);
    new(ptr) boost::shared_ptr<T>(s_ptr);
    typename k::reference const ref = { data, *ptr };
    k::table_[s_ptr.get()] = ref;
    return data;
  }
}

namespace mpl = boost::mpl;

struct rb_method {
  char const* name;
  rubyfunc function;
  int arg_num;
  bool is_singletone;

  template<class F>
  struct arg_num_type : public mpl::if_
  <boost::is_same<F, VALUE(int, VALUE*, VALUE)>,
   mpl::int_<-1>, mpl::int_<boost::function_traits<F>::arity - 1> >::type {};

  template<class F>
  rb_method(char const* n, F f, bool s = false)
      : name(n), function((rubyfunc)f)
      , arg_num(arg_num_type<typename boost::remove_pointer<F>::type>::value)
      , is_singletone(s) {}

  rb_method() : function(NULL) {}
};

template<class T, bool Deep>
struct Compare;

template<class T>
struct Compare<T, true> {
  VALUE eq_(VALUE self, VALUE rhs) {
    return get<T>(self) == get<T>(rhs)? Qtrue : Qfalse;
  }
  VALUE not_eq_(VALUE self, VALUE rhs) {
    return get<T>(self) != get<T>(rhs)? Qtrue : Qfalse;
  }
};

template<class T>
struct Compare<T, false> {
  VALUE eq_(VALUE self, VALUE rhs) {
    return get_ptr<T>(self) == get_ptr<T>(rhs)? Qtrue : Qfalse;
  }
  VALUE not_eq_(VALUE self, VALUE rhs) {
    return get_ptr<T>(self) != get_ptr<T>(rhs)? Qtrue : Qfalse;
  }
};

template<class T, bool Deep = false>
VALUE define_class(char const* name, rb_method const* meth, VALUE const super = rb_cObject) {
  VALUE const c = rb_define_class(name, super);
  RubyClass<T>::value = c;

  for(rb_method const* i = meth; i->function != NULL; ++i) {
    (i->is_singletone? rb_define_singleton_method : rb_define_method)
        (c, i->name, i->function, i->arg_num);
  }
  rb_define_method(c, "!=", (rubyfunc)Compare<T, Deep>::not_eq_, 1);
  rb_define_method(c, "==", (rubyfunc)Compare<T, Deep>::eq_, 1);
  rb_define_alloc_func(c, &alloc_func<T>);
  return c;
}
template<class T, bool Deep = false>
VALUE define_class_under(VALUE const parent, char const* name, rb_method const* meth, VALUE const super = rb_cObject) {
  VALUE const c = rb_define_class_under(parent, name, super);
  RubyClass<T>::value = c;

  for(rb_method const* i = meth; i->function != NULL; ++i) {
    (i->is_singletone? rb_define_singleton_method : rb_define_method)
        (c, i->name, i->function, i->arg_num);
  }
  rb_define_method(c, "!=", (rubyfunc)Compare<T, Deep>::not_eq_, 1);
  rb_define_method(c, "==", (rubyfunc)Compare<T, Deep>::eq_, 1);
  rb_define_alloc_func(c, &alloc_func<T>);
  return c;
}

VALUE define_module(char const* name, rb_method const* meth);
VALUE define_module_under(VALUE const parent, char const* name, rb_method const* meth);

template<class T>
bool check_disposed(VALUE id) {
  if(not get_ptr<T>(id)) {
    Output::raise(boost::format("disposed %s <%i>") % rb_class2name(ruby_class<T>()) % id);
    return true;
  }
  return false;
}

}

#endif
