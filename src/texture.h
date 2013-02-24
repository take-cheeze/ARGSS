#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "opengl.h"
#include "bitmap_fwd.h"

struct Texture {
  Texture(BitmapRef const& bmp);
  ~Texture();

  void Bind() const;
  bool Update();

  bool expired() const;
  BitmapRef bitmap() const;

 private:
  boost::weak_ptr<Bitmap> const bmp_;
  GLuint name_;
};

#endif
