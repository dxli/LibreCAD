#include "rs_python.h"
#include "LCL.h"
#include "Environment.h"
#include "StaticList.h"
#include "Types.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <stdio.h>
#include <cstdlib>
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <QMessageBox>
#include <QFileDialog>

/* temp defined */
#include <regex>

unsigned int tmpFileCount = 0;

typedef std::regex Regex;

static const Regex intRegex("^[-+]?\\d+$");
static const Regex floatRegex("^[+-]?\\d+[.]{1}\\d+$");
static const Regex floatPointRegex("[.]{1}\\d+$");

#include <math.h>
#include <cmath>

#define CHECK_ARGS_IS(expected) \
    checkArgsIs(name.c_str(), expected, \
                  std::distance(argsBegin, argsEnd))

#define CHECK_ARGS_BETWEEN(min, max) \
    checkArgsBetween(name.c_str(), min, max, \
                       std::distance(argsBegin, argsEnd))

#define CHECK_ARGS_AT_LEAST(expected) \
    checkArgsAtLeast(name.c_str(), expected, \
                        std::distance(argsBegin, argsEnd))

#define FLOAT_PTR \
    (argsBegin->ptr()->type() == LCLTYPE::REAL)

#define INT_PTR \
    (argsBegin->ptr()->type() == LCLTYPE::INT)

#define NIL_PTR \
    (argsBegin->ptr()->print(true).compare("nil") == 0)

#define TRUE_PTR \
    (argsBegin->ptr()->print(true).compare("true") == 0)

#define T_PTR \
    (argsBegin->ptr()->print(true).compare("T") == 0)

#define FALSE_PTR \
    (argsBegin->ptr()->print(true).compare("false") == 0)

bool argsHasFloat(lclValueIter argsBegin, lclValueIter argsEnd)
{
    for (auto it = argsBegin; it != argsEnd; ++it) {
        if (it->ptr()->type() == LCLTYPE::REAL) {
            return true;
        }
    }
    return false;
}

#define ARGS_HAS_FLOAT \
    argsHasFloat(argsBegin, argsEnd)

#define AG_INT(name) \
    CHECK_IS_NUMBER(argsBegin->ptr()) \
    lclInteger* name = VALUE_CAST(lclInteger, *argsBegin++)

#define ADD_INT_VAL(val) \
    CHECK_IS_NUMBER(argsBegin->ptr()) \
    lclInteger val = dynamic_cast<lclInteger*>(argsBegin->ptr());

#define ADD_FLOAT_VAL(val) \
    CHECK_IS_NUMBER(argsBegin->ptr()) \
    lclDouble val = dynamic_cast<lclDouble*>(argsBegin->ptr());

#define ADD_LIST_VAL(val) \
    lclList val = dynamic_cast<lclList*>(argsBegin->ptr());

#define SET_INT_VAL(opr, checkDivByZero) \
    ADD_INT_VAL(*intVal) \
    intValue = intValue opr intVal->value(); \
    if (checkDivByZero) { \
        LCL_CHECK(intVal->value() != 0, "Division by zero"); }

#define SET_FLOAT_VAL(opr, checkDivByZero) \
    if (FLOAT_PTR) \
    { \
        ADD_FLOAT_VAL(*floatVal) \
        floatValue = floatValue opr floatVal->value(); \
        if (checkDivByZero) { \
            LCL_CHECK(floatVal->value() != 0.0, "Division by zero"); } \
    } \
    else \
    { \
        ADD_INT_VAL(*intVal) \
        floatValue = floatValue opr double(intVal->value()); \
        if (checkDivByZero) { \
            LCL_CHECK(intVal->value() != 0, "Division by zero"); } \
    }

static String printValues(lclValueIter begin, lclValueIter end,
                           const String& sep, bool readably);

static int countValues(lclValueIter begin, lclValueIter end);

static StaticList<lclBuiltIn*> handlers;

#define ARG(type, name) type* name = VALUE_CAST(type, *argsBegin++)

#define FUNCNAME(uniq) builtIn ## uniq
#define HRECNAME(uniq) handler ## uniq
#define BUILTIN_DEF(uniq, symbol) \
    static lclBuiltIn::ApplyFunc FUNCNAME(uniq); \
    static StaticList<lclBuiltIn*>::Node HRECNAME(uniq) \
        (handlers, new lclBuiltIn(symbol, FUNCNAME(uniq))); \
    lclValuePtr FUNCNAME(uniq)(const String& name, \
        lclValueIter argsBegin, lclValueIter argsEnd)

#define BUILTIN(symbol)  BUILTIN_DEF(__LINE__, symbol)

#define BUILIN_ALIAS(uniq) \
    FUNCNAME(uniq)(name, argsBegin, argsEnd)

#define BUILTIN_ISA(symbol, type) \
    BUILTIN(symbol) { \
        CHECK_ARGS_IS(1); \
        return lcl::boolean(DYNAMIC_CAST(type, *argsBegin)); \
    }

#define BUILTIN_IS(op, constant) \
    BUILTIN(op) { \
        CHECK_ARGS_IS(1); \
        return lcl::boolean(*argsBegin == lcl::constant()); \
    }

#define BUILTIN_INTOP(op, checkDivByZero) \
    BUILTIN(#op) { \
        BUILTIN_VAL(op, checkDivByZero); \
        }

#define BUILTIN_VAL(opr, checkDivByZero) \
    CHECK_ARGS_AT_LEAST(2); \
    if (ARGS_HAS_FLOAT) { \
        BUILTIN_FLOAT_VAL(opr, checkDivByZero) \
    } else { \
        BUILTIN_INT_VAL(opr, checkDivByZero) \
    }

#define BUILTIN_FLOAT_VAL(opr, checkDivByZero) \
    [[maybe_unused]] double floatValue = 0; \
    SET_FLOAT_VAL(+, false); \
    argsBegin++; \
    do { \
        SET_FLOAT_VAL(opr, checkDivByZero); \
        argsBegin++; \
    } while (argsBegin != argsEnd); \
    return lcl::mdouble(floatValue);

#define BUILTIN_INT_VAL(opr, checkDivByZero) \
    [[maybe_unused]] int64_t intValue = 0; \
    SET_INT_VAL(+, false); \
    argsBegin++; \
    do { \
        SET_INT_VAL(opr, checkDivByZero); \
        argsBegin++; \
    } while (argsBegin != argsEnd); \
    return lcl::integer(intValue);

#define BUILTIN_FUNCTION(foo) \
    CHECK_ARGS_IS(1); \
    if (FLOAT_PTR) { \
        ADD_FLOAT_VAL(*lhs) \
        return lcl::mdouble(foo(lhs->value())); } \
    else { \
        ADD_INT_VAL(*lhs) \
        return lcl::mdouble(foo(lhs->value())); }

#define BUILTIN_OP_COMPARE(opr) \
    CHECK_ARGS_IS(2); \
    if (((argsBegin->ptr()->type() == LCLTYPE::LIST) && ((argsBegin + 1)->ptr()->type() == LCLTYPE::LIST)) || \
        ((argsBegin->ptr()->type() == LCLTYPE::VEC) && ((argsBegin + 1)->ptr()->type() == LCLTYPE::VEC))) { \
        ARG(lclSequence, lhs); \
        ARG(lclSequence, rhs); \
        return lcl::boolean(lhs->count() opr rhs->count()); } \
    if (ARGS_HAS_FLOAT) { \
        if (FLOAT_PTR) { \
            ADD_FLOAT_VAL(*floatLhs) \
            argsBegin++; \
            if (FLOAT_PTR) { \
                ADD_FLOAT_VAL(*floatRhs) \
                return lcl::boolean(floatLhs->value() opr floatRhs->value()); } \
            else { \
               ADD_INT_VAL(*intRhs) \
               return lcl::boolean(floatLhs->value() opr double(intRhs->value())); } } \
        else { \
            ADD_INT_VAL(*intLhs) \
            argsBegin++; \
            ADD_FLOAT_VAL(*floatRhs) \
            return lcl::boolean(double(intLhs->value()) opr floatRhs->value()); } } \
    else { \
        ADD_INT_VAL(*intLhs) \
        argsBegin++; \
        ADD_INT_VAL(*intRhs) \
        return lcl::boolean(intLhs->value() opr intRhs->value()); }

// helper foo to cast integer (64 bit) type to char (8 bit) type
unsigned char itoa64(const int64_t &sign)
{
    int64_t bit64[8];
    unsigned char result = 0;

    if(sign < 0)
    {
        std::cout << "Warning: out of char value!" << std::endl;
        return result;
    }

    for (int i = 0; i < 8; i++)
    {
        bit64[i] = (sign >> i) & 1;
        if (bit64[i])
        {
            result |= 1 << i;
        }
    }
    return result;
}

int kbhit()
{
    static const int STDIN = 0;
    static bool initialized = false;

    if (! initialized) {
        // Use termios to turn off line buffering
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}

bool compareNat(const String& a, const String& b)
{
    //std::cout << a << " " << b << std::endl;
    if (a.empty()) {
        return true;
    }
    if (b.empty()) {
        return false;
    }
    if (std::isdigit(a[0]) && !std::isdigit(b[0])) {
        return false;
    }
    if (!std::isdigit(a[0]) && std::isdigit(b[0])) {
        return false;
    }
    if (!std::isdigit(a[0]) && !std::isdigit(b[0])) {
        //std::cout << "HIT no dig" << std::endl;
        if (a[0] == '.' &&
            b[0] == '.' &&
            a.size() > 1 &&
            b.size() > 1) {
            return (std::toupper(a[1]) < std::toupper(b[1]));
        }

        if (std::toupper(a[0]) == std::toupper(b[0])) {
            return compareNat(a.substr(1), b.substr(1));
        }
        return (std::toupper(a[0]) < std::toupper(b[0]));
    }

    // Both strings begin with digit --> parse both numbers
    std::istringstream issa(a);
    std::istringstream issb(b);
    int ia, ib;
    issa >> ia;
    issb >> ib;
    if (ia != ib)
        return ia < ib;

    // Numbers are the same --> remove numbers and recurse
    String anew, bnew;
    std::getline(issa, anew);
    std::getline(issb, bnew);
    return (compareNat(anew, bnew));
}

template <typename TP>
std::time_t to_time_t(TP tp)
{
    using namespace std::chrono;
    auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
              + system_clock::now());
    return system_clock::to_time_t(sctp);
}

BUILTIN_ISA("atom?",        lclAtom);
BUILTIN_ISA("double?",      lclDouble);
BUILTIN_ISA("file?",        lclFile);
BUILTIN_ISA("integer?",     lclInteger);
BUILTIN_ISA("keyword?",     lclKeyword);
BUILTIN_ISA("list?",        lclList);
BUILTIN_ISA("map?",         lclHash);
BUILTIN_ISA("sequential?",  lclSequence);
BUILTIN_ISA("string?",      lclString);
BUILTIN_ISA("symbol?",      lclSymbol);
BUILTIN_ISA("vector?",      lclVector);

BUILTIN_INTOP(+,            false);
BUILTIN_INTOP(/,            true);
BUILTIN_INTOP(*,            false);

BUILTIN_IS("true?",         trueValue);
BUILTIN_IS("false?",        falseValue);
BUILTIN_IS("nil?",          nilValue);

BUILTIN("-")
{
    if (CHECK_ARGS_AT_LEAST(1) == 1)
    {
        if (FLOAT_PTR)
        {
            ADD_FLOAT_VAL(*lhs)
            return lcl::mdouble(-lhs->value());
        }
        else
        {
            ADD_INT_VAL(*lhs)
            return lcl::integer(-lhs->value());
        }
    }
    CHECK_ARGS_AT_LEAST(2);
    if (ARGS_HAS_FLOAT) {
        BUILTIN_FLOAT_VAL(-, false);
    } else {
        BUILTIN_INT_VAL(-, false);
    }
}

BUILTIN("%")
{
    CHECK_ARGS_AT_LEAST(2);
    if (ARGS_HAS_FLOAT) {
        return lcl::nilValue();
    } else {
        BUILTIN_INT_VAL(%, false);
    }
}

BUILTIN("<=")
{
    BUILTIN_OP_COMPARE(<=);
}

BUILTIN(">=")
{
    BUILTIN_OP_COMPARE(>=);
}

BUILTIN("<")
{
    BUILTIN_OP_COMPARE(<);
}

BUILTIN(">")
{
    BUILTIN_OP_COMPARE(>);
}

BUILTIN("=")
{
    CHECK_ARGS_IS(2);
    const lclValue* lhs = (*argsBegin++).ptr();
    const lclValue* rhs = (*argsBegin++).ptr();

    return lcl::boolean(lhs->isEqualTo(rhs));
}

BUILTIN("/=")
{
    CHECK_ARGS_IS(2);
    const lclValue* lhs = (*argsBegin++).ptr();
    const lclValue* rhs = (*argsBegin++).ptr();

    return lcl::boolean(!lhs->isEqualTo(rhs));
}

BUILTIN("~ ")
{
    Q_UNUSED(argsEnd);
    Q_UNUSED(name);
    if (FLOAT_PTR)
    {
        return lcl::nilValue();
    }
    else
    {
        ADD_INT_VAL(*lhs)
        return lcl::integer(~lhs->value());
    }
}

BUILTIN("1+")
{
    CHECK_ARGS_IS(1);
    if (FLOAT_PTR)
    {
        ADD_FLOAT_VAL(*lhs)
        return lcl::mdouble(lhs->value()+1);
    }
    else
    {
        ADD_INT_VAL(*lhs)
        return lcl::integer(lhs->value()+1);
    }
}

BUILTIN("1-")
{
    CHECK_ARGS_IS(1);
    if (FLOAT_PTR)
    {
        ADD_FLOAT_VAL(*lhs)
        return lcl::mdouble(lhs->value()-1);
    }
    else
    {
        ADD_INT_VAL(*lhs)
        return lcl::integer(lhs->value()-1);
    }
}

BUILTIN("abs")
{
    CHECK_ARGS_IS(1);
    if (FLOAT_PTR)
    {
        ADD_FLOAT_VAL(*lhs)
        return lcl::mdouble(abs(lhs->value()));
    }
    else
    {
        ADD_INT_VAL(*lhs)
        return lcl::integer(abs(lhs->value()));
    }
}

BUILTIN("alert")
{
    CHECK_ARGS_IS(1);
    ARG(lclString, str);

    QMessageBox msgBox;
    msgBox.setWindowTitle("LibreCAD");
    msgBox.setText(str->value().c_str());
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();

    return lcl::nilValue();
}

BUILTIN("apply")
{
    CHECK_ARGS_AT_LEAST(2);
    lclValuePtr op = *argsBegin++; // this gets checked in APPLY

    // both LISPs
    if (op->type() == LCLTYPE::SYM ||
        op->type() == LCLTYPE::LIST) {
        op = EVAL(op, NULL);
    }

    // Copy the first N-1 arguments in.
    lclValueVec args(argsBegin, argsEnd-1);

    // Then append the argument as a list.
    const lclSequence* lastArg = VALUE_CAST(lclSequence, *(argsEnd-1));
    for (int i = 0; i < lastArg->count(); i++) {
        args.push_back(lastArg->item(i));
    }

    return APPLY(op, args.begin(), args.end());
}

BUILTIN("ascii")
{
    CHECK_ARGS_IS(1);
    const lclValuePtr arg = *argsBegin++;

    if (const lclString* s = DYNAMIC_CAST(lclString, arg))
    {
        return lcl::integer(int(s->value().c_str()[0]));
    }

    return lcl::integer(0);
}

BUILTIN("assoc")
{
    CHECK_ARGS_AT_LEAST(1);
    //both LISPs
    if (!(argsBegin->ptr()->type() == LCLTYPE::MAP)) {
        lclValuePtr op = *argsBegin++;
        ARG(lclSequence, seq);

        const int length = seq->count();
        lclValueVec* items = new lclValueVec(length);
        std::copy(seq->begin(), seq->end(), items->begin());

        for (int i = 0; i < length; i++) {
            if (items->at(i)->type() == LCLTYPE::LIST) {
                lclList* list = VALUE_CAST(lclList, items->at(i));
                if (list->count() == 2) {
                    lclValueVec* duo = new lclValueVec(2);
                    std::copy(list->begin(), list->end(), duo->begin());
                    if (duo->begin()->ptr()->print(true).compare(op->print(true)) == 0) {
                        return list;
                    }
                }
                if (list->count() == 3) {
                    lclValueVec* dotted = new lclValueVec(3);
                    std::copy(list->begin(), list->end(), dotted->begin());
                    if (dotted->begin()->ptr()->print(true).compare(op->print(true)) == 0
                        && (dotted->at(1)->print(true).compare(".") == 0)
                    ) {
                        return list;
                    }
                }
            }
        }
        return lcl::nilValue();
    }
    ARG(lclHash, hash);

    return hash->assoc(argsBegin, argsEnd);
}

BUILTIN("atan")
{
    BUILTIN_FUNCTION(atan);
}

BUILTIN("atof")
{
    CHECK_ARGS_IS(1);
    const lclValuePtr arg = *argsBegin++;

    if (const lclString* s = DYNAMIC_CAST(lclString, arg))
    {
        if(std::regex_match(s->value().c_str(), intRegex) ||
            std::regex_match(s->value().c_str(), floatRegex))
            {
                return lcl::mdouble(atof(s->value().c_str()));
            }
    }
    return lcl::mdouble(0);
}

BUILTIN("atoi")
{
    CHECK_ARGS_IS(1);
    const lclValuePtr arg = *argsBegin++;

    if (const lclString* s = DYNAMIC_CAST(lclString, arg))
    {
        if (std::regex_match(s->value().c_str(), intRegex))
        {
            return lcl::integer(atoi(s->value().c_str()));
        }
        if (std::regex_match(s->value().c_str(), floatRegex))
        {
            return lcl::integer(atoi(std::regex_replace(s->value().c_str(),
                                                        floatPointRegex, "").c_str()));
        }
    }
    return lcl::integer(0);
}

BUILTIN("atom")
{
    CHECK_ARGS_IS(1);

    return lcl::atom(*argsBegin);
}

BUILTIN("boolean?")
{
    CHECK_ARGS_IS(1);
    {
        return lcl::boolean(argsBegin->ptr()->type() == LCLTYPE::BOOLEAN);
    }
}

#if 0
BUILTIN("car")
{
    CHECK_ARGS_IS(1);
    ARG(lclSequence, seq);

    LCL_CHECK(0 < seq->count(), "Index out of range");

    return seq->first();
}
#endif
BUILTIN("cadr")
{
    CHECK_ARGS_IS(1);
    ARG(lclSequence, seq);

    LCL_CHECK(1 < seq->count(), "Index out of range");

    return seq->item(1);
}

BUILTIN("caddr")
{
    CHECK_ARGS_IS(1);
    ARG(lclSequence, seq);

    LCL_CHECK(2 < seq->count(), "Index out of range");

    return seq->item(2);
}

BUILTIN("cdr")
{
    CHECK_ARGS_IS(1);
    if (*argsBegin == lcl::nilValue()) {
        return lcl::list(new lclValueVec(0));
    }
    ARG(lclSequence, seq);
    if (seq->isDotted()) {
            return seq->dotted();
    }
    return seq->rest();
}


BUILTIN("chr")
{
    CHECK_ARGS_IS(1);
    unsigned char sign = 0;

    if (FLOAT_PTR)
    {
        ADD_FLOAT_VAL(*lhs)
        auto sign64 = static_cast<std::int64_t>(lhs->value());
        sign = itoa64(sign64);
    }
    else
    {
        ADD_INT_VAL(*lhs)
        sign = itoa64(lhs->value());
    }

    return lcl::string(String(1 , sign));
}

BUILTIN("close")
{
    CHECK_ARGS_IS(1);
    ARG(lclFile, pf);

    return pf->close();
}

BUILTIN("command")
{
    CHECK_ARGS_AT_LEAST(1);
    for (auto it = argsBegin; it != argsEnd; ++it) {
        const lclValuePtr arg = *it;
        std::cout << "parameter: " << it->ptr()->print(true) << "type: " << (int)it->ptr()->type() << std::endl;
    }

    return lcl::nilValue();
}

BUILTIN("concat")
{
    Q_UNUSED(name);
    int count = 0;
    for (auto it = argsBegin; it != argsEnd; ++it) {
        const lclSequence* seq = VALUE_CAST(lclSequence, *it);
        count += seq->count();
    }

    lclValueVec* items = new lclValueVec(count);
    int offset = 0;
    for (auto it = argsBegin; it != argsEnd; ++it) {
        const lclSequence* seq = STATIC_CAST(lclSequence, *it);
        std::copy(seq->begin(), seq->end(), items->begin() + offset);
        offset += seq->count();
    }

    return lcl::list(items);
}
#if 0
BUILTIN("bla")
{
    return BUILIN_ALIAS(543);
    //return builtIn540(name, argsBegin, argsEnd);
}
#endif
BUILTIN("conj")
{
    CHECK_ARGS_AT_LEAST(1);
    ARG(lclSequence, seq);

    return seq->conj(argsBegin, argsEnd);
}

BUILTIN("cons")
{
    CHECK_ARGS_IS(2);
    lclValuePtr first = *argsBegin++;
    lclValuePtr second = *argsBegin;

    if (second->type() == LCLTYPE::INT ||
        second->type() == LCLTYPE::REAL ||
        second->type() == LCLTYPE::STR)
    {
        lclValueVec* items = new lclValueVec(3);
        items->at(0) = first;
        items->at(1) = new lclSymbol(".");
        items->at(2) = second;
        return lcl::list(items);
    }

    ARG(lclSequence, rest);

    lclValueVec* items = new lclValueVec(1 + rest->count());
    items->at(0) = first;
    std::copy(rest->begin(), rest->end(), items->begin() + 1);

    return lcl::list(items);
}

BUILTIN("contains?")
{
    CHECK_ARGS_IS(2);
    if (*argsBegin == lcl::nilValue()) {
        return *argsBegin;
    }
    ARG(lclHash, hash);
    return lcl::boolean(hash->contains(*argsBegin));
}

BUILTIN("cos")
{
    BUILTIN_FUNCTION(cos);
}

BUILTIN("count")
{
    CHECK_ARGS_IS(1);
    if (*argsBegin == lcl::nilValue()) {
        return lcl::integer(0);
    }

    ARG(lclSequence, seq);
    return lcl::integer(seq->count());
}

BUILTIN("deref")
{
    CHECK_ARGS_IS(1);
    ARG(lclAtom, atom);

    return atom->deref();
}

BUILTIN("dissoc")
{
    CHECK_ARGS_AT_LEAST(1);
    ARG(lclHash, hash);

    return hash->dissoc(argsBegin, argsEnd);
}

BUILTIN("empty?")
{
    CHECK_ARGS_IS(1);
    ARG(lclSequence, seq);

    return lcl::boolean(seq->isEmpty());
}

BUILTIN("eval")
{
    CHECK_ARGS_IS(1);
    return EVAL(*argsBegin, NULL);
}

BUILTIN("exit")
{
    CHECK_ARGS_IS(0);
    throw("application terminated by exit!");
}

BUILTIN("exp")
{
    BUILTIN_FUNCTION(exp);
}

BUILTIN("expt")
{
    CHECK_ARGS_IS(2);

    if (FLOAT_PTR)
    {
        ADD_FLOAT_VAL(*lhs)
        argsBegin++;
        if (FLOAT_PTR)
        {
            ADD_FLOAT_VAL(*rhs)
            return lcl::mdouble(pow(lhs->value(),
                                    rhs->value()));
        }
        else
        {
            ADD_INT_VAL(*rhs)
            return lcl::mdouble(pow(lhs->value(),
                                    double(rhs->value())));
        }
    }
    else
    {
        ADD_INT_VAL(*lhs)
        argsBegin++;
        if (FLOAT_PTR)
        {
            ADD_FLOAT_VAL(*rhs)
            return lcl::mdouble(pow(double(lhs->value()),
                                    rhs->value()));
        }
        else
        {
            ADD_INT_VAL(*rhs)
            auto result = static_cast<std::int64_t>(pow(double(lhs->value()),
                                    double(rhs->value())));
            return lcl::integer(result);
        }
    }
}

BUILTIN("first")
{
    CHECK_ARGS_IS(1);
    if (*argsBegin == lcl::nilValue()) {
        return lcl::nilValue();
    }
    ARG(lclSequence, seq);
    return seq->first();
}

BUILTIN("fix")
{
    CHECK_ARGS_IS(1);

    if (FLOAT_PTR)
    {
        ADD_FLOAT_VAL(*lhs)
        return lcl::integer(floor(lhs->value()));
    }
    else
    {
        ADD_INT_VAL(*lhs)
        return lcl::integer(lhs->value());
    }
}

BUILTIN("float")
{
    CHECK_ARGS_IS(1);

    if (FLOAT_PTR)
    {
        ADD_FLOAT_VAL(*lhs)
        return lcl::mdouble(lhs->value());
    }
    else
    {
        ADD_INT_VAL(*lhs)
        return lcl::mdouble(double(lhs->value()));
    }
}

BUILTIN("fn?")
{
    CHECK_ARGS_IS(1);
    lclValuePtr arg = *argsBegin++;

    // Lambdas are functions, unless they're macros.
    if (const lclLambda* lambda = DYNAMIC_CAST(lclLambda, arg)) {
        return lcl::boolean(!lambda->isMacro());
    }
    // Builtins are functions.
    return lcl::boolean(DYNAMIC_CAST(lclBuiltIn, arg));
}

BUILTIN("get")
{
    CHECK_ARGS_IS(2);
    if (*argsBegin == lcl::nilValue()) {
        return *argsBegin;
    }
    ARG(lclHash, hash);
    return hash->get(*argsBegin);
}

BUILTIN("getenv")
{
    CHECK_ARGS_IS(1);
    ARG(lclString, str);

    if (const char* env_p = std::getenv(str->value().c_str())) {
        String env = env_p;
        return lcl::string(env);
    }
    return lcl::nilValue();
}

BUILTIN("getfiled")
{
    CHECK_ARGS_IS(4);
    ARG(lclString, title);
    ARG(lclString, def);
    ARG(lclString, ext);
    ARG(lclInteger, flags);
    QString fileName;
    QString path = def->value().c_str();
    QString fileExt = "(*.";
    fileExt += ext->value().c_str();
    fileExt += ")";

    if (flags->value() & 1)
    {
        if (flags->value() & 4) {
            path += ".";
            path += ext->value().c_str();
        }

        QFileDialog saveFile;
        if (flags->value() & 32) {
            saveFile.setAcceptMode(QFileDialog::AcceptSave);
            saveFile.setOptions(QFileDialog::DontConfirmOverwrite);
        }
        fileName = saveFile.getSaveFileName(nullptr, title->value().c_str(), path, fileExt);
        //dialog.exec();
        if (fileName.size())
        {
            if (flags->value() & 4) {
                fileName += ".";
                fileName += ext->value().c_str();
            }
            return lcl::string(qUtf8Printable(fileName));
        }
    }

    if (flags->value() & 2)
    {
        if (!(flags->value() & 4)) {
            fileExt = "(*.dxf)";
        }
        if (!(flags->value() & 16)) {
            // pfad abschliesen
        }
        if (fileExt.size() == 0) {
            fileExt = "(*)";
        }
        fileName = QFileDialog::getOpenFileName(nullptr, title->value().c_str(), path, fileExt);
        if (fileName.size())
        {
            return lcl::string(qUtf8Printable(fileName));
        }
    }

    /*
     * not implemented yet
     *
     * 8 (bit 3) -- If this bit is set and bit 0 is not set, getfiled performs a library
     *  search for the file name entered. If it finds the file and its directory in the library search path,
     *  it strips the path and returns only the file name.
     *  (It does not strip the path name if it finds that a file of the same name is in a different directory.)
     *
     * 64  (bit 6) -- Do not transfer the remote file if the user specifies a URL.
     *
     * 128 (bit 7) -- Do not allow URLs at all.
     *
     */

    return lcl::nilValue();
}

BUILTIN("getint")
{
    if (CHECK_ARGS_AT_LEAST(0))
    {
        ARG(lclString, str);
        std::cout << str->value();
    }
    int x = 0;
    while(1)
    {
        std::cin >> x;
        if (!std::cin.fail()) break;
        std::cin.clear();
        std::cin.ignore(10000,'\n');
        std::cout << "Bad entry. Enter a NUMBER: ";
    }
    return lcl::integer(x);
}

BUILTIN("getreal")
{
    if (CHECK_ARGS_AT_LEAST(0))
    {
        ARG(lclString, str);
        std::cout << str->value();
    }
    float x = 0;
    while(1)
    {
        std::cin >> x;
        if (!std::cin.fail()) break;
        std::cin.clear();
        std::cin.ignore(10000,'\n');
        std::cout << "Bad entry. Enter a NUMBER: ";
    }
    return lcl::mdouble(x);
}


BUILTIN("getstring")
{
    int count = CHECK_ARGS_AT_LEAST(0);
    if (count == 2)
    {
        argsBegin++;
    }
    ARG(lclString, str);
    std::cout << str->value();
    String s = "";
    std::getline(std::cin >> std::ws, s);
    return lcl::string(s);
}

BUILTIN("hash-map")
{
    Q_UNUSED(name);
    return lcl::hash(argsBegin, argsEnd, true);
}

BUILTIN("keys")
{
    CHECK_ARGS_IS(1);
    ARG(lclHash, hash);
    return hash->keys();
}

BUILTIN("keyword")
{
    CHECK_ARGS_IS(1);
    const lclValuePtr arg = *argsBegin++;
    if (lclKeyword* s = DYNAMIC_CAST(lclKeyword, arg))
      return s;
    if (const lclString* s = DYNAMIC_CAST(lclString, arg))
      return lcl::keyword(":" + s->value());
    LCL_FAIL("keyword expects a keyword or string");
}

BUILTIN("last")
{
    CHECK_ARGS_IS(1);
    ARG(lclSequence, seq);

    LCL_CHECK(0 < seq->count(), "Index out of range");
    return seq->item(seq->count()-1);
}

BUILTIN("list")
{
    Q_UNUSED(name);
    return lcl::list(argsBegin, argsEnd);
}

BUILTIN("listp")
{
    CHECK_ARGS_IS(1);
    return (DYNAMIC_CAST(lclList, *argsBegin)) ? lcl::trueValue() : lcl::nilValue();
}

BUILTIN("log")
{
    BUILTIN_FUNCTION(log);
}

BUILTIN("logand")
{
    int argCount = CHECK_ARGS_AT_LEAST(0);
    int result = 0;
    [[maybe_unused]] double floatValue = 0;
    [[maybe_unused]] int64_t intValue = 0;

    if (argCount == 0) {
        return lcl::integer(0);
    }
    else {
        CHECK_IS_NUMBER(argsBegin->ptr());
        if (INT_PTR) {
            ADD_INT_VAL(*intVal);
            intValue = intVal->value();
            if (argCount == 1) {
                return lcl::integer(intValue);
            }
            else {
                result = intValue;
            }
        }
        else {
            ADD_FLOAT_VAL(*floatVal);
            floatValue = floatVal->value();
            if (argCount == 1) {
                return lcl::integer(int(floatValue));
            }
            else {
                result = int(floatValue);
            }
        }
    }
    for (auto it = argsBegin; it != argsEnd; it++) {
        CHECK_IS_NUMBER(it->ptr());
        if (it->ptr()->type() == LCLTYPE::INT) {
            const lclInteger* i = VALUE_CAST(lclInteger, *it);
            result = result & i->value();
        }
        else {
            const lclDouble* i = VALUE_CAST(lclDouble, *it);
            result = result & int(i->value());
        }
    }
    return lcl::integer(result);
}

BUILTIN("log10")
{
    CHECK_ARGS_IS(1);
    if (FLOAT_PTR) {
        ADD_FLOAT_VAL(*lhs)
        if (lhs->value() < 0) {
            return lcl::nilValue();
        }
        return lcl::mdouble(log10(lhs->value())); }
    else {
        ADD_INT_VAL(*lhs)
        if (lhs->value() < 0) {
            return lcl::nilValue();
        }
        return lcl::mdouble(log10(lhs->value())); }
}

BUILTIN("macro?")
{
    CHECK_ARGS_IS(1);

    // Macros are implemented as lambdas, with a special flag.
    const lclLambda* lambda = DYNAMIC_CAST(lclLambda, *argsBegin);
    return lcl::boolean((lambda != NULL) && lambda->isMacro());
}

BUILTIN("map")
{
    CHECK_ARGS_IS(2);
    lclValuePtr op = *argsBegin++; // this gets checked in APPLY
    ARG(lclSequence, source);

    const int length = source->count();
    lclValueVec* items = new lclValueVec(length);
    auto it = source->begin();
    for (int i = 0; i < length; i++) {
      items->at(i) = APPLY(op, it+i, it+i+1);
    }

    return  lcl::list(items);
}

BUILTIN("mapcar")
{
    int argCount = CHECK_ARGS_AT_LEAST(2);
    int i = 0;
    int count = 0;
    int offset = 0;
    int listCount = argCount-1;
    //int listCounts[listCount];
    std::vector<int> listCounts(static_cast<int>(listCount));
    const lclValuePtr op = EVAL(argsBegin++->ptr(), NULL);

    for (auto it = argsBegin++; it != argsEnd; it++) {
        const lclSequence* seq = VALUE_CAST(lclSequence, *it);
        listCounts[i++] = seq->count();
        offset += seq->count();
        if (count < seq->count()) {
            count = seq->count();
        }
    }

    //int newListCounts[count];
    //std::vector<int> newListCounts(static_cast<int>(count));
    std::vector<int> newListCounts(count);
    //lclValueVec* valItems[count]; // FIXME [-Wvla-cxx-extension]
    std::vector<lclValueVec *> valItems(count);
    lclValueVec* items = new lclValueVec(offset);
    lclValueVec* result = new lclValueVec(count);

    offset = 0;
    for (auto it = --argsBegin; it != argsEnd; ++it) {
        const lclSequence* seq = STATIC_CAST(lclSequence, *it);
        std::copy(seq->begin(), seq->end(), items->begin() + offset);
        offset += seq->count();
    }

    for (auto l = 0; l < count; l++) {
        newListCounts[l] = 0;
        valItems[l] = { new lclValueVec(listCount+1) };
        valItems[l]->at(0) = op;
    }

    offset = 0;
    for (auto n = 0; n < listCount; n++) {
        for (auto l = 0; l < count; l++) {
            if (listCounts[n] > l) {
                valItems[l]->at(n + 1) = items->at(offset + l);
                newListCounts[l] += 1;
            }
        }
        offset += listCounts[n];
    }

    for (auto l = 0; l < count; l++) {
        for (auto v = listCount - newListCounts[l]; v > 0; v--) {
            valItems[l]->erase(std::next(valItems[l]->begin()));
        }
        lclList* List = new lclList(valItems[l]);
        result->at(l) = EVAL(List, NULL);
    }
    return lcl::list(result);
}


BUILTIN("max")
{
    int count = CHECK_ARGS_AT_LEAST(1);
    bool hasFloat = ARGS_HAS_FLOAT;
    bool unset = true;
    [[maybe_unused]] double floatValue = 0;
    [[maybe_unused]] int64_t intValue = 0;

    if (count == 1)
    {
        if (hasFloat) {
            ADD_FLOAT_VAL(*floatVal);
            floatValue = floatVal->value();
            return lcl::mdouble(floatValue);
        }
        else {
            ADD_INT_VAL(*intVal);
            intValue = intVal->value();
            return lcl::integer(intValue);
        }
    }

    if (hasFloat) {
        do {
            if (FLOAT_PTR) {
                if (unset) {
                    unset = false;
                    ADD_FLOAT_VAL(*floatVal);
                    floatValue = floatVal->value();
                }
                else {
                    ADD_FLOAT_VAL(*floatVal)
                    if (floatVal->value() > floatValue) {
                        floatValue = floatVal->value();
                    }
                }
            }
            else {
                if (unset) {
                    unset = false;
                    ADD_INT_VAL(*intVal);
                    floatValue = double(intVal->value());
                }
                else {
                    ADD_INT_VAL(*intVal);
                    if (intVal->value() > floatValue)
                    {
                        floatValue = double(intVal->value());
                    }
                }
            }
            argsBegin++;
        } while (argsBegin != argsEnd);
        return lcl::mdouble(floatValue);
    } else {
        ADD_INT_VAL(*intVal);
        intValue = intVal->value();
        argsBegin++;
        do {
            ADD_INT_VAL(*intVal);
            if (intVal->value() > intValue)
            {
                intValue = intVal->value();
            }
            argsBegin++;
        } while (argsBegin != argsEnd);
        return lcl::integer(intValue);
    }
}

BUILTIN("member?")
{
    CHECK_ARGS_IS(2);
    lclValuePtr op = *argsBegin++;
    ARG(lclSequence, seq);

    const int length = seq->count();
    lclValueVec* items = new lclValueVec(length);
    std::copy(seq->begin(), seq->end(), items->begin());

    for (int i = 0; i < length; i++) {
        if (items->at(i)->print(true).compare(op->print(true)) == 0) {
            return lcl::trueValue();
        }
    }
    return lcl::nilValue();
}

BUILTIN("meta")
{
    CHECK_ARGS_IS(1);
    lclValuePtr obj = *argsBegin++;

    return obj->meta();
}

BUILTIN("min")
{
    int count = CHECK_ARGS_AT_LEAST(1);
    bool hasFloat = ARGS_HAS_FLOAT;
    bool unset = true;
    [[maybe_unused]] double floatValue = 0;
    [[maybe_unused]] int64_t intValue = 0;

    if (count == 1)
    {
        if (hasFloat) {
            ADD_FLOAT_VAL(*floatVal);
            floatValue = floatVal->value();
            return lcl::mdouble(floatValue);
        }
        else {
            ADD_INT_VAL(*intVal);
            intValue = intVal->value();
            return lcl::integer(intValue);
        }
    }

    if (hasFloat) {
        do {
            if (FLOAT_PTR) {
                if (unset) {
                    unset = false;
                    ADD_FLOAT_VAL(*floatVal);
                    floatValue = floatVal->value();
                }
                else {
                    ADD_FLOAT_VAL(*floatVal)
                    if (floatVal->value() < floatValue) {
                        floatValue = floatVal->value();
                    }
                }
            }
            else {
                if (unset) {
                    unset = false;
                    ADD_INT_VAL(*intVal);
                    floatValue = double(intVal->value());
                }
                else {
                    ADD_INT_VAL(*intVal);
                    if (intVal->value() < floatValue) {
                        floatValue = double(intVal->value());
                    }
                }
            }
            argsBegin++;
        } while (argsBegin != argsEnd);
        return lcl::mdouble(floatValue);
    } else {
        ADD_INT_VAL(*intVal);
        intValue = intVal->value();
        argsBegin++;
        do {
            ADD_INT_VAL(*intVal);
            if (intVal->value() < intValue) {
                intValue = intVal->value();
            }
            argsBegin++;
        } while (argsBegin != argsEnd);
        return lcl::integer(intValue);
    }
}

BUILTIN("nth")
{
    // twisted parameter for both LISPs!
    CHECK_ARGS_IS(2);
    int i;

    if(INT_PTR)
    {
        AG_INT(index);
        ARG(lclSequence, seq);
        i = index->value();
        LCL_CHECK(i >= 0 && i < seq->count(), "Index out of range");
        return seq->item(i);
    }
    else if(FLOAT_PTR) {
        // add dummy for error msg
        AG_INT(index);
        [[maybe_unused]] const String dummy = index->print(true);
        return lcl::nilValue();
    }
    else {
        ARG(lclSequence, seq);
        AG_INT(index);
        i = index->value();
        LCL_CHECK(i >= 0 && i < seq->count(), "Index out of range");
        return seq->item(i);
    }
}

BUILTIN("null")
{
    CHECK_ARGS_IS(1);
    if (NIL_PTR) {
        return lcl::trueValue();
    }
    return lcl::nilValue();
}

BUILTIN("number?")
{
    Q_UNUSED(name);
    Q_UNUSED(argsEnd);
    return lcl::boolean(DYNAMIC_CAST(lclInteger, *argsBegin) ||
            DYNAMIC_CAST(lclDouble, *argsBegin));
}

BUILTIN("numberp")
{
    Q_UNUSED(name);
    Q_UNUSED(argsEnd);
    return (DYNAMIC_CAST(lclInteger, *argsBegin) ||
            DYNAMIC_CAST(lclDouble, *argsBegin)) ? lcl::trueValue() : lcl::nilValue();
}

BUILTIN("open")
{
    CHECK_ARGS_IS(2);
    ARG(lclString, filename);
    ARG(lclString, m);
    const char mode = std::tolower(m->value().c_str()[0]);
    lclFile* pf = new lclFile(filename->value().c_str(), mode);

    return pf->open();
}

BUILTIN("polar")
{
    CHECK_ARGS_IS(3);
    double angle = 0;
    double dist = 0;
    double x = 0;
    double y = 0;
    [[maybe_unused]] double z = 0;

    ARG(lclSequence, seq);

    if (FLOAT_PTR) {
        ADD_FLOAT_VAL(*floatAngle)
        angle = floatAngle->value();
    }
    else
    {
        ADD_INT_VAL(*intAngle)
        angle = double(intAngle->value());
    }
    if (FLOAT_PTR) {
        ADD_FLOAT_VAL(*floatDist)
        dist = floatDist->value();
    }
    else
    {
        ADD_INT_VAL(*intDist)
        dist = double(intDist->value());
    }

    if(seq->count() == 2)
    {
        CHECK_IS_NUMBER(seq->item(0))
        if (seq->item(0)->type() == LCLTYPE::INT)
        {
            const lclInteger* intX = VALUE_CAST(lclInteger, seq->item(0));
            x = double(intX->value());
        }
        if (seq->item(0)->type() == LCLTYPE::REAL)
        {
            const lclDouble* floatX = VALUE_CAST(lclDouble, seq->item(0));
            x = floatX->value();
        }
        CHECK_IS_NUMBER(seq->item(1))
        if (seq->item(1)->type() == LCLTYPE::INT)
        {
            const lclInteger* intY = VALUE_CAST(lclInteger, seq->item(1));
            y = double(intY->value());
        }
        if (seq->item(1)->type() == LCLTYPE::REAL)
        {
            const lclDouble* floatY = VALUE_CAST(lclDouble, seq->item(1));
            y = floatY->value();
        }

        lclValueVec* items = new lclValueVec(2);
        items->at(0) = lcl::mdouble(x + dist * sin(angle));
        items->at(1) = lcl::mdouble(y + dist * cos(angle));
        return lcl::list(items);
    }

    if(seq->count() == 3)
    {
        if (seq->item(0)->type() == LCLTYPE::INT)
        {
            const lclInteger* intX = VALUE_CAST(lclInteger, seq->item(0));
            x = double(intX->value());
        }
        if (seq->item(0)->type() == LCLTYPE::REAL)
        {
            const lclDouble* floatX = VALUE_CAST(lclDouble, seq->item(0));
            x = floatX->value();
        }
        CHECK_IS_NUMBER(seq->item(1))
        if (seq->item(1)->type() == LCLTYPE::INT)
        {
            const lclInteger* intY = VALUE_CAST(lclInteger, seq->item(1));
            y = double(intY->value());
        }
        if (seq->item(1)->type() == LCLTYPE::REAL)
        {
            const lclDouble* floatY = VALUE_CAST(lclDouble, seq->item(1));
            y = floatY->value();
        }
        CHECK_IS_NUMBER(seq->item(2))
        if (seq->item(2)->type() == LCLTYPE::INT)
        {
            const lclInteger* intY = VALUE_CAST(lclInteger, seq->item(2));
            z = double(intY->value());
        }
        if (seq->item(2)->type() == LCLTYPE::REAL)
        {
            const lclDouble* floatY = VALUE_CAST(lclDouble, seq->item(2));
            z = floatY->value();
        }
        lclValueVec* items = new lclValueVec(3);
        items->at(0) = lcl::mdouble(x + dist * sin(angle));
        items->at(1) = lcl::mdouble(y + dist * cos(angle));
        items->at(2) = lcl::mdouble(z);
        return lcl::list(items);
    }
    return lcl::nilValue();
}

BUILTIN("pr-str")
{
    Q_UNUSED(name);
    return lcl::string(printValues(argsBegin, argsEnd, " ", true));
}

BUILTIN("prin1")
{
    Q_UNUSED(name);
    int args = CHECK_ARGS_BETWEEN(0, 2);
    if (args == 0) {
        std::cout << std::endl;
        return lcl::nullValue();
    }
    lclFile* pf = NULL;
    LCLTYPE type = argsBegin->ptr()->type();
    String boolean = argsBegin->ptr()->print(true);
    lclValueIter value = argsBegin;

    if (args == 2) {
        argsBegin++;
        if (argsBegin->ptr()->print(true).compare("nil") != 0) {
            pf = VALUE_CAST(lclFile, *argsBegin);
        }
    }
    if (boolean == "nil") {
        if (pf) {
            pf->writeLine("\"nil\"");
        }
        else {
            std::cout << "\"nil\"";
        }
            return lcl::nilValue();
    }
    if (boolean == "false") {
        if (pf) {
            pf->writeLine("\"false\"");
        }
        else {
            std::cout << "\"false\"";
        }
            return lcl::falseValue();
    }
    if (boolean == "true") {
        if (pf) {
            pf->writeLine("\"true\"");
        }
        else {
            std::cout << "\"true\"";
        }
            return lcl::trueValue();
    }
    if (boolean == "T") {
        if (pf) {
            pf->writeLine("\"T\"");
        }
        else {
            std::cout << "\"T\"";
        }
            return lcl::trueValue();
    }

    switch(type) {
        case LCLTYPE::FILE: {
            lclFile* f = VALUE_CAST(lclFile, *value);
            char filePtr[32];
            sprintf(filePtr, "%p", f->value());
            const String file = filePtr;
            if (pf) {
                pf->writeLine("\"" + file + "\"");
            }
            else {
                std::cout << "\"" << file << "\"";
            }
            return f;
        }
        case LCLTYPE::INT: {
            lclInteger* i = VALUE_CAST(lclInteger, *value);
            if (pf) {
                pf->writeLine("\"" + i->print(true) + "\"");
            }
            else {
                std::cout << "\"" << i->print(true) << "\"";
            }
            return i;
        }
        case LCLTYPE::LIST: {
            lclList* list = VALUE_CAST(lclList, *value);
            if (pf) {
                pf->writeLine("\"" + list->print(true) + "\"");
            }
            else {
                std::cout << "\"" << list->print(true) << "\"";
            }
            return list;
        }
        case LCLTYPE::MAP: {
            lclHash* hash = VALUE_CAST(lclHash, *value);
            if (pf) {
                pf->writeLine("\"" + hash->print(true) + "\"");
            }
            else {
                std::cout << "\"" << hash->print(true) << "\"";
            }
            return hash;
         }
        case LCLTYPE::REAL: {
            lclDouble* d = VALUE_CAST(lclDouble, *value);
            if (pf) {
                pf->writeLine("\"" + d->print(true) + "\"");
            }
            else {
                std::cout << "\"" << d->print(true) << "\"";
            }
            return d;
        }
        case LCLTYPE::STR: {
            lclString* str = VALUE_CAST(lclString, *value);
            if (pf) {
                pf->writeLine("\"" + str->value() + "\"");
            }
            else {
                std::cout << "\"" << str->value() << "\"";
            }
            return str;
        }
        case LCLTYPE::SYM: {
            lclSymbol* sym = VALUE_CAST(lclSymbol, *value);
            if (pf) {
                pf->writeLine("\"" + sym->value() + "\"");
            }
            else {
                std::cout << "\"" << sym->value() << "\"";
            }
            return sym;
        }
        case LCLTYPE::VEC: {
            lclVector* vector = VALUE_CAST(lclVector, *value);
            if (pf) {
                pf->writeLine("\"" + vector->print(true) + "\"");
            }
            else {
                std::cout << "\"" << vector->print(true) << "\"";
            }
            return vector;
        }
        case LCLTYPE::KEYW: {
            lclKeyword* keyword = VALUE_CAST(lclKeyword, *value);
            if (pf) {
                pf->writeLine("\"" + keyword->print(true) + "\"");
            }
            else {
                std::cout << "\"" << keyword->print(true) << "\"";
            }
            return keyword;
        }
        default: {
            if (pf) {
                pf->writeLine("\"nil\"");
            }
            else {
                std::cout << "\"nil\"";
            }
            return lcl::nilValue();
        }
    }

    if (pf) {
        pf->writeLine("\"nil\"");
    }
    else {
        std::cout << "\"nil\"";
    }
        return lcl::nilValue();
}

BUILTIN("princ")
{
    int args = CHECK_ARGS_BETWEEN(0, 2);
    if (args == 0) {
        std::cout << std::endl;
        fflush(stdout);
        return lcl::nullValue();
    }
    lclFile* pf = NULL;
    LCLTYPE type = argsBegin->ptr()->type();
    String boolean = argsBegin->ptr()->print(true);
    lclValueIter value = argsBegin;

    if (args == 2) {
        argsBegin++;
        if (argsBegin->ptr()->print(true).compare("nil") != 0) {
            pf = VALUE_CAST(lclFile, *argsBegin);
        }
    }
    if (boolean == "nil") {
        if (pf) {
            pf->writeLine("nil");
        }
        else {
            std::cout << "nil";
        }
            return lcl::nilValue();
    }
    if (boolean == "false") {
        if (pf) {
            pf->writeLine("false");
        }
        else {
            std::cout << "false";
        }
            return lcl::falseValue();
    }
    if (boolean == "true") {
        if (pf) {
            pf->writeLine("true");
        }
        else {
            std::cout << "true";
        }
            return lcl::trueValue();
    }
    if (boolean == "T") {
        if (pf) {
            pf->writeLine("T");
        }
        else {
            std::cout << "T";
        }
            return lcl::trueValue();
    }

    switch(type) {
        case LCLTYPE::FILE: {
            lclFile* f = VALUE_CAST(lclFile, *value);
            char filePtr[32];
            sprintf(filePtr, "%p", f->value());
            const String file = filePtr;
            if (pf) {
                pf->writeLine(file);
            }
            else {
                std::cout << file;
            }
            return f;
        }
        case LCLTYPE::INT: {
            lclInteger* i = VALUE_CAST(lclInteger, *value);
            if (pf) {
                pf->writeLine(i->print(true));
            }
            else {
                std::cout << i->print(true);
            }
            return i;
        }
        case LCLTYPE::LIST: {
            lclList* list = VALUE_CAST(lclList, *value);
            if (pf) {
                pf->writeLine(list->print(true));
            }
            else {
                std::cout << list->print(true);
            }
            return list;
        }
        case LCLTYPE::MAP: {
            lclHash* hash = VALUE_CAST(lclHash, *value);
            if (pf) {
                pf->writeLine(hash->print(true));
            }
            else {
                std::cout << hash->print(true);
            }
            return hash;
         }
        case LCLTYPE::REAL: {
            lclDouble* d = VALUE_CAST(lclDouble, *value);
            if (pf) {
                pf->writeLine(d->print(true));
            }
            else {
                std::cout << d->print(true);
            }
            return d;
        }
        case LCLTYPE::STR: {
            lclString* str = VALUE_CAST(lclString, *value);
            if (pf) {
                pf->writeLine(str->value());
            }
            else {
                std::cout << str->value();
                fflush(stdout);
            }
            return str;
        }
        case LCLTYPE::SYM: {
            lclSymbol* sym = VALUE_CAST(lclSymbol, *value);
            if (pf) {
                pf->writeLine(sym->value());
            }
            else {
                std::cout << sym->value();
            }
            return sym;
        }
        case LCLTYPE::VEC: {
            lclVector* vector = VALUE_CAST(lclVector, *value);
            if (pf) {
                pf->writeLine(vector->print(true));
            }
            else {
                std::cout << vector->print(true);
            }
            return vector;
        }
        case LCLTYPE::KEYW: {
            lclKeyword* keyword = VALUE_CAST(lclKeyword, *value);
            if (pf) {
                pf->writeLine(keyword->print(true));
            }
            else {
                std::cout << keyword->print(true);
            }
            return keyword;
        }
        default: {
            if (pf) {
                pf->writeLine("nil");
            }
            else {
                std::cout << "nil";
            }
            return lcl::nilValue();
        }
        fflush(stdout);
    }

    if (pf) {
        pf->writeLine("nil");
    }
    else {
        std::cout << "nil";
    }
        return lcl::nilValue();
}

BUILTIN("print")
{
    int args = CHECK_ARGS_BETWEEN(0, 2);
    if (args == 0) {
        std::cout << std::endl;
        return lcl::nullValue();
    }
    lclFile* pf = NULL;
    LCLTYPE type = argsBegin->ptr()->type();
    String boolean = argsBegin->ptr()->print(true);
    lclValueIter value = argsBegin;

    if (args == 2) {
        argsBegin++;
        if (argsBegin->ptr()->print(true).compare("nil") != 0) {
            pf = VALUE_CAST(lclFile, *argsBegin);
        }
    }
    if (boolean == "nil") {
        if (pf) {
            pf->writeLine("\n\"nil\" ");
        }
        else {
            std::cout << "\n\"nil\" ";
        }
            return lcl::nilValue();
    }
    if (boolean == "false") {
        if (pf) {
            pf->writeLine("\n\"false\" ");
        }
        else {
            std::cout << "\n\"false\" ";
        }
            return lcl::falseValue();
    }
    if (boolean == "true") {
        if (pf) {
            pf->writeLine("\n\"true\" ");
        }
        else {
            std::cout << "\n\"true\" ";
        }
            return lcl::trueValue();
    }
    if (boolean == "T") {
        if (pf) {
            pf->writeLine("\n\"T\" ");
        }
        else {
            std::cout << "\n\"T\" ";
        }
            return lcl::trueValue();
    }

    switch(type) {
        case LCLTYPE::FILE: {
            lclFile* f = VALUE_CAST(lclFile, *value);
            char filePtr[32];
            sprintf(filePtr, "%p", f->value());
            const String file = filePtr;
            if (pf) {
                pf->writeLine("\n\"" + file + "\" ");
            }
            else {
                std::cout << "\n\"" << file << "\" ";
            }
            return f;
        }
        case LCLTYPE::INT: {
            lclInteger* i = VALUE_CAST(lclInteger, *value);
            if (pf) {
                pf->writeLine("\n\"" + i->print(true) + "\" ");
            }
            else {
                std::cout << "\n\"" << i->print(true) << "\" ";
            }
            return i;
        }
        case LCLTYPE::LIST: {
            lclList* list = VALUE_CAST(lclList, *value);
            if (pf) {
                pf->writeLine("\n\"" + list->print(true) + "\" ");
            }
            else {
                std::cout << "\n\"" << list->print(true) << "\" ";
            }
            return list;
        }
        case LCLTYPE::MAP: {
            lclHash* hash = VALUE_CAST(lclHash, *value);
            if (pf) {
                pf->writeLine("\n\"" + hash->print(true) + "\" ");
            }
            else {
                std::cout << "\n\"" << hash->print(true) << "\" ";
            }
            return hash;
         }
        case LCLTYPE::REAL: {
            lclDouble* d = VALUE_CAST(lclDouble, *value);
            if (pf) {
                pf->writeLine("\n\"" + d->print(true) + "\" ");
            }
            else {
                std::cout << "\n\"" << d->print(true) << "\" ";
            }
            return d;
        }
        case LCLTYPE::STR: {
            lclString* str = VALUE_CAST(lclString, *value);
            if (pf) {
                pf->writeLine("\n\"" + str->value() + "\" ");
            }
            else {
                std::cout << "\n\"" << str->value() << "\" ";
            }
            return str;
        }
        case LCLTYPE::SYM: {
            lclSymbol* sym = VALUE_CAST(lclSymbol, *value);
            if (pf) {
                pf->writeLine("\n\"" + sym->value() + "\" ");
            }
            else {
                std::cout << "\n\"" << sym->value() << "\" ";
            }
            return sym;
        }
        case LCLTYPE::VEC: {
            lclVector* vector = VALUE_CAST(lclVector, *value);
            if (pf) {
                pf->writeLine("\n\"" + vector->print(true) + "\" ");
            }
            else {
                std::cout << "\n\"" << vector->print(true) << "\" ";
            }
            return vector;
        }
        case LCLTYPE::KEYW: {
            lclKeyword* keyword = VALUE_CAST(lclKeyword, *value);
            if (pf) {
                pf->writeLine("\n\"" + keyword->print(true) + "\" ");
            }
            else {
                std::cout << "\n\"" << keyword->print(true) << "\" ";
            }
            return keyword;
        }
        default: {
            if (pf) {
                pf->writeLine("\n\"nil\" ");
            }
            else {
                std::cout << "\n\"nil\" ";
            }
            return lcl::nilValue();
        }
    }

    if (pf) {
        pf->writeLine("\n\"nil\" ");
    }
    else {
        std::cout << "\n\"nil\" ";
    }
        return lcl::nilValue();
}

BUILTIN("println")
{
    Q_UNUSED(name);
    std::cout << printValues(argsBegin, argsEnd, " ", false) << std::endl;
    return lcl::nilValue();
}

BUILTIN("prn")
{
    Q_UNUSED(name);
    std::cout << printValues(argsBegin, argsEnd, " ", true) << std::endl;
    return lcl::nilValue();
}

BUILTIN("prompt")
{
    Q_UNUSED(name);
    Q_UNUSED(argsEnd);
    ARG(lclString, str);
    std::cout << str->value();
    return lcl::nilValue();
}

BUILTIN("py-eval-float")
{
    Q_UNUSED(name);
    Q_UNUSED(argsEnd);
    ARG(lclString, com);
    double result;
    int err = RS_PYTHON->evalFloat(com->value().c_str(), result);
    if (err == 0)
    {
        return lcl::mdouble(result);
    }
    LCL_FAIL("'evalFloat' python failed");
}

BUILTIN("py-eval-integer")
{
    Q_UNUSED(name);
    Q_UNUSED(argsEnd);
    ARG(lclString, com);
    int result;
    int err = RS_PYTHON->evalInteger(com->value().c_str(), result);
    if (err == 0)
    {
        return lcl::integer(result);
    }
    LCL_FAIL("'evalInteger' python failed");
}

BUILTIN("py-eval-vector")
{
    Q_UNUSED(name);
    Q_UNUSED(argsEnd);
    ARG(lclString, com);
    v3_t result;
    int err = RS_PYTHON->evalVector(com->value().c_str(), result);

    if (err == 0)
    {
        lclValueVec* items = new lclValueVec(3);
        items->at(0) = lcl::mdouble(result.x);
        items->at(1) = lcl::mdouble(result.y);
        items->at(2) = lcl::mdouble(result.z);
        return lcl::list(items);
    }
    LCL_FAIL("'evalVector' python failed");
}

BUILTIN("py-eval-string")
{
    Q_UNUSED(name);
    Q_UNUSED(argsEnd);
    ARG(lclString, com);
    QString result;
    int err = RS_PYTHON->evalString(com->value().c_str(), result);
    if (err == 0)
    {
        return lcl::string(result.toStdString());
    }
    LCL_FAIL("'evalString' python failed");
}

BUILTIN("py-simple-string")
{
    Q_UNUSED(name);
    Q_UNUSED(argsEnd);
    ARG(lclString, com);
    return lcl::integer(RS_PYTHON->runString(com->value().c_str()));
}

BUILTIN("py-simple-file")
{
    Q_UNUSED(name);
    Q_UNUSED(argsEnd);
    ARG(lclString, com);
    return lcl::integer(RS_PYTHON->runFile(com->value().c_str()));
}

BUILTIN("read-string")
{
    Q_UNUSED(argsEnd);
    CHECK_ARGS_IS(1);
    ARG(lclString, str);

    return readStr(str->value());
}

BUILTIN("read-line")
{
    Q_UNUSED(argsBegin);
    Q_UNUSED(argsEnd);
    if (!CHECK_ARGS_AT_LEAST(0))
    {
        String str;
        std::getline(std::cin, str);
        return lcl::string(str);
    }
    ARG(lclFile, pf);

    return pf->readLine();
}

BUILTIN("read-char")
{
    Q_UNUSED(argsBegin);
    Q_UNUSED(argsEnd);
    if (!CHECK_ARGS_AT_LEAST(0))
    {
        unsigned char c = 0;
        while (! kbhit())
        {
            fflush(stdout);
            c=getchar();
            break;
        }
        std::cout << std::endl;
        return lcl::integer(int(c));
    }
    ARG(lclFile, pf);

    return pf->readChar();
}
#if 0
BUILTIN("readline")
{
    CHECK_ARGS_IS(1);
    ARG(lclString, str);

    return readline(str->value());
}
#endif
BUILTIN("rem")
{
    CHECK_ARGS_AT_LEAST(2);
    if (ARGS_HAS_FLOAT) {
        [[maybe_unused]] double floatValue = 0;
        if (FLOAT_PTR) {
            ADD_FLOAT_VAL(*floatVal)
            floatValue = floatValue + floatVal->value();
            LCL_CHECK(floatVal->value() != 0.0, "Division by zero");
    }
    else {
        ADD_INT_VAL(*intVal)
        floatValue = floatValue + double(intVal->value());
        LCL_CHECK(intVal->value() != 0, "Division by zero");
    }
    argsBegin++;
    do {
        if (FLOAT_PTR) {
            ADD_FLOAT_VAL(*floatVal)
            floatValue = fmod(floatValue, floatVal->value());
            LCL_CHECK(floatVal->value() != 0.0, "Division by zero");
        }
        else {
            ADD_INT_VAL(*intVal)
            floatValue = fmod(floatValue, double(intVal->value()));
            LCL_CHECK(intVal->value() != 0, "Division by zero");
        }
        argsBegin++;
    } while (argsBegin != argsEnd);
    return lcl::mdouble(floatValue);
    } else {
        [[maybe_unused]] int64_t intValue = 0;
        ADD_INT_VAL(*intVal) // +
        intValue = intValue + intVal->value();
        LCL_CHECK(intVal->value() != 0, "Division by zero");
        argsBegin++;
        do {
            ADD_INT_VAL(*intVal)
            intValue = int(fmod(double(intValue), double(intVal->value())));
            LCL_CHECK(intVal->value() != 0, "Division by zero");
            argsBegin++;
        } while (argsBegin != argsEnd);
        return lcl::integer(intValue);
    }
}

BUILTIN("reset!")
{
    CHECK_ARGS_IS(2);
    ARG(lclAtom, atom);
    return atom->reset(*argsBegin);
}

BUILTIN("rest")
{
    Q_UNUSED(argsEnd);
    CHECK_ARGS_IS(1);
    if (*argsBegin == lcl::nilValue()) {
        return lcl::list(new lclValueVec(0));
    }
    ARG(lclSequence, seq);
    return seq->rest();
}

BUILTIN("reverse")
{
    Q_UNUSED(argsEnd);
    CHECK_ARGS_IS(1);
    if (*argsBegin == lcl::nilValue()) {
        return lcl::list(new lclValueVec(0));
    }
    ARG(lclSequence, seq);
    return seq->reverse(seq->begin(), seq->end());
}

BUILTIN("seq")
{
    CHECK_ARGS_IS(1);
    lclValuePtr arg = *argsBegin++;
    if (arg == lcl::nilValue()) {
        return lcl::nilValue();
    }
    if (const lclSequence* seq = DYNAMIC_CAST(lclSequence, arg)) {
        return seq->isEmpty() ? lcl::nilValue()
                              : lcl::list(seq->begin(), seq->end());
    }
    if (const lclString* strVal = DYNAMIC_CAST(lclString, arg)) {
        const String str = strVal->value();
        int length = str.length();
        if (length == 0)
            return lcl::nilValue();

        lclValueVec* items = new lclValueVec(length);
        for (int i = 0; i < length; i++) {
            (*items)[i] = lcl::string(str.substr(i, 1));
        }
        return lcl::list(items);
    }
    LCL_FAIL("'%s' is not a string or sequence", arg->print(true).c_str());
}

BUILTIN("sin")
{
    BUILTIN_FUNCTION(sin);
}

BUILTIN("slurp")
{
    Q_UNUSED(argsEnd);
    CHECK_ARGS_IS(1);
    ARG(lclString, filename);

    std::ios_base::openmode openmode =
        std::ios::ate | std::ios::in | std::ios::binary;
    std::ifstream file(filename->value().c_str(), openmode);
    LCL_CHECK(!file.fail(), "Cannot open %s", filename->value().c_str());

    String data;
    data.reserve(file.tellg());
    file.seekg(0, std::ios::beg);
    data.append(std::istreambuf_iterator<char>(file.rdbuf()),
                std::istreambuf_iterator<char>());

    return lcl::string(data);
}

BUILTIN("sqrt")
{
    BUILTIN_FUNCTION(sqrt);
}

BUILTIN("startapp")
{
    int count = CHECK_ARGS_AT_LEAST(1);
    ARG(lclString, com);
    String command = com->value();

    if (count > 1)
    {
        ARG(lclString, para);
        command += " ";
        command += para->value();
    }

    if (system(command.c_str()))
    {
        return lcl::nilValue();
    }
    return lcl::integer(count);
}

BUILTIN("str")
{
    Q_UNUSED(name);
    return lcl::string(printValues(argsBegin, argsEnd, "", false));
}

BUILTIN("strcase")
{
    int count = CHECK_ARGS_AT_LEAST(1);
    ARG(lclString, str);
    String trans = str->value();

    if (count > 1)
    {
        ARG(lclConstant, boolVal);
        if (boolVal->isTrue())
        {
            std::transform(trans.begin(), trans.end(), trans.begin(),
                   [](unsigned char c){ return std::tolower(c); });
            return lcl::string(trans);
        }
    }

    std::transform(trans.begin(), trans.end(), trans.begin(),
                   [](unsigned char c){ return std::toupper(c); });

    return lcl::string(trans);
}

BUILTIN("strlen")
{
    Q_UNUSED(name);
    return lcl::integer(countValues(argsBegin, argsEnd));
}

BUILTIN("substr")
{
    int count = CHECK_ARGS_AT_LEAST(2);
    ARG(lclString, s);
    AG_INT(start);
    int startPos = (int)start->value();

    if (s)
    {
        String bla = s->value();
        if (startPos > (int)bla.size()+1) {
            startPos = (int)bla.size()+1;
        }

        if (count > 2)
        {
            AG_INT(size);
            return lcl::string(bla.substr(startPos-1, size->value()));
        }
        else
        {
                return lcl::string(bla.substr(startPos-1, bla.size()));
        }
    }

    return lcl::string(String(""));
}

BUILTIN("subst")
{
    CHECK_ARGS_IS(3);
    lclValuePtr oldSym = *argsBegin++;
    lclValuePtr newSym = *argsBegin++;
    ARG(lclSequence, seq);

    const int length = seq->count();
    lclValueVec* items = new lclValueVec(length);
    std::copy(seq->begin(), seq->end(), items->begin());

    for (int i = 0; i < length; i++) {
        if (items->at(i)->print(true).compare(oldSym->print(true)) == 0) {
            items->at(i) = newSym;
            return lcl::list(items);
        }
    }
    return lcl::nilValue();
}

BUILTIN("swap!")
{
    CHECK_ARGS_AT_LEAST(2);
    ARG(lclAtom, atom);

    lclValuePtr op = *argsBegin++; // this gets checked in APPLY

    lclValueVec args(1 + argsEnd - argsBegin);
    args[0] = atom->deref();
    std::copy(argsBegin, argsEnd, args.begin() + 1);

    lclValuePtr value = APPLY(op, args.begin(), args.end());
    return atom->reset(value);
}

BUILTIN("symbol")
{
    CHECK_ARGS_IS(1);
    ARG(lclString, token);
    return lcl::symbol(token->value());
}

BUILTIN("tan")
{
    BUILTIN_FUNCTION(tan);
}

BUILTIN("terpri")
{
    Q_UNUSED(name);
    Q_UNUSED(argsBegin);
    Q_UNUSED(argsEnd);
    std::cout << std::endl;
    return lcl::nilValue();
}

BUILTIN("throw")
{
    Q_UNUSED(argsEnd);
    CHECK_ARGS_IS(1);
    throw *argsBegin;
}

BUILTIN("time-ms")
{
    CHECK_ARGS_IS(0);

    using namespace std::chrono;
    milliseconds ms = duration_cast<milliseconds>(
        high_resolution_clock::now().time_since_epoch()
    );

    return lcl::integer(ms.count());
}

BUILTIN("type?")
{
    CHECK_ARGS_IS(1);

    if (argsBegin->ptr()->print(true).compare("nil") == 0) {
        return lcl::nilValue();
    }

    return lcl::type(argsBegin->ptr()->type());
}

BUILTIN("vals")
{
    CHECK_ARGS_IS(1);
    ARG(lclHash, hash);
    return hash->values();
}

BUILTIN("vec")
{
    CHECK_ARGS_IS(1);
    ARG(lclSequence, s);
    return lcl::vector(s->begin(), s->end());
}

BUILTIN("vector")
{
    Q_UNUSED(name);
    return lcl::vector(argsBegin, argsEnd);
}

BUILTIN("ver")
{
    Q_UNUSED(name);
    Q_UNUSED(argsBegin);
    Q_UNUSED(argsEnd);
    return lcl::string(LISP_VERSION);
}

BUILTIN("vl-consp")
{
    CHECK_ARGS_IS(1);
    ARG(lclSequence, s);

    if(s->isDotted()) {
        return lcl::trueValue();
    }
    return lcl::nilValue();
}

BUILTIN("vl-directory-files")
{
    int count = CHECK_ARGS_AT_LEAST(0);
    int len = 0;
    String path = "./";
    lclValueVec* items;
    std::vector<std::filesystem::path> sorted_by_name;

    if (count > 0) {
        ARG(lclString, directory);
        path = directory->value();
        if (!std::filesystem::exists(path.c_str())) {
            return lcl::nilValue();
        }
        if (count > 1 && (NIL_PTR || INT_PTR) && !(count == 2 && (NIL_PTR || INT_PTR))) {
            if (NIL_PTR) {
                argsBegin++;
            }
            // pattern + dirs
            AG_INT(directories);
            switch(directories->value())
            {
                case -1:
                    for (const auto & entry : std::filesystem::directory_iterator(path.c_str())) {
                        if (std::filesystem::is_directory(entry.path()))
                        {
                            sorted_by_name.push_back(entry.path().filename());
                            len++;
                        }
                    }
                    break;
                case 0:
                    for (const auto & entry : std::filesystem::directory_iterator(path.c_str())) {
                        sorted_by_name.push_back(entry.path());
                        len++;
                    }
                    break;
                case 1:
                    for (const auto & entry : std::filesystem::directory_iterator(path.c_str())) {
                        if (!std::filesystem::is_directory(entry.path()))
                        {
                            sorted_by_name.push_back(entry.path().filename());
                            len++;
                        }
                    }
                    break;
                default: {}
            }
        }
        else if (count > 1 && !(count == 2 && (NIL_PTR || INT_PTR))) {
            ARG(lclString, pattern);
            int dir = 3;
            if (count > 2) {
                AG_INT(directories2);
                dir = directories2->value();
                if (dir > 1 || dir < -1) {
                    dir = 0;
                }
            }
            // pattern
            bool hasExt = false;
            bool hasName = false;
            String pat = pattern->value();
            int asterix = (int) pat.find_last_of("*");
            if (asterix != -1 && (int) pat.size() >= asterix) {
                hasExt = true;
            }
            if (asterix != -1 && (int) pat.size() >= asterix && pat.size() > pat.substr(asterix+1).size() ) {
                hasName = true;
            }
            for (const auto & entry : std::filesystem::directory_iterator(path.c_str())) {
                if (!std::filesystem::is_directory(entry.path()) &&
                    hasExt &&
                    hasName &&
                    (dir == 3 || dir == 1)) {
                    if ((int)entry.path().filename().string().find(pat.substr(asterix+1)) != -1 &&
                        (int)entry.path().filename().string().find(pat.substr(0, asterix)) != -1) {
                        sorted_by_name.push_back(entry.path().filename());
                        len++;
                    }
                }
                if (!std::filesystem::is_directory(entry.path()) && !hasExt &&
                    (int)entry.path().filename().string().find(pat) != -1 && (dir == 3 || dir == 1)) {
                    sorted_by_name.push_back(entry.path().filename());
                    len++;
                }
                if (std::filesystem::is_directory(entry.path()) && !hasExt &&
                    (int)entry.path().filename().string().find(pat) != -1  && dir == -1) {
                    sorted_by_name.push_back(entry.path().filename());
                    len++;
                }
                if ((int)entry.path().string().find(pat) != -1 && dir == 0) {
                    sorted_by_name.push_back(entry.path());
                    len++;
                }
            }
        }
        else {
            // directory
            for (const auto & entry : std::filesystem::directory_iterator(path.c_str())) {
                if (!std::filesystem::is_directory(entry.path()))
                {
                    sorted_by_name.push_back(entry.path().filename());
                    len++;
                }
            }
        }
    }
    else {
        // current directory
        for (const auto & entry : std::filesystem::directory_iterator(path.c_str())) {
            if (!std::filesystem::is_directory(entry.path()))
            {
                sorted_by_name.push_back(entry.path().filename());
                len++;
            }
        }
    }
    std::sort(sorted_by_name.begin(), sorted_by_name.end(), compareNat);
    items = new lclValueVec(len);
    len = 0;
    for (const auto & filename : sorted_by_name) {
        items->at(len) = lcl::string(filename);
        len++;
    }
    return items->size() ? lcl::list(items) : lcl::nilValue();
}

BUILTIN("vl-file-copy")
{
    int count = CHECK_ARGS_AT_LEAST(2);
    ARG(lclString, source);
    ARG(lclString, dest);

    if (count == 3 && argsBegin->ptr()->isTrue()) {

        std::ofstream of;
        std::ios_base::openmode openmode =
            std::ios::ate | std::ios::in | std::ios::binary;
        std::ifstream file(source->value().c_str(), openmode);

        if (file.fail()) {
            return lcl::nilValue();
        }

        String data;
        data.reserve(file.tellg());
        file.seekg(0, std::ios::beg);
        data.append(std::istreambuf_iterator<char>(file.rdbuf()), std::istreambuf_iterator<char>());

        of.open(dest->value(), std::ios::app);
        if (!of) {
            return lcl::nilValue();
        }
        else {
            of << data;
            of.close();
            return lcl::integer(sizeof source->value());
        }
    }

    std::error_code err;
    std::filesystem::copy(source->value(), dest->value(), std::filesystem::copy_options::update_existing, err);
    if (err) {
        return lcl::nilValue();
    }
    return lcl::integer(sizeof source->value());
}

BUILTIN("vl-file-delete")
{
    CHECK_ARGS_IS(1);
    ARG(lclString, path);
    if (!std::filesystem::exists(path->value().c_str())) {
        return lcl::nilValue();
    }
    if (std::filesystem::remove(path->value().c_str()))
    {
        return lcl::trueValue();
    }
    return lcl::nilValue();
}

BUILTIN("vl-file-directory-p")
{
    CHECK_ARGS_IS(1);
    ARG(lclString, path);
    const std::filesystem::directory_entry dir(path->value().c_str());
    if (std::filesystem::exists(path->value().c_str()) &&
        dir.is_directory()) {
        return lcl::trueValue();
    }
    return lcl::nilValue();
}

BUILTIN("vl-file-rename")
{
    CHECK_ARGS_IS(2);
    ARG(lclString, path);
    ARG(lclString, newName);
    if (!std::filesystem::exists(path->value().c_str())) {
        return lcl::nilValue();
    }
    std::error_code err;
    std::filesystem::rename(path->value().c_str(), newName->value().c_str(), err);
    if (err) {
        return lcl::nilValue();
    }
    return lcl::trueValue();
}

BUILTIN("vl-file-size")
{
    CHECK_ARGS_IS(1);
    ARG(lclString, path);
    if (!std::filesystem::exists(path->value().c_str())) {
        return lcl::nilValue();
    }
    if (!std::filesystem::is_directory(path->value().c_str())) {
        return lcl::string("0");
    }
    try {
        [[maybe_unused]] auto size = std::filesystem::file_size(path->value().c_str());
        char str[50];
        sprintf(str, "%ld", size);
        return lcl::string(str);
    }
    catch (std::filesystem::filesystem_error&) {}
    return lcl::nilValue();
}

BUILTIN("vl-file-systime")
{
    CHECK_ARGS_IS(1);
    ARG(lclString, path);
    if (!std::filesystem::exists(path->value().c_str())) {
        return lcl::nilValue();
    }

    std::filesystem::file_time_type ftime = std::filesystem::last_write_time(path->value().c_str());
    std::time_t cftime = to_time_t(ftime); // assuming system_clock

    char buffer[64];
    int J,M,W,D,h,m,s;

    if (strftime(buffer, sizeof buffer, "%Y %m %w %e %I %M %S", std::localtime(&cftime))) {
        sscanf (buffer,"%d %d %d %d %d %d %d",&J,&M,&W,&D,&h,&m,&s);

        lclValueVec* items = new lclValueVec(6);
        items->at(0) = new lclInteger(J);
        items->at(1) = new lclInteger(M);
        items->at(2) = new lclInteger(W);
        items->at(3) = new lclInteger(D);
        items->at(4) = new lclInteger(m);
        items->at(5) = new lclInteger(s);
        return lcl::list(items);
    }
    return lcl::nilValue();
}

BUILTIN("vl-filename-base")
{
    CHECK_ARGS_IS(1);
    ARG(lclString, path);

    const std::filesystem::path p(path->value());
    return lcl::string(p.stem());
}

BUILTIN("vl-filename-directory")
{
    CHECK_ARGS_IS(1);
    ARG(lclString, path);

    const std::filesystem::path p(path->value());
    if (!p.has_extension()) {
        return lcl::string(path->value());
    }

    const auto directory = std::filesystem::path{ p }.parent_path().string();
    return lcl::string(directory);
}

BUILTIN("vl-filename-extension")
{
    CHECK_ARGS_IS(1);
    ARG(lclString, path);

    const std::filesystem::path p(path->value());
    if (!p.has_extension()) {
        return lcl::nilValue();
    }

    return lcl::string(p.extension());
}

BUILTIN("vl-filename-mktemp")
{
    int count = CHECK_ARGS_AT_LEAST(0);
    char num[4];
    sprintf(num, "%03x", ++tmpFileCount);
    String filename = "tmpfile_";
    String path;
    std::filesystem::path p(std::filesystem::temp_directory_path());
     std::filesystem::path d("");

    filename +=  + num;
    path = p / filename;

    if (count > 0) {
        ARG(lclString, pattern);
        p = pattern->value().c_str();
        filename = p.stem();
        filename +=  + num;
        if (!p.has_root_path()) {
            path = std::filesystem::temp_directory_path() / d;
        }
        else {
            path = p.root_path() / p.relative_path().remove_filename();
        }
        if (p.has_extension()) {
            filename += p.extension();
        }
        path += filename;
    }
    if (count > 1) {
        ARG(lclString, directory);
        path = directory->value() / d;
        path += filename;
    }
    if (count == 3) {
        ARG(lclString, extension);
        path += extension->value();
    }
    return lcl::string(path);
}

BUILTIN("vl-mkdir")
{
    CHECK_ARGS_IS(1);
    ARG(lclString, dir);

    if(std::filesystem::create_directory(dir->value())) {
        return lcl::trueValue();
    }
    return lcl::nilValue();
}

BUILTIN("vl-position")
{
    CHECK_ARGS_IS(2);
    lclValuePtr op = *argsBegin++; // this gets checked in APPLY

    const lclSequence* seq = VALUE_CAST(lclSequence, *(argsBegin));
    for (int i = 0; i < seq->count(); i++) {
        if(seq->item(i)->print(true).compare(op->print(true)) == 0) {
            return lcl::integer(i);
        }
    }
    return lcl::nilValue();
}

BUILTIN("symbol")
{
    CHECK_ARGS_IS(1);
    if(argsBegin->ptr()->type() == LCLTYPE::SYM) {
        return lcl::trueValue();
    }
    return lcl::nilValue();
}

BUILTIN("wcmatch")
{
    CHECK_ARGS_IS(2);
    ARG(lclString, str);
    ARG(lclString, p);
    std::vector<String> StringList;
    String del = ",";
    String pat = p->value();
    auto pos = pat.find(del);

    while (pos != String::npos) {
        StringList.push_back(pat.substr(0, pos));
        pat.erase(0, pos + del.length());
        pos = pat.find(del);
    }
    StringList.push_back(pat);
    for (auto &it : StringList) {
        String pattern = it;
        String expr = "";
        bool exclude = false;
        bool open_br = false;
        for (auto &ch : it) {
            switch (ch) {
                case '#':
                    expr += "(\\d)";
                    break;
                case '@':
                    expr += "[A-Za-zÀ-ȕ]";
                    break;
                case ' ':
                    expr += "[ ]+";
                    break;
                case '.':
                    expr += "([^(A-Za-z0-9 )]{1,})";
                    break;
                case '*':
                    expr += "(.*)";
                    break;
                case '?':
                    expr += "[A-Za-zÀ-ȕ0-9_ ]";
                    break;
                case '~': {
                    if (open_br) {
                        expr += "^";
                    } else {
                        expr += "[^";
                    exclude = true;
                    }
                    break;
                }
                case '[':
                    expr += "[";
                    open_br = true;
                    break;
                case ']': {
                    expr += "]{1}";
                    open_br = false;
                    break;
                }
                case '`':
                    expr += "//";
                    break;
                default: {
                    expr += ch;
                }
            }
        }
        if (exclude) {
            expr += "]*";
        }
        std::regex e (expr);
        if (std::regex_match (str->value(),e)) {
            return lcl::trueValue();
        }
    }
    return lcl::nilValue();
}

BUILTIN("with-meta")
{
    CHECK_ARGS_IS(2);
    lclValuePtr obj  = *argsBegin++;
    lclValuePtr meta = *argsBegin++;
    return obj->withMeta(meta);
}

BUILTIN("write-line")
{
    int count = CHECK_ARGS_AT_LEAST(1);
    //multi
    ARG(lclString, str);

    if (count == 1)
    {
        return lcl::string(str->value());
    }

    ARG(lclFile, pf);

    return pf->writeLine(str->value());
}

BUILTIN("write-char")
{
    int count = CHECK_ARGS_AT_LEAST(1);
    AG_INT(c);

    std::cout << itoa64(c->value()) << std::endl;

    if (count == 1)
    {
        return lcl::integer(c->value());
    }

    ARG(lclFile, pf);

    return pf->writeChar(itoa64(c->value()));
}

void installCore(lclEnvPtr env) {
    for (auto it = handlers.begin(), end = handlers.end(); it != end; ++it) {
        lclBuiltIn* handler = *it;
        env->set(handler->name(), handler);
    }
}

static String printValues(lclValueIter begin, lclValueIter end,
                          const String& sep, bool readably)
{
    String out;

    if (begin != end) {
        out += (*begin)->print(readably);
        ++begin;
    }

    for ( ; begin != end; ++begin) {
        out += sep;
        out += (*begin)->print(readably);
    }

    return out;
}

static int countValues(lclValueIter begin, lclValueIter end)
{
    int result = 0;

    if (begin != end) {
        result += (*begin)->print(true).length() -2;
        ++begin;
    }

    for ( ; begin != end; ++begin) {
        result += (*begin)->print(true).length() -2;
    }

    return result;
}
