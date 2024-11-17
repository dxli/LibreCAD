#ifndef INCLUDE_LCL_H
#define INCLUDE_LCL_H

#include "Debug.h"
#include "RefCountedPtr.h"
#include "String.h"
#include "Validation.h"
#include <sstream>

#include <vector>

#ifdef DEVELOPER

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

class lclValue;
typedef RefCountedPtr<lclValue>  lclValuePtr;
typedef std::vector<lclValuePtr> lclValueVec;
typedef lclValueVec::iterator    lclValueIter;

class lclEnv;
typedef RefCountedPtr<lclEnv>    lclEnvPtr;

// lisp.cpp
extern lclValuePtr APPLY(lclValuePtr op,
                         lclValueIter argsBegin, lclValueIter argsEnd);
extern lclValuePtr EVAL(lclValuePtr ast, lclEnvPtr env);

extern String rep(const String& input, lclEnvPtr env);

// Core.cpp
extern void installCore(lclEnvPtr env);
extern String noQuotes(const String& s);

// Reader.cpp
extern lclValuePtr readStr(const String& input);
extern lclValuePtr loadDcl(const String& path);

#endif // DEVELOPER

#endif // INCLUDE_LCL_H
