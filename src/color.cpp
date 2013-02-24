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
#include "color.h"

///////////////////////////////////////////////////////////
/// Constructor
///////////////////////////////////////////////////////////
Color::Color() {
  red = 0.0f;
  green = 0.0f;
  blue = 0.0f;
  alpha = 0.0f;
}
Color::Color(int ired, int igreen, int iblue, int ialpha) {
  red = (float)ired;
  green = (float)igreen;
  blue = (float)iblue;
  alpha = (float)ialpha;
}

///////////////////////////////////////////////////////////
/// Get uint32_t
///////////////////////////////////////////////////////////
uint32_t Color::GetUint32() const {
  uint32_t ret;
  uint8_t* const ptr = reinterpret_cast<uint8_t*>(&ret);
  ptr[0] = red;
  ptr[1] = green;
  ptr[2] = blue;
  ptr[3] = alpha;
  return ret;
}

///////////////////////////////////////////////////////////
/// Static create Color from uint32_t
///////////////////////////////////////////////////////////
Color Color::NewUint32(uint32_t color) {
  uint8_t const* const ptr = reinterpret_cast<uint8_t*>(&color);
  return Color(ptr[0], ptr[1], ptr[2], ptr[3]);
}
