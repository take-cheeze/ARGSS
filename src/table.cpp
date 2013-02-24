#include "table.h"


Table::Table(size_t x, size_t y, size_t z)
    : TableBase(boost::extents[z][y][x])
{
  dim_ = z != 1? 3 : y != 1? 2 : x != 1? 1 : 0;
}

void Table::resize(size_t x, size_t y, size_t z) {
  dim_ = z != 1? 3 : y != 1? 2 : x != 1? 1 : 0;
  TableBase::resize(boost::extents[z][y][x]);
}

int16_t& Table::operator()(size_t x, size_t y, size_t z) {
  return (*this)[z][y][x];
}

size_t Table::xsize() const {
  return shape()[2];
}

size_t Table::ysize() const {
  return shape()[1];
}

size_t Table::zsize() const {
  return shape()[0];
}
