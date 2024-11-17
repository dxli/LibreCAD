#ifndef LISP_H
#define LISP_H
#include <LCL.h>

#include <stdio.h>
#include <streambuf>
#include <string>
#include <ostream>
#include <iostream>
#include <sstream>

#ifdef DEVELOPER

#define LISP_COPYRIGHT \
"\nType (help), (copyright), (credits) or (license) " \
    "for more information.\n"

// LISP_VERSION
#define REL "1.0.1"
#define BUILD_TYPE "devel"
#ifdef _WINDOWS
#define COMPILER "MSC"
#define HOST "windows"
#else
#define COMPILER "GCC"
#define HOST "linux"
#endif
#define VERSION_STR(rel, build, date, time) "LibreLisp " rel " (" build ", " date ", " time ") [" COMPILER " " __VERSION__ "] on " HOST
#define LISP_VERSION VERSION_STR(REL,BUILD_TYPE,__DATE__,__TIME__)

const char *Lisp_GetVersion();

int Lisp_Initialize(int argc=0, char* argv[]=NULL);

int LispRun_SimpleFile(const char *filename);

int LispRun_SimpleString(const char *command);

std::string Lisp_EvalString(const String& input);

std::string Lisp_EvalFile(const char *filename);

#endif // DEVELOPER

#endif // LISP_H
