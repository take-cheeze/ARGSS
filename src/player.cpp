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
#include "player.h"
#include "options.h"
#include "output.h"
#include "config.h"
#include "audio.h"
#include "input.h"
#include "graphics.h"


///////////////////////////////////////////////////////////
// Global Variables
///////////////////////////////////////////////////////////
WindowUi* Player::main_window;
bool Player::focus;
bool Player::alt_pressing;

///////////////////////////////////////////////////////////
/// Initialize
///////////////////////////////////////////////////////////
void Player::Init() {
  main_window = new WindowUi(Config::Width, Config::Height, Config::Title, true, RUN_FULLSCREEN);

  focus = true;
  alt_pressing = false;
}

///////////////////////////////////////////////////////////
/// Initialize
///////////////////////////////////////////////////////////
void Player::Update() {
  Event evnt;
  bool result;

  do {
    result = main_window->GetEvent(evnt);
    if (evnt.type == Event::Quit) {
      // ARuby::Exit();
      Player::Exit();
      exit(0);
      return;
    } else if (evnt.type == Event::KeyDown) {
      if (evnt.param1 == Input::Keys::ALT) {
        alt_pressing = true;
      } else if (evnt.param1 == Input::Keys::RETURN && alt_pressing) {
        ToggleFullscreen();
        alt_pressing = false;
      }
    } else if (evnt.type == Event::KeyUp) {
      if (evnt.param1 == Input::Keys::ALT) {
        alt_pressing = false;
      }
    }
    #if (PAUSE_GAME_WHEN_FOCUS_LOST == YES)
      else {
        if (evnt.type == Event::GainFocus && !focus) {
          focus = true;
          Graphics::TimerContinue();
          #if (PAUSE_AUDIO_WHEN_FOCUS_LOST)
            Audio::Continue();
          #endif
        } else if (evnt.type == Event::LostFocus && focus) {
          focus = false;
          Input::ClearKeys();
          Graphics::TimerWait();
          #if (PAUSE_AUDIO_WHEN_FOCUS_LOST)
            Audio::Pause();
          #endif
        }
      }
    #endif
#if PAUSE_GAME_WHEN_FOCUS_LOST == YES
  } while(result || !focus);
#else
  } while(result);
#endif
}

///////////////////////////////////////////////////////////
/// Exit
///////////////////////////////////////////////////////////
void Player::Exit() {
  Graphics::Exit();
  Output::None();
  main_window->Dispose();
}

///////////////////////////////////////////////////////////
/// Switch fullscreen
///////////////////////////////////////////////////////////
void Player::ToggleFullscreen() {
  bool toggle = !main_window->IsFullscreen();
  main_window->Dispose();
  delete main_window;
  main_window = new WindowUi(Config::Width, Config::Height, Config::Title, true, toggle);
  Graphics::InitOpenGL();
  Graphics::RefreshAll();
}

///////////////////////////////////////////////////////////
/// Resize window
///////////////////////////////////////////////////////////
void Player::ResizeWindow(long width, long height) {
  main_window->Resize(width, height);
}

///////////////////////////////////////////////////////////
/// Get window width
///////////////////////////////////////////////////////////
int Player::GetWidth() {
  return main_window->GetWidth();
}

///////////////////////////////////////////////////////////
/// Get window height
///////////////////////////////////////////////////////////
int Player::GetHeight() {
  return main_window->GetHeight();
}

///////////////////////////////////////////////////////////
/// Swap Buffers
///////////////////////////////////////////////////////////
void Player::SwapBuffers() {
  main_window->SwapBuffers();
}
