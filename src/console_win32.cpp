#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include "console_win32.h"
#include "argss.h"

static VALUE argss_console_create(VALUE self)
{
    int hConHandle;
    long lStdHandle;
    FILE *fp;
    // allocate a console for this app
    AllocConsole();
    // redirect unbuffered STDOUT to the console
    lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
    fp = _fdopen( hConHandle, "w" );
    *stdout = *fp;
    setvbuf( stdout, NULL, _IONBF, 0 );
    // redirect unbuffered STDIN to the console
    lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
    fp = _fdopen( hConHandle, "r" );
    *stdin = *fp;
    setvbuf( stdin, NULL, _IONBF, 0 );
    // redirect unbuffered STDERR to the console
    lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
    fp = _fdopen( hConHandle, "w" );
    *stderr = *fp;
    setvbuf( stderr, NULL, _IONBF, 0 );
    // make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
    // point to console as well
    std::ios::sync_with_stdio();
    Sleep(10);
    return Qtrue;
}


static VALUE argss_console_set_max_lines(VALUE self, VALUE lines)
{
    CONSOLE_SCREEN_BUFFER_INFO coninfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
    coninfo.dwSize.Y = lines;
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);
    return Qtrue;
}

static VALUE argss_console_set_title(VALUE self, VALUE title)
{
    //return INT2BOOL(SetConsoleTitle(StringValuePtr(title)) != 0);
    return 0;
}

static VALUE argss_console_dispose(VALUE self)
{
    return INT2BOOL(FreeConsole() != 0);
}

void Init_Console()
{
    typedef VALUE (*rubyfunc)(...);
    ARGSS_Console = rb_define_module("Console");
    rb_define_singleton_method(ARGSS_Console, "initialize", (rubyfunc)argss_console_create, 0);
    rb_define_singleton_method(ARGSS_Console, "set_max_lines", (rubyfunc)argss_console_set_max_lines, 1);
    rb_define_singleton_method(ARGSS_Console, "set_title", (rubyfunc)argss_console_set_title, 1);
    rb_define_singleton_method(ARGSS_Console, "dispose", (rubyfunc)argss_console_dispose, 0);
}