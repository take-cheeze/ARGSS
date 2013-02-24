#ifndef _TABLE_H_
#define _TABLE_H_

#include <boost/multi_array.hpp>
#include <boost/cstdint.hpp>

typedef boost::multi_array<int16_t, 3> TableBase;

class Table : public TableBase {
 public:
  Table(size_t x, size_t y = 1, size_t z = 1);
  void resize(size_t x, size_t y = 1, size_t z = 1);

  int16_t& operator()(size_t x, size_t y = 0, size_t z = 0);

  size_t xsize() const;
  size_t ysize() const;
  size_t zsize() const;

  size_t dim() const { return dim_; }
 private:
  int dim_;
};

#endif
