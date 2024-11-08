#ifndef INCLUDE_VALIDATION_H
#define INCLUDE_VALIDATION_H

#include "String.h"

#define MAL_CHECK(condition, ...)  \
    if (!(condition)) { throw STRF(__VA_ARGS__); } else { }

#define MAL_FAIL(...) MAL_CHECK(false, __VA_ARGS__)

#define MALTYPE_ERR_STR(name, prog) \
        const malValuePtr type = mal::type(name->type()); \
        const String typeError = "'" + String(prog) + "': type is " + type->print(true);

#define ERROR_STR_TYPE(name, prog) \
    "'" prog "': type is " name

#define MAL_TYPE_FAIL(name, nameType) \
    MALTYPE_ERR_STR(name, nameType) \
    MAL_FAIL(typeError.c_str());

#define CHECK_IS_NUMBER(name) \
    MAL_CONSTAND_FAIL_CHECK(name, "nil", "number?") \
    MAL_CONSTAND_FAIL_CHECK(name, "false", "number?") \
    MAL_CONSTAND_FAIL_CHECK(name, "true", "number?") \
    if (!(name->type() == MALTYPE::INT) && !(name->type() == MALTYPE::REAL)) { \
        MAL_TYPE_FAIL(name, "number?") \
    }

#define MAL_CONSTAND_FAIL_CHECK(name, c, nameType) MAL_CHECK(!(name->print(true).compare(c) == 0), ERROR_STR_TYPE(c, nameType));

extern int checkArgsIs(const char* name, int expected, int got);
extern int checkArgsBetween(const char* name, int min, int max, int got);
extern int checkArgsAtLeast(const char* name, int min, int got);
extern int checkArgsEven(const char* name, int got);

#endif // INCLUDE_VALIDATION_H
