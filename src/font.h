#ifndef _FONT_H_
#define _FONT_H_

#include <string>
#include <vector>
#include "bitmap_fwd.h"
#include "color.h"

typedef std::vector<std::string> FontList;

class Font {
 public:
  static FontList default_name;
  static size_t default_size;
  static bool default_bold, default_italic, default_shadow;
  static ColorRef default_color;

  FontList name;
  size_t size;
  bool bold, italic, shadow;
  ColorRef color;

  Font(FontList const& n = default_name, std::size_t s = default_size);

  std::string Find() const;
};

inline bool operator ==(Font const& lhs, Font const& rhs) {
  return lhs.name == rhs.name and lhs.size == rhs.size
      and lhs.bold == rhs.bold and lhs.italic == rhs.italic
      and *lhs.color == *rhs.color;
}
inline bool operator !=(Font const& lhs, Font const& rhs) {
  return not(lhs == rhs);
}

#endif
