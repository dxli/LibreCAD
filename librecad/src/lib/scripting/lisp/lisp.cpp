#include "lisp.h"
#include "MAL.h"

#include "Environment.h"
//#include "ReadLine.h"
#include "Types.h"

#include <iostream>
#include <filesystem>
#include <algorithm>
#include <QDebug>

#define MAX_FUNC 33

static const char* malEvalFunctionTable[MAX_FUNC] = {
    "and",
    "bound?",
    "boundp",
    "def!",
    "defun",
    "defmacro!",
    "do",
    "fn*",
    "getkword",
    "getvar",
    "if",
    "initget",
    "lambda",
    "load_dialog",
    "let*",
    "minus?",
    "minusp",
    "new_dialog",
    "or",
    "progn",
    "quasiquote",
    "quote",
    "repeat",
    "set",
    "setq",
    "setvar",
    "start_dialog",
    "trace",
    "try*",
    "untrace",
    "while",
    "zero?",
    "zerop"
};

namespace emb
{
std::stringstream cout;
}

int64_t malGuiId = 0;
bool traceDebug = false;

malValuePtr READ(const String& input);
String PRINT(malValuePtr ast);
String strToUpper(String s);
static void installFunctions(malEnvPtr env);
//  Installs functions, macros and constants implemented in MAL.
static void installEvalCore(malEnvPtr env);
//  Installs functions from EVAL, implemented in MAL.
static void openTile(const malGui* tile) ;

static void makeArgv(malEnvPtr env, int argc, char* argv[]);
static String safeRep(const String& input, malEnvPtr env);
static malValuePtr quasiquote(malValuePtr obj);

//static ReadLine s_readLine("~/.mal-history");

static malEnvPtr replEnv(new malEnv);

static malEnvPtr shadowEnv(new malEnv);

int LispRun_SimpleString(const char *command)
{
    String out = safeRep(command, replEnv);
    if (out.length() > 0) {
        std::cout << out << std::endl;
        fflush(stdout);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int LispRun_SimpleFile(const char *filename)
{
    String out = safeRep(STRF("(load-file \"%s\")", filename), replEnv);
    if (out.length() > 0) {
        std::cout << out << std::endl;
        fflush(stdout);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

std::string Lisp_EvalFile(const char *filename)
{
    return safeRep(STRF("(load-file \"%s\")", filename), replEnv);
}

std::string Lisp_EvalString(const String& input)
{
#if 0
    String out = lispOut.str();
    qDebug() << "out: " << out.c_str();
    out += safeRep(input, replEnv);
    qDebug() << "out: " << out.c_str();
    lispOut.str("");
    return out;
#else
    return safeRep(input.c_str(), replEnv);
#endif
}

int Lisp_Initialize(int argc, char* argv[])
{
    installCore(replEnv);
    installEvalCore(replEnv);
    installFunctions(replEnv);
    makeArgv(replEnv, argc, argv);
    if (argc > 1) {
        String filename = escape(argv[1]);
        safeRep(STRF("(load-file %s)", filename.c_str()), replEnv);
        return 0;
    }
    rep("(println (str \"LibreLisp [\" *host-language* \"]\"))", replEnv);
    return 0;
}

static String safeRep(const String& input, malEnvPtr env)
{
    try {
        return rep(input, env);
    }
    catch (malEmptyInputException&) {
        return String();
    }
    catch (malValuePtr& mv) {
        return "Error: " + mv->print(true);
    }
    catch (String& s) {
        return "Error: " + s;
    };
}

static void makeArgv(malEnvPtr env, int argc, char* argv[])
{
    malValueVec* args = new malValueVec();
    for (int i = 0; i < argc; i++) {
        args->push_back(mal::string(argv[i]));
    }
    env->set("*ARGV*", mal::list(args));
}

String rep(const String& input, malEnvPtr env)
{
    return PRINT(EVAL(READ(input), env));
}

malValuePtr READ(const String& input)
{
    return readStr(input);
}

malValuePtr EVAL(malValuePtr ast, malEnvPtr env)
{
    if (!env) {
        env = replEnv;
    }
    while (1) {

        const malEnvPtr dbgenv = env->find("DEBUG-EVAL");
        if (dbgenv && dbgenv->get("DEBUG-EVAL")->isTrue()) {
            std::cout << "EVAL: " << PRINT(ast) << "\n";
        }

        if (traceDebug) {
            std::cout << "TRACE: " << PRINT(ast) << std::endl;
        }

        const malList* list = DYNAMIC_CAST(malList, ast);
        if (!list || (list->count() == 0)) {
            return ast->eval(env);
        }

        // From here on down we are evaluating a non-empty list.
        // First handle the special forms.
        if (const malSymbol* symbol = DYNAMIC_CAST(malSymbol, list->item(0))) {
            String special = symbol->value();

            const malEnvPtr traceEnv = shadowEnv->find(strToUpper(special));
            if (traceEnv && traceEnv->get(strToUpper(special))->print(true) != "nil") {
                traceDebug = true;
                std::cout << "TRACE: " << PRINT(ast) << std::endl;
            }
            int argCount = list->count() - 1;

            if (special == "and") {
                checkArgsAtLeast("and", 2, argCount);
                int value = 0;
                for (int i = 1; i < argCount+1; i++) {
                    if (EVAL(list->item(i), env)->isTrue()) {
                        value |= 1;
                    }
                    else {
                        value |= 2;
                    }
                }
                return value == 3 ? mal::falseValue() : mal::trueValue();
            }
#if 0
            if (special == "bla") {
                checkArgsIs(special.c_str(), 1, argCount);
                std::cout << "bla da" << std::endl;
                malValuePtr foo = list->item(1);
                shadowEnv->set("TRACE", mal::string(list->item(1)->print(true)));
                return mal::symbol(list->item(1)->print(true));



#if 0
                malValueVec *items = new malValueVec(2);
                const malSymbol* bla = new malSymbol("do");
                //bla = EVAL(bla, env);

                std::cout << "bla: " << bla->print(true) << std::endl;
                items->at(0) = READ("do");  //mal::symbol("do");
                items->at(0) = list->item(1);
                malValuePtr exec = new malList(items);
                exec->eval(env);



                //std::cout << "bla da 3" << mal::list(items)->print(true) << std::endl;
                //EVAL(mal::list(items), env);
#endif
                return mal::nilValue();
            }
#endif
            if (special == "bound?" || special == "boundp") {
                checkArgsIs(special.c_str(), 1, argCount);
                if (EVAL(list->item(1), env)->print(true).compare("nil") == 0) {
                    return special == "bound?" ? mal::falseValue() : mal::nilValue();
                }
                else {
                    const malEnvPtr sym = env->find(EVAL(list->item(1), env)->print(true));

                    if(!sym) {
                        return special == "bound?" ? mal::falseValue() : mal::nilValue();
                    }
                    else {
                        if (env->get(EVAL(list->item(1), env)->print(true)) == mal::nilValue()) {
                            return special == "bound?" ? mal::falseValue() : mal::nilValue();
                        }
                    }
                }
                return mal::trueValue();
            }

            if (special == "debug-eval") {
                checkArgsIs("debug-eval", 1, argCount);
                if (list->item(1) == mal::trueValue()) {
                    env->set("DEBUG-EVAL", mal::trueValue());
                    return mal::trueValue();
                }
                else {
                    env->set("DEBUG-EVAL", mal::falseValue());
                    return mal::falseValue();
                }
            }

            if (special == "def!") {
                checkArgsIs("def!", 2, argCount);
                const malSymbol* id = VALUE_CAST(malSymbol, list->item(1));
                return env->set(id->value(), EVAL(list->item(2), env));
            }

            if (special == "defmacro!") {
                checkArgsIs("defmacro!", 2, argCount);

                const malSymbol* id = VALUE_CAST(malSymbol, list->item(1));
                malValuePtr body = EVAL(list->item(2), env);
                const malLambda* lambda = VALUE_CAST(malLambda, body);
                return env->set(id->value(), mal::macro(*lambda));
            }

            if (special == "defun") {
                checkArgsAtLeast("defun", 3, argCount);

                String macro = "(do";
                const malSymbol* id = VALUE_CAST(malSymbol, list->item(1));
                const malSequence* bindings =
                    VALUE_CAST(malSequence, list->item(2));
                StringVec params;
                for (int i = 0; i < bindings->count(); i++) {
                    const malSymbol* sym =
                        VALUE_CAST(malSymbol, bindings->item(i));
                    params.push_back(sym->value());
                }

                for (int i = 3; i <= argCount; i++) {
                    macro += " ";
                    macro += list->item(i)->print(true);
#if 0
                    for (auto it = params.begin(); it != params.end(); it++) {
                        if (list->item(i)->print(true).find(*it) != std::string::npos) {
                            std::cout << "parameter '" << *it << "' in: " << list->item(i)->print(true) << std::endl;
                        }
                    }
#endif
                }
                macro += ")";
                malValuePtr body = READ(macro);
                const malLambda* lambda = new malLambda(params, body, env);
                return env->set(id->value(), new malLambda(*lambda, true));
            }

            if (special == "do" || special == "progn") {
                checkArgsAtLeast(special.c_str(), 1, argCount);

                for (int i = 1; i < argCount; i++) {
                    EVAL(list->item(i), env);
                }
                ast = list->item(argCount);
                continue; // TCO
            }

            if (special == "fn*" || special == "lambda") {
                checkArgsIs(special.c_str(), 2, argCount);

                const malSequence* bindings =
                    VALUE_CAST(malSequence, list->item(1));
                StringVec params;
                for (int i = 0; i < bindings->count(); i++) {
                    const malSymbol* sym =
                        VALUE_CAST(malSymbol, bindings->item(i));
                    params.push_back(sym->value());
                }

                return mal::lambda(params, list->item(2), env);
            }

            if (special == "foreach") {
                checkArgsIs("foreach", 3, argCount);
                const malSymbol* sym =
                        VALUE_CAST(malSymbol, list->item(1));
                malSequence* each =
                    VALUE_CAST(malSequence, EVAL(list->item(2), env));

                malEnvPtr inner(new malEnv(env));
                inner->set(sym->value(), mal::nilValue());
                int count = each->count();
                malValuePtr result = NULL;
                for (int i=0; i < count; i++) {
                    inner->set(sym->value(), each->item(i));
                    result = EVAL(list->item(3), inner);
                }
                if (result) {
                    return result;
                }
                return mal::nilValue();
            }

            if (special == "getkword") {
                checkArgsIs("getkword", 1, argCount);
                const malString* msg = VALUE_CAST(malString, list->item(1));
                std::cout << msg->value();

                const malString* pat = VALUE_CAST(malString, shadowEnv->get("INITGET-STR"));
                const malInteger* bit = VALUE_CAST(malInteger, shadowEnv->get("INITGET-BIT"));
                std::vector<String> StringList;
                String del = " ";
                String result;
                String pattern = pat->value();
                auto pos = pattern.find(del);

                while (pos != String::npos) {
                    StringList.push_back(pattern.substr(0, pos));
                    pattern.erase(0, pos + del.length());
                    pos = pattern.find(del);
                }
                StringList.push_back(pattern);

                while (getline (std::cin, result)) {
                    for (auto &it : StringList) {
                        if (it == result) {
                            return mal::string(result);
                        }
                    }
                    if ((bit->value() & 1) != 1) {
                        return mal::nilValue();
                    }
                    std::cout << msg->value();
                }
            }

            if (special == "getvar") {
                checkArgsIs("getvar", 1, argCount);
                malValuePtr value = shadowEnv->get(EVAL(list->item(1), NULL)->print(true));
                if (value) {
                    return value;
                }
                return mal::nilValue();
            }

            if (special == "if") {
                checkArgsBetween("if", 2, 3, argCount);

                bool isTrue = EVAL(list->item(1), env)->isTrue();
                if (!isTrue && (argCount == 2)) {
                    return mal::nilValue();
                }
                ast = list->item(isTrue ? 2 : 3);
                continue; // TCO
            }
            if (special == "initget") {
                checkArgsBetween("initget",1, 2, argCount);
                if (list->item(1)->type() == MALTYPE::INT && argCount == 2) {
                    shadowEnv->set("INITGET-BIT", EVAL(list->item(1), env));
                    shadowEnv->set("INITGET-STR", EVAL(list->item(2), env));
                }
                else {
                    shadowEnv->set("INITGET-BIT", mal::integer(0));
                    shadowEnv->set("INITGET-STR", EVAL(list->item(1), env));
                }
                return mal::nilValue();
            }

            if (special == "let*") {
                checkArgsIs("let*", 2, argCount);
                const malSequence* bindings =
                    VALUE_CAST(malSequence, list->item(1));
                int count = checkArgsEven("let*", bindings->count());
                malEnvPtr inner(new malEnv(env));
                for (int i = 0; i < count; i += 2) {
                    const malSymbol* var =
                        VALUE_CAST(malSymbol, bindings->item(i));
                    inner->set(var->value(), EVAL(bindings->item(i+1), inner));
                }
                ast = list->item(2);
                env = inner;
                continue; // TCO
            }

            if (special == "load_dialog") {
                checkArgsIs("load_dialog", 1, argCount);
                const malString* arg = DYNAMIC_CAST(malString, list->item(1));
                String path = arg->value();
                const std::filesystem::path p(path.c_str());
                if (!p.has_extension()) {
                    path += ".dcl";
                }
                if (!std::filesystem::exists(path.c_str())) {
                    return mal::integer(-1);
                }

                malValuePtr dcl = loadDcl(path);
                if (dcl) {
                    int uniq = ++malGuiId;
                    env->set(STRF("#builtin-gui(%d)", uniq), dcl);
                    return mal::integer(uniq);
                }
                return mal::integer(-1);
            }

            if (special == "minus?" || special == "minusp" ) {
                checkArgsIs(special.c_str(), 1, argCount);
                if (EVAL(list->item(1), env)->type() == MALTYPE::REAL) {
                    malDouble* val = VALUE_CAST(malDouble, EVAL(list->item(1), env));
                    if (special == "minus?") {
                        return mal::boolean(val->value() < 0.0);
                    }
                    else {
                        return val->value() < 0 ? mal::trueValue() : mal::nilValue();
                    }
                }
                else if (EVAL(list->item(1), env)->type() == MALTYPE::INT) {
                    malInteger* val = VALUE_CAST(malInteger, EVAL(list->item(1), env));
                    if (special == "minus?") {
                        return mal::boolean(val->value() < 0);
                    }
                    else {
                        return val->value() < 0 ? mal::trueValue() : mal::nilValue();
                    }
                }
                else {
                        return special == "minus?" ? mal::falseValue() : mal::nilValue();
                }
            }

            if (special == "new_dialog") {
                checkArgsAtLeast("new_dialog", 2, argCount);
                const malString* dlgName = DYNAMIC_CAST(malString, list->item(1));
                const malInteger* id = DYNAMIC_CAST(malInteger, EVAL(list->item(2), env));
                const malGui*    gui = DYNAMIC_CAST(malGui, env->get(STRF("#builtin-gui(%d)", id->value())));
                malValueVec* items = new malValueVec(gui->value().tiles->size());
                std::copy(gui->value().tiles->begin(), gui->value().tiles->end(), items->begin());

                for (auto it = items->begin(), end = items->end(); it != end; it++) {
                    const malGui* dlg = DYNAMIC_CAST(malGui, *it);
                    std::cout << "Dialog: " << dlg->value().name << std::endl;
                    if (dlg->value().name == dlgName->value()) {
                        openTile(dlg);
                        return mal::trueValue();
                    }
                }
                return mal::nilValue();
            }

            if (special == "or") {
                checkArgsAtLeast("or", 2, argCount);
                int value = 0;
                for (int i = 1; i < argCount+1; i++) {
                    if (EVAL(list->item(i), env)->isTrue()) {
                        value |= 1;
                    }
                    else {
                        value |= 2;
                    }
                }
                return value == 3 ? mal::trueValue() : mal::falseValue();
            }

            if (special == "quasiquote") {
                checkArgsIs("quasiquote", 1, argCount);
                ast = quasiquote(list->item(1));
                continue; // TCO
            }

            if (special == "quote") {
                checkArgsIs("quote", 1, argCount);
                return list->item(1);
            }

            if (special == "repeat") {
                checkArgsIs("repeat*", 2, argCount);
                const malInteger* loop = VALUE_CAST(malInteger, list->item(1));
                for (int64_t i = 1; i < loop->value(); i++) {
                    EVAL(list->item(argCount), env);
                }
                ast = list->item(argCount);
                continue; // TCO
            }

            if (special == "set") {
                checkArgsIs("set", 2, argCount);
                const malSymbol* id = new malSymbol(list->item(1)->print(true));
                return env->set(id->value(), EVAL(list->item(2), env));
            }

            if (special == "setq") {
                MAL_CHECK(checkArgsAtLeast(special.c_str(), 2, argCount) % 2 == 0, "setq: missing odd number");
                int i;
                for (i = 1; i < argCount - 2; i += 2) {
                    const malSymbol* id = VALUE_CAST(malSymbol, list->item(i));
                    env->set(id->value(), EVAL(list->item(i+1), env));
                }
                const malSymbol* id = VALUE_CAST(malSymbol, list->item(i));
                return env->set(id->value(), EVAL(list->item(i+1), env));
            }

            if (special == "setvar") {
                checkArgsIs("setvar", 2, argCount);
                const malSymbol* id = VALUE_CAST(malSymbol, list->item(1));
                return shadowEnv->set(id->value(), EVAL(list->item(2), env));
            }

            if (special == "start_dialog") {
                checkArgsIs("setvar", 0, argCount);

                return mal::integer(0);
            }

            if (special == "trace") {
                checkArgsIs("trace", 1, argCount);
                shadowEnv->set(strToUpper(list->item(1)->print(true)), mal::trueValue());
                return mal::symbol(list->item(1)->print(true));
            }

            if (special == "untrace") {
                checkArgsIs("untrace", 1, argCount);
                shadowEnv->set(strToUpper(list->item(1)->print(true)), mal::nilValue());
                return mal::symbol(strToUpper(list->item(1)->print(true)));
            }

            if (special == "try*") {
                malValuePtr tryBody = list->item(1);

                if (argCount == 1) {
                    ast = tryBody;
                    continue; // TCO
                }
                checkArgsIs("try*", 2, argCount);
                const malList* catchBlock = VALUE_CAST(malList, list->item(2));

                checkArgsIs("catch*", 2, catchBlock->count() - 1);
                MAL_CHECK(VALUE_CAST(malSymbol,
                    catchBlock->item(0))->value() == "catch*",
                    "catch block must begin with catch*");

                // We don't need excSym at this scope, but we want to check
                // that the catch block is valid always, not just in case of
                // an exception.
                const malSymbol* excSym =
                    VALUE_CAST(malSymbol, catchBlock->item(1));

                malValuePtr excVal;

                try {
                    return EVAL(tryBody, env);
                }
                catch(String& s) {
                    excVal = mal::string(s);
                }
                catch (malEmptyInputException&) {
                    // Not an error, continue as if we got nil
                    ast = mal::nilValue();
                }
                catch(malValuePtr& o) {
                    excVal = o;
                };

                if (excVal) {
                    // we got some exception
                    env = malEnvPtr(new malEnv(env));
                    env->set(excSym->value(), excVal);
                    ast = catchBlock->item(2);
                }
                continue; // TCO
            }

            if (special == "while") {
                checkArgsIs("while", 2, argCount);

                malValuePtr loop = list->item(1);
                malValuePtr loopBody = list->item(argCount);

                while (1) {
                    loopBody = EVAL(list->item(argCount), env);
                    loop = EVAL(list->item(1), env);

                    if (!loop->isTrue()) {
                        ast = loopBody;
                        break;
                    }
                }
                continue; // TCO
            }
            if (special == "zero?" || special == "zerop") {
                                checkArgsIs(special.c_str(), 1, argCount);
                if (EVAL(list->item(1), env)->type() == MALTYPE::REAL) {
                    malDouble* val = VALUE_CAST(malDouble, EVAL(list->item(1), env));
                    if (special == "zero?") {
                        return mal::boolean(val->value() == 0.0);
                    }
                    else {
                        return val->value() == 0 ? mal::trueValue() : mal::nilValue();
                    }
                }
                else if (EVAL(list->item(1), env)->type() == MALTYPE::INT) {
                    malInteger* val = VALUE_CAST(malInteger, EVAL(list->item(1), env));
                    if (special == "zero?") {
                        return mal::boolean(val->value() == 0);
                    }
                    else {
                        return val->value() == 0 ? mal::trueValue() : mal::nilValue();
                    }
                }
                else {
                        return special == "zero?" ? mal::falseValue() : mal::nilValue();
                }
            }
        }

        // Now we're left with the case of a regular list to be evaluated.
        malValuePtr op = EVAL(list->item(0), env);
        if (const malLambda* lambda = DYNAMIC_CAST(malLambda, op)) {
            if (lambda->isMacro()) {
                ast = lambda->apply(list->begin()+1, list->end());
                traceDebug = false;
                continue; // TCO
            }
            malValueVec* items = STATIC_CAST(malList, list->rest())->evalItems(env);
            ast = lambda->getBody();
            env = lambda->makeEnv(items->begin(), items->end());
            traceDebug = false;
            continue; // TCO
        }
        else {
            malValueVec* items = STATIC_CAST(malList, list->rest())->evalItems(env);
            return APPLY(op, items->begin(), items->end());
        }
    }
}

String PRINT(malValuePtr ast)
{
    return ast->print(true);
}

malValuePtr APPLY(malValuePtr op, malValueIter argsBegin, malValueIter argsEnd)
{
    const malApplicable* handler = DYNAMIC_CAST(malApplicable, op);
    MAL_CHECK(handler != NULL,
              "'%s' is not applicable", op->print(true).c_str());

    return handler->apply(argsBegin, argsEnd);
}

static bool isSymbol(malValuePtr obj, const String& text)
{
    const malSymbol* sym = DYNAMIC_CAST(malSymbol, obj);
    return sym && (sym->value() == text);
}

//  Return arg when ast matches ('sym, arg), else NULL.
static malValuePtr starts_with(const malValuePtr ast, const char* sym)
{
    const malList* list = DYNAMIC_CAST(malList, ast);
    if (!list || list->isEmpty() || !isSymbol(list->item(0), sym))
        return NULL;
    checkArgsIs(sym, 1, list->count() - 1);
    return list->item(1);
}

static malValuePtr quasiquote(malValuePtr obj)
{
    if (DYNAMIC_CAST(malSymbol, obj) || DYNAMIC_CAST(malHash, obj))
        return mal::list(mal::symbol("quote"), obj);

    const malSequence* seq = DYNAMIC_CAST(malSequence, obj);
    if (!seq)
        return obj;

    const malValuePtr unquoted = starts_with(obj, "unquote");
    if (unquoted)
        return unquoted;

    malValuePtr res = mal::list(new malValueVec(0));
    for (int i=seq->count()-1; 0<=i; i--) {
        const malValuePtr elt     = seq->item(i);
        const malValuePtr spl_unq = starts_with(elt, "splice-unquote");
        if (spl_unq)
            res = mal::list(mal::symbol("concat"), spl_unq, res);
         else
            res = mal::list(mal::symbol("cons"), quasiquote(elt), res);
    }
    if (DYNAMIC_CAST(malVector, obj))
        res = mal::list(mal::symbol("vec"), res);
    return res;
}

static const char* malFunctionTable[] = {
    "(defmacro! cond (fn* (& xs) (if (> (count xs) 0) (list 'if (first xs) (if (> (count xs) 1) (nth xs 1) (throw \"odd number of forms to cond\")) (cons 'cond (rest (rest xs)))))))",
    "(defmacro! 2+ (fn* (zahl)(+ zahl 2)))",
    "(def! not (fn* (cond) (if cond false true)))",
    "(def! load-file (fn* (filename) \
        (eval (read-string (str \"(do \" (slurp filename) \"\nnil)\")))))",
    "(def! *host-language* \"C++\")",
    "(def! append concat)",
    "(def! car first)",
    "(def! length count)",
    "(def! load load-file)",
    "(def! strcat str)",
    "(def! type type?)",
    "(def! EOF -1)",
    "(def! *DCL* '())",
};

static void installFunctions(malEnvPtr env) {
    for (auto &function : malFunctionTable) {
        rep(function, env);
    }
}

static void installEvalCore(malEnvPtr env) {
    for (auto &function : malEvalFunctionTable) {
        env->set(function, mal::builtin(true, function));
    }
}

static void openTile(const malGui* tile)
{
    std::cout << "Name: "<< tile->value().name << std::endl;

    malValueVec* tiles = new malValueVec(tile->value().tiles->size());
    std::copy(tile->value().tiles->begin(), tile->value().tiles->end(), tiles->begin());

    for (auto it = tiles->begin(), end = tiles->end(); it != end; it++) {
        const malGui* tile = DYNAMIC_CAST(malGui, *it);
        std::cout << "found: " << tile->value().name << std::endl;
        openTile(tile);
    }
}

String strToUpper(String s)
{
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::toupper(c); } // correct
                  );
    return s;
}

