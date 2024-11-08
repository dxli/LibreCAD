#ifndef INCLUDE_MAL_H
#define INCLUDE_MAL_H

#include "Debug.h"
#include "RefCountedPtr.h"
#include "String.h"
#include "Validation.h"
#include <sstream>

#include <vector>

#define BUILD_TYPE "devel"

#ifdef _WINDOWS
#define COMPILER "MSC"
#define HOST "windows"
#else
#define COMPILER "GCC"
#define HOST "linux"
#endif

#define REL "1.0"
#define VERSION_STR(rel, build, date, time) "LibreLisp " rel " (" build ", " date ", " time ") [" COMPILER " " __VERSION__ "] on " HOST
#define LISP_VERSION VERSION_STR(REL,BUILD_TYPE,__DATE__,__TIME__)

namespace emb
{
extern std::stringstream cout;
}

class malValue;
typedef RefCountedPtr<malValue>  malValuePtr;
typedef std::vector<malValuePtr> malValueVec;
typedef malValueVec::iterator    malValueIter;

class malEnv;
typedef RefCountedPtr<malEnv>    malEnvPtr;

// step*.cpp
extern malValuePtr APPLY(malValuePtr op,
                         malValueIter argsBegin, malValueIter argsEnd);
extern malValuePtr EVAL(malValuePtr ast, malEnvPtr env);
//extern malValuePtr readline(const String& prompt);
extern String rep(const String& input, malEnvPtr env);

// Core.cpp
extern void installCore(malEnvPtr env);

// Reader.cpp
extern malValuePtr readStr(const String& input);
extern malValuePtr loadDcl(const String& path);

#endif // INCLUDE_MAL_H
