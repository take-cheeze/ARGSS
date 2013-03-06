#include "drawable.h"
#include "graphics.h"
#include "viewport.h"

#include <cassert>


Drawable::Drawable(ViewportRef const& v)
    : viewport_(v), z_(0)
{
  if (viewport_) {
    viewport_->RegisterZObj(z_, *this);
  } else {
    Graphics::RegisterZObj(z_, *this);
  }
}

Drawable::~Drawable() {
  if (viewport_) {
    viewport_->RemoveZObj(*this);
  } else {
    Graphics::RemoveZObj(*this);
  }
}

void Drawable::viewport(ViewportRef const& v) {
  if(v) { assert(this != static_cast<Drawable*>(v.get())); }

  if (viewport_ != v) {
    if (v) {
      Graphics::RemoveZObj(*this);
      v->RegisterZObj(z_, *this);
    } else {
      if (viewport_) viewport_->RemoveZObj(*this);
      Graphics::RegisterZObj(z_, *this);
    }
  }

  viewport_ = v;
}

int Drawable::z() const {
  return z_;
}
void Drawable::z(int nz) {
  if (z_ != nz) {
    if (viewport_) {
      viewport_->UpdateZObj(*this, nz);
    } else {
      Graphics::UpdateZObj(*this, nz);
    }
  }
  z_ = nz;
}
