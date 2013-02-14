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
#include <string>

extern "C" {
  #include "ruby.h"
}

#include "binding/argss.h"
#include "binding/abitmap.h"
#include "binding/acolor.h"
#include "binding/afont.h"
#include "binding/aerror.h"
#include "binding/aplane.h"
#include "binding/arect.h"
#include "binding/asprite.h"
#include "binding/atable.h"
#include "binding/atilemap.h"
#include "binding/atone.h"
#include "binding/aviewport.h"
#include "binding/awindow.h"
#include "binding/aaudio.h"
#include "binding/agraphics.h"
#include "binding/ainput.h"
#include "binding/akeys.h"
#include "binding/aoutput.h"
#include "binding/rpg.h"
#include "binding/rpg_cache.h"
#include "binding/rpg_weather.h"
#include "binding/rpg_sprite.h"

///////////////////////////////////////////////////////////
// Initialize
///////////////////////////////////////////////////////////
void ARGSS::Init() {
  ARGSS::ABitmap::Init();
  ARGSS::AColor::Init();
  ARGSS::AError::Init();
  ARGSS::AFont::Init();
  ARGSS::APlane::Init();
  ARGSS::ARect::Init();
  ARGSS::ASprite::Init();
  ARGSS::ATable::Init();
  ARGSS::ATilemap::Init();
  ARGSS::ATone::Init();
  ARGSS::AViewport::Init();
  ARGSS::AWindow::Init();

  ARGSS::AAudio::Init();
  ARGSS::AGraphics::Init();
  ARGSS::AInput::Init();
  ARGSS::AKeys::Init();
  ARGSS::AOutput::Init();

  ARGSS::ARPG::Init();
  /*ARGSS::ARPG::ACache::Init();
  ARGSS::ARPG::ASprite::Init();
  ARGSS::ARPG::AWeather::Init();*/
}
