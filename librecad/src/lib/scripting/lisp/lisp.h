#ifndef LISP_H
#define LISP_H
#include <MAL.h>

#include <stdio.h>
#include <streambuf>
#include <string>
#include <ostream>
#include <iostream>
#include <sstream>

int Lisp_Initialize(int argc=0, char* argv[]=NULL);

int LispRun_SimpleFile(const char *filename);

int LispRun_SimpleString(const char *command);

std::string Lisp_EvalString(const String& input);

std::string Lisp_EvalFile(const char *filename);

#endif // LISP_H
