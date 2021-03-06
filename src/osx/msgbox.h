/*
 *  msgbox_osx.h
 *  xcode
 *
 *  Created by Nicolas Pépin Perreault on 10-07-15.
 *  Copyright 2010 McGill University. All rights reserved.
 *
 */

#ifndef _MSGBOX_OSX_H_
#define _MSGBOX_OSX_H_

////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include <string>

////////////////////////////////////////////////////////////
/// Defines
////////////////////////////////////////////////////////////
#define MSGBOX_LEVEL_INFO    0
#define MSGBOX_LEVEL_ERROR    1
#define MSGBOX_LEVEL_WARNING  2

////////////////////////////////////////////////////////////
/// Message Box namespace
////////////////////////////////////////////////////////////
namespace MsgBox {
  void OK(std::string msg, std::string title);
  void Error(std::string msg, std::string title);
  void Warning(std::string msg, std::string title);
};

#endif