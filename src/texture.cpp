#include "texture.h"
#include "bitmap.h"


Texture::Texture(BitmapRef const& bmp)
    : bmp_(bmp), name_(0)
{
  Update();
}

Texture::~Texture() {
  if(name_ != 0) {
    glDeleteTextures(1, &name_);
  }
}

bool Texture::expired() const {
  return bmp_.expired();
}

BitmapRef Texture::bitmap() const {
  assert(! expired());
  return bmp_.lock();
}

void Texture::Bind() const {
  if(name_ != 0) {
    glBindTexture(GL_TEXTURE_2D, name_);
  }
}

bool Texture::Update() {
  assert(! expired());

  BitmapRef const bmp = bmp_.lock();
  bool const ret = bmp->dirty();

  if(not ret) { return false; }

  glEnable(GL_TEXTURE_2D);
  if(name_ == 0) {
    glEnable(GL_TEXTURE_2D);

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();

    glGenTextures(1, &name_);
    Bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, 4, bmp->GetWidth(), bmp->GetHeight(),
                 0, GL_RGBA, GL_UNSIGNED_BYTE, bmp->GetPixels());
  } else {
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();

    Bind();
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, bmp->GetWidth(), bmp->GetHeight(),
                    GL_RGBA, GL_UNSIGNED_BYTE, bmp->GetPixels());
  }

  return true;
}
