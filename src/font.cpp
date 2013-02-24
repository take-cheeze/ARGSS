#include "font.h"
#include "color.h"
#include "filefinder.h"

FontList Font::default_name(1, "MS PMincho"); // "Arial"
size_t Font::default_size = 22;
bool Font::default_bold = false;
bool Font::default_italic = false;
bool Font::default_shadow = false;
ColorRef Font::default_color = boost::make_shared<Color>(255, 255, 255, 255);

Font::Font(FontList const& n, std::size_t const s)
    : name(n), size(s), bold(default_bold), italic(default_italic)
    , shadow(default_shadow)
    , color(boost::make_shared<Color>(*default_color)) {}

std::string Font::Find() const {
  for(size_t i = 0; i < name.size(); ++i) {
    std::string const ret = FileFinder::FindFont(name[i]);
    if(not ret.empty()) { return ret; }
  }

  return std::string();
}
