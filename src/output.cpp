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
#include <iostream>
#include <fstream>
#include <cstdlib>
#ifdef WIN32
#include <conio.h>
#endif
#include <stdarg.h>
#include "output.h"
#include "options.h"
#include "config.h"
#include "player.h"
#include "console.h"
#include "msgbox.h"
#include "graphics.h"

void Output::raise(boost::format const& f) {
  raise(f.str());
}

///////////////////////////////////////////////////////////
// Global Variables
///////////////////////////////////////////////////////////
int Output::output_type;
std::string Output::filename;

///////////////////////////////////////////////////////////
/// Output Initialize
///////////////////////////////////////////////////////////
void Output::Init(){
  output_type = OUTPUT_TYPE;
  filename = OUTPUT_FILE;
}

///////////////////////////////////////////////////////////
/// Output Error
///////////////////////////////////////////////////////////
void Output::Error(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);

  char str[256];
  vsprintf(str, fmt, args);
  Output::ErrorStr((std::string)str);

  va_end(args);
}
void Output::ErrorStr(std::string const& err){
  PostStr(err);
  if (Console::Active()) {
    Post("\nARGSS will close now. Press any key...");
#ifdef WIN32
    getch();
#endif
  }
  Player::Exit();
  exit(-1);
}

///////////////////////////////////////////////////////////
/// Output Warning
///////////////////////////////////////////////////////////
void Output::Warning(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);

  char str[256];
#ifdef MSVC
  vsprintf_s(str, 256, fmt, args);
#else
  vsprintf(str, fmt, args);
#endif
  Output::WarningStr((std::string)str);

  va_end(args);
}
void Output::WarningStr(std::string const& warn) {
  PostStr(warn);
}

///////////////////////////////////////////////////////////
/// Output Post message
///////////////////////////////////////////////////////////
void Output::Post(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);

  char str[256];
  vsprintf(str, fmt, args);
  Output::PostStr((std::string)str);

  va_end(args);
}
void Output::PostStr(std::string const& msg) {
  Graphics::TimerWait();
  switch (output_type) {
  case 1:
    if (Console::Active()) Console::Write(msg);
    break;
  case 2:
    if (!Console::Active()) Console::Init();
    Console::Write(msg);
    break;
  case 3:
    MsgBox::OK(msg, Config::Title);
    break;
  case 4:
    PostFile(msg);
    break;
  case 5:
    if (Console::Active()) Console::Write(msg);
    else PostFile(msg);
    break;
  case 6:
    if (Console::Active()) Console::Write(msg);
    else MsgBox::OK(msg, Config::Title);
  }
  Graphics::TimerContinue();
}

///////////////////////////////////////////////////////////
/// Output File
///////////////////////////////////////////////////////////
void Output::PostFile(std::string const& msg) {
  std::ofstream file;
  file.open(filename.c_str(), std::ios::out | std::ios::app);
  file << msg;
  file.close();
}

///////////////////////////////////////////////////////////
/// Output Console
///////////////////////////////////////////////////////////
void Output::Console() {
  if (!Console::Active()) Console::Init();
  output_type = 2;
}

///////////////////////////////////////////////////////////
/// Output Console
///////////////////////////////////////////////////////////
void Output::MsgBox() {
  if (Console::Active()) Console::Free();
  output_type = 3;
}

///////////////////////////////////////////////////////////
/// Output Console
///////////////////////////////////////////////////////////
void Output::File(std::string name) {
  if (Console::Active()) Console::Free();
  output_type = 4;
  filename = name;
}

///////////////////////////////////////////////////////////
/// Output Console
///////////////////////////////////////////////////////////
void Output::None() {
  if (Console::Active()) Console::Free();
  output_type = 0;
}

///////////////////////////////////////////////////////////
/// Output Console
///////////////////////////////////////////////////////////
std::string Output::Gets() {
  char string[256] = "\0";
  if (Console::Active()) {
    gets(string);
  }
  return string;
}

///////////////////////////////////////////////////////////
/// Output Console
///////////////////////////////////////////////////////////
std::string Output::Getc() {
  std::string chr = "";
#ifdef WIN32
  if (Console::Active()) {
    chr += (char)getch();
  }
#endif
  return chr;
}

///////////////////////////////////////////////////////////
// AudioNotLoad
///////////////////////////////////////////////////////////
void Output::AudioNotLoad(std::string const& type, std::string const& file) {

  raise(boost::format("couldn't load %s %s.\n") % file % type);
}

void Output::AudioNotLoad(std::string const& type, std::string const& file, std::string const& description) {
  raise(boost::format("couldn't load %s %s.\n%s\n") % file % type % description);
}

///////////////////////////////////////////////////////////
// AudioNotPlay
///////////////////////////////////////////////////////////
void Output::AudioNotPlay(std::string const& type, std::string const& file) {
  raise(boost::format("couldn't play %s %s.\n") % file % type);
}

void Output::AudioNotPlay(std::string const& type, std::string const& file, std::string const& description) {
  raise(boost::format("couldn't play %s %s.\n%s\n") % file % type % description);
}
