#include "lisp.h"
#include "LCL.h"

#include "Environment.h"
//#include "ReadLine.h"
#include "Types.h"

#include <iostream>
#include <filesystem>
#include <algorithm>
#include <QDebug>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#define MAX_FUNC 36

static const char* lclEvalFunctionTable[MAX_FUNC] = {
    "action_tile",
    "and",
    "bound?",
    "boundp",
    "def!",
    "defun",
    "defmacro!",
    "do",
    "done_dialog",
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
    "unload_dialog",
    "untrace",
    "while",
    "zero?",
    "zerop"
};

int64_t lclGuiId = 0;
bool traceDebug = false;

QWidget *dclDialog = nullptr;
QVBoxLayout *vLayout = nullptr;

lclValuePtr READ(const String& input);
String PRINT(lclValuePtr ast);
String strToUpper(String s);

static void installFunctions(lclEnvPtr env);
//  Installs functions, macros and constants implemented in LCL.
static void installEvalCore(lclEnvPtr env);
//  Installs functions from EVAL, implemented in LCL.
static void openTile(const lclGui* tile);

static void makeArgv(lclEnvPtr env, int argc, char* argv[]);
static String safeRep(const String& input, lclEnvPtr env);
static lclValuePtr quasiquote(lclValuePtr obj);

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

static String safeRep(const String& input, lclEnvPtr env)
{
    try {
        return rep(input, env);
    }
    catch (lclEmptyInputException&) {
        return String();
    }
    catch (lclValuePtr& mv) {
        return "Error: " + mv->print(true);
    }
    catch (String& s) {
        return "Error: " + s;
    };
}

static void makeArgv(lclEnvPtr env, int argc, char* argv[])
{
    lclValueVec* args = new lclValueVec();
    for (int i = 0; i < argc; i++) {
        args->push_back(lcl::string(argv[i]));
    }
    env->set("*ARGV*", lcl::list(args));
}

String rep(const String& input, lclEnvPtr env)
{
    return PRINT(EVAL(READ(input), env));
}

lclValuePtr READ(const String& input)
{
    return readStr(input);
}

lclValuePtr EVAL(lclValuePtr ast, lclEnvPtr env)
{
    if (!env) {
        env = replEnv;
    }
    while (1) {

        const lclEnvPtr dbgenv = env->find("DEBUG-EVAL");
        if (dbgenv && dbgenv->get("DEBUG-EVAL")->isTrue()) {
            std::cout << "EVAL: " << PRINT(ast) << "\n";
        }

        if (traceDebug) {
            std::cout << "TRACE: " << PRINT(ast) << std::endl;
        }

        const lclList* list = DYNAMIC_CAST(lclList, ast);
        if (!list || (list->count() == 0)) {
            return ast->eval(env);
        }

        // From here on down we are evaluating a non-empty list.
        // First handle the special forms.
        if (const lclSymbol* symbol = DYNAMIC_CAST(lclSymbol, list->item(0))) {
            String special = symbol->value();

            const lclEnvPtr traceEnv = shadowEnv->find(strToUpper(special));
            if (traceEnv && traceEnv->get(strToUpper(special))->print(true) != "nil") {
                traceDebug = true;
                std::cout << "TRACE: " << PRINT(ast) << std::endl;
            }
            int argCount = list->count() - 1;

            if (special == "action_tile") {
                checkArgsAtLeast("action_tile", 2, argCount);
                const lclString* id = VALUE_CAST(lclString, list->item(1));
                const lclString* action = VALUE_CAST(lclString, list->item(2));

                lclValuePtr value = dclEnv->get(id->value().c_str());
                qDebug() << "value->print(true)" << value->print(true).c_str();
                if (value->print(true).compare("nil") == 0) {
                    dclEnv->set(id->value().c_str(), lcl::string(action->value().c_str()));
                    return lcl::trueValue();
                }
                return lcl::nilValue();
            }

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
                return value == 3 ? lcl::falseValue() : lcl::trueValue();
            }
#if 0
            if (special == "bla") {
                checkArgsIs(special.c_str(), 1, argCount);
                std::cout << "bla da" << std::endl;
                lclValuePtr foo = list->item(1);
                shadowEnv->set("TRACE", lcl::string(list->item(1)->print(true)));
                return lcl::symbol(list->item(1)->print(true));



#if 0
                lclValueVec *items = new lclValueVec(2);
                const lclSymbol* bla = new lclSymbol("do");
                //bla = EVAL(bla, env);

                std::cout << "bla: " << bla->print(true) << std::endl;
                items->at(0) = READ("do");  //lcl::symbol("do");
                items->at(0) = list->item(1);
                lclValuePtr exec = new lclList(items);
                exec->eval(env);



                //std::cout << "bla da 3" << lcl::list(items)->print(true) << std::endl;
                //EVAL(lcl::list(items), env);
#endif
                return lcl::nilValue();
            }
#endif
            if (special == "bound?" || special == "boundp") {
                checkArgsIs(special.c_str(), 1, argCount);
                if (EVAL(list->item(1), env)->print(true).compare("nil") == 0) {
                    return special == "bound?" ? lcl::falseValue() : lcl::nilValue();
                }
                else {
                    const lclEnvPtr sym = env->find(EVAL(list->item(1), env)->print(true));

                    if(!sym) {
                        return special == "bound?" ? lcl::falseValue() : lcl::nilValue();
                    }
                    else {
                        if (env->get(EVAL(list->item(1), env)->print(true)) == lcl::nilValue()) {
                            return special == "bound?" ? lcl::falseValue() : lcl::nilValue();
                        }
                    }
                }
                return lcl::trueValue();
            }

            if (special == "debug-eval") {
                checkArgsIs("debug-eval", 1, argCount);
                if (list->item(1) == lcl::trueValue()) {
                    env->set("DEBUG-EVAL", lcl::trueValue());
                    return lcl::trueValue();
                }
                else {
                    env->set("DEBUG-EVAL", lcl::falseValue());
                    return lcl::falseValue();
                }
            }

            if (special == "def!") {
                checkArgsIs("def!", 2, argCount);
                const lclSymbol* id = VALUE_CAST(lclSymbol, list->item(1));
                return env->set(id->value(), EVAL(list->item(2), env));
            }

            if (special == "defmacro!") {
                checkArgsIs("defmacro!", 2, argCount);

                const lclSymbol* id = VALUE_CAST(lclSymbol, list->item(1));
                lclValuePtr body = EVAL(list->item(2), env);
                const lclLambda* lambda = VALUE_CAST(lclLambda, body);
                return env->set(id->value(), lcl::macro(*lambda));
            }

            if (special == "defun") {
                checkArgsAtLeast("defun", 3, argCount);

                String macro = "(do";
                const lclSymbol* id = VALUE_CAST(lclSymbol, list->item(1));
                const lclSequence* bindings =
                    VALUE_CAST(lclSequence, list->item(2));
                StringVec params;
                for (int i = 0; i < bindings->count(); i++) {
                    const lclSymbol* sym =
                        VALUE_CAST(lclSymbol, bindings->item(i));
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
                lclValuePtr body = READ(macro);
                const lclLambda* lambda = new lclLambda(params, body, env);
                return env->set(id->value(), new lclLambda(*lambda, true));
            }

            if (special == "do" || special == "progn") {
                checkArgsAtLeast(special.c_str(), 1, argCount);

                for (int i = 1; i < argCount; i++) {
                    EVAL(list->item(i), env);
                }
                ast = list->item(argCount);
                continue; // TCO
            }

            if (special == "done_dialog") {
                checkArgsBetween("done_dialog", 0, 1, argCount);
                if (dclDialog != nullptr)
                {
                    lclValueVec* items = new lclValueVec(2);
                    items->at(0) = lcl::integer(dclDialog->x());
                    items->at(1) = lcl::integer(dclDialog->y());
                    dclDialog->close();
                    return lcl::list(items);
                }
            }

            if (special == "fn*" || special == "lambda") {
                checkArgsIs(special.c_str(), 2, argCount);

                const lclSequence* bindings =
                    VALUE_CAST(lclSequence, list->item(1));
                StringVec params;
                for (int i = 0; i < bindings->count(); i++) {
                    const lclSymbol* sym =
                        VALUE_CAST(lclSymbol, bindings->item(i));
                    params.push_back(sym->value());
                }

                return lcl::lambda(params, list->item(2), env);
            }

            if (special == "foreach") {
                checkArgsIs("foreach", 3, argCount);
                const lclSymbol* sym =
                        VALUE_CAST(lclSymbol, list->item(1));
                lclSequence* each =
                    VALUE_CAST(lclSequence, EVAL(list->item(2), env));

                lclEnvPtr inner(new lclEnv(env));
                inner->set(sym->value(), lcl::nilValue());
                int count = each->count();
                lclValuePtr result = NULL;
                for (int i=0; i < count; i++) {
                    inner->set(sym->value(), each->item(i));
                    result = EVAL(list->item(3), inner);
                }
                if (result) {
                    return result;
                }
                return lcl::nilValue();
            }

            if (special == "getkword") {
                checkArgsIs("getkword", 1, argCount);
                const lclString* msg = VALUE_CAST(lclString, list->item(1));
                std::cout << msg->value();

                const lclString* pat = VALUE_CAST(lclString, shadowEnv->get("INITGET-STR"));
                const lclInteger* bit = VALUE_CAST(lclInteger, shadowEnv->get("INITGET-BIT"));
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
                            return lcl::string(result);
                        }
                    }
                    if ((bit->value() & 1) != 1) {
                        return lcl::nilValue();
                    }
                    std::cout << msg->value();
                }
            }

            if (special == "getvar") {
                checkArgsIs("getvar", 1, argCount);
                lclValuePtr value = shadowEnv->get(EVAL(list->item(1), NULL)->print(true));
                if (value) {
                    return value;
                }
                return lcl::nilValue();
            }

            if (special == "if") {
                checkArgsBetween("if", 2, 3, argCount);

                bool isTrue = EVAL(list->item(1), env)->isTrue();
                if (!isTrue && (argCount == 2)) {
                    return lcl::nilValue();
                }
                ast = list->item(isTrue ? 2 : 3);
                continue; // TCO
            }
            if (special == "initget") {
                checkArgsBetween("initget",1, 2, argCount);
                if (list->item(1)->type() == LCLTYPE::INT && argCount == 2) {
                    shadowEnv->set("INITGET-BIT", EVAL(list->item(1), env));
                    shadowEnv->set("INITGET-STR", EVAL(list->item(2), env));
                }
                else {
                    shadowEnv->set("INITGET-BIT", lcl::integer(0));
                    shadowEnv->set("INITGET-STR", EVAL(list->item(1), env));
                }
                return lcl::nilValue();
            }

            if (special == "let*") {
                checkArgsIs("let*", 2, argCount);
                const lclSequence* bindings =
                    VALUE_CAST(lclSequence, list->item(1));
                int count = checkArgsEven("let*", bindings->count());
                lclEnvPtr inner(new lclEnv(env));
                for (int i = 0; i < count; i += 2) {
                    const lclSymbol* var =
                        VALUE_CAST(lclSymbol, bindings->item(i));
                    inner->set(var->value(), EVAL(bindings->item(i+1), inner));
                }
                ast = list->item(2);
                env = inner;
                continue; // TCO
            }

            if (special == "load_dialog") {
                checkArgsIs("load_dialog", 1, argCount);
                const lclString* arg = DYNAMIC_CAST(lclString, list->item(1));
                String path = arg->value();
                const std::filesystem::path p(path.c_str());
                if (!p.has_extension()) {
                    path += ".dcl";
                }
                if (!std::filesystem::exists(path.c_str())) {
                    return lcl::integer(-1);
                }

                lclValuePtr dcl = loadDcl(path);
                if (dcl) {
                    int uniq = ++lclGuiId;
                    dclEnv->set(STRF("#builtin-gui(%d)", uniq), dcl);
                    return lcl::integer(uniq);
                }
                return lcl::integer(-1);
            }

            if (special == "minus?" || special == "minusp" ) {
                checkArgsIs(special.c_str(), 1, argCount);
                if (EVAL(list->item(1), env)->type() == LCLTYPE::REAL) {
                    lclDouble* val = VALUE_CAST(lclDouble, EVAL(list->item(1), env));
                    if (special == "minus?") {
                        return lcl::boolean(val->value() < 0.0);
                    }
                    else {
                        return val->value() < 0 ? lcl::trueValue() : lcl::nilValue();
                    }
                }
                else if (EVAL(list->item(1), env)->type() == LCLTYPE::INT) {
                    lclInteger* val = VALUE_CAST(lclInteger, EVAL(list->item(1), env));
                    if (special == "minus?") {
                        return lcl::boolean(val->value() < 0);
                    }
                    else {
                        return val->value() < 0 ? lcl::trueValue() : lcl::nilValue();
                    }
                }
                else {
                        return special == "minus?" ? lcl::falseValue() : lcl::nilValue();
                }
            }

            if (special == "new_dialog") {
                checkArgsAtLeast("new_dialog", 2, argCount);
                const lclString* dlgName = DYNAMIC_CAST(lclString, list->item(1));
                const lclInteger* id = DYNAMIC_CAST(lclInteger, EVAL(list->item(2), env));
                const lclGui*     gui = DYNAMIC_CAST(lclGui, dclEnv->get(STRF("#builtin-gui(%d)", id->value())));
                lclValueVec* items = new lclValueVec(gui->value().tiles->size());
                std::copy(gui->value().tiles->begin(), gui->value().tiles->end(), items->begin());

                for (auto it = items->begin(), end = items->end(); it != end; it++) {
                    const lclGui* dlg = DYNAMIC_CAST(lclGui, *it);
                    std::cout << "Dialog: " << dlg->value().name << std::endl;
                    if (dlg->value().name == dlgName->value()) {
                        openTile(dlg);
                        return lcl::trueValue();
                    }
                }
                return lcl::nilValue();
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
                return value == 3 ? lcl::trueValue() : lcl::falseValue();
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
                checkArgsAtLeast("repeat", 2, argCount);
                const lclInteger* loop = VALUE_CAST(lclInteger, list->item(1));
                lclValuePtr loopBody;

                for (int i = 0; i < loop->value(); i++) {
                    for (int j = 1; j < argCount; j++)
                    {
                        loopBody = EVAL(list->item(j+1), env);
                    }
                }
                ast = loopBody;
                continue; // TCO
            }

            if (special == "set") {
                checkArgsIs("set", 2, argCount);
                const lclSymbol* id = new lclSymbol(list->item(1)->print(true));
                return env->set(id->value(), EVAL(list->item(2), env));
            }

            if (special == "setq") {
                LCL_CHECK(checkArgsAtLeast(special.c_str(), 2, argCount) % 2 == 0, "setq: missing odd number");
                int i;
                for (i = 1; i < argCount - 2; i += 2) {
                    const lclSymbol* id = VALUE_CAST(lclSymbol, list->item(i));
                    env->set(id->value(), EVAL(list->item(i+1), env));
                }
                const lclSymbol* id = VALUE_CAST(lclSymbol, list->item(i));
                return env->set(id->value(), EVAL(list->item(i+1), env));
            }

            if (special == "setvar") {
                checkArgsIs("setvar", 2, argCount);
                const lclSymbol* id = VALUE_CAST(lclSymbol, list->item(1));
                return shadowEnv->set(id->value(), EVAL(list->item(2), env));
            }

            if (special == "start_dialog") {
                checkArgsIs("setvar", 0, argCount);

                if (dclDialog != nullptr)
                {
                    dclDialog->show();
                }

                return lcl::integer(0);
            }

            if (special == "trace") {
                checkArgsIs("trace", 1, argCount);
                shadowEnv->set(strToUpper(list->item(1)->print(true)), lcl::trueValue());
                return lcl::symbol(list->item(1)->print(true));
            }

            if (special == "unload_dialog") {
                checkArgsIs("unload_dialog", 1, argCount);
                //const lclInteger* id = DYNAMIC_CAST(lclInteger, EVAL(list->item(1), env));

                if (dclDialog != nullptr)
                {
                    delete dclDialog;
                    dclDialog = nullptr;
                }

                return lcl::integer(0);
            }

            if (special == "untrace") {
                checkArgsIs("untrace", 1, argCount);
                shadowEnv->set(strToUpper(list->item(1)->print(true)), lcl::nilValue());
                return lcl::symbol(strToUpper(list->item(1)->print(true)));
            }

            if (special == "try*") {
                lclValuePtr tryBody = list->item(1);

                if (argCount == 1) {
                    ast = tryBody;
                    continue; // TCO
                }
                checkArgsIs("try*", 2, argCount);
                const lclList* catchBlock = VALUE_CAST(lclList, list->item(2));

                checkArgsIs("catch*", 2, catchBlock->count() - 1);
                LCL_CHECK(VALUE_CAST(lclSymbol,
                    catchBlock->item(0))->value() == "catch*",
                    "catch block must begin with catch*");

                // We don't need excSym at this scope, but we want to check
                // that the catch block is valid always, not just in case of
                // an exception.
                const lclSymbol* excSym =
                    VALUE_CAST(lclSymbol, catchBlock->item(1));

                lclValuePtr excVal;

                try {
                    return EVAL(tryBody, env);
                }
                catch(String& s) {
                    excVal = lcl::string(s);
                }
                catch (lclEmptyInputException&) {
                    // Not an error, continue as if we got nil
                    ast = lcl::nilValue();
                }
                catch(lclValuePtr& o) {
                    excVal = o;
                };

                if (excVal) {
                    // we got some exception
                    env = lclEnvPtr(new lclEnv(env));
                    env->set(excSym->value(), excVal);
                    ast = catchBlock->item(2);
                }
                continue; // TCO
            }

            if (special == "while") {
                checkArgsAtLeast("while", 2, argCount);
                lclValuePtr loop = list->item(1);
                lclValuePtr loopBody;

                while (1) {
                    for (int i = 1; i < argCount; i++)
                    {
                        loopBody = EVAL(list->item(i+1), env);
                        loop = EVAL(list->item(1), env);

                        if (!loop->isTrue()) {
                            break;
                        }
                    }

                    if (!loop->isTrue()) {
                        ast = loopBody;
                        break;
                    }
                }
                continue; // TCO
            }

            if (special == "zero?" || special == "zerop") {
                                checkArgsIs(special.c_str(), 1, argCount);
                if (EVAL(list->item(1), env)->type() == LCLTYPE::REAL) {
                    lclDouble* val = VALUE_CAST(lclDouble, EVAL(list->item(1), env));
                    if (special == "zero?") {
                        return lcl::boolean(val->value() == 0.0);
                    }
                    else {
                        return val->value() == 0 ? lcl::trueValue() : lcl::nilValue();
                    }
                }
                else if (EVAL(list->item(1), env)->type() == LCLTYPE::INT) {
                    lclInteger* val = VALUE_CAST(lclInteger, EVAL(list->item(1), env));
                    if (special == "zero?") {
                        return lcl::boolean(val->value() == 0);
                    }
                    else {
                        return val->value() == 0 ? lcl::trueValue() : lcl::nilValue();
                    }
                }
                else {
                        return special == "zero?" ? lcl::falseValue() : lcl::nilValue();
                }
            }
        }

        // Now we're left with the case of a regular list to be evaluated.
        lclValuePtr op = EVAL(list->item(0), env);
        if (const lclLambda* lambda = DYNAMIC_CAST(lclLambda, op)) {
            if (lambda->isMacro()) {
                ast = lambda->apply(list->begin()+1, list->end());
                traceDebug = false;
                continue; // TCO
            }
            lclValueVec* items = STATIC_CAST(lclList, list->rest())->evalItems(env);
            ast = lambda->getBody();
            env = lambda->makeEnv(items->begin(), items->end());
            traceDebug = false;
            continue; // TCO
        }
        else {
            lclValueVec* items = STATIC_CAST(lclList, list->rest())->evalItems(env);
            return APPLY(op, items->begin(), items->end());
        }
    }
}

String PRINT(lclValuePtr ast)
{
    return ast->print(true);
}

lclValuePtr APPLY(lclValuePtr op, lclValueIter argsBegin, lclValueIter argsEnd)
{
    const lclApplicable* handler = DYNAMIC_CAST(lclApplicable, op);
    LCL_CHECK(handler != NULL,
              "'%s' is not applicable", op->print(true).c_str());

    return handler->apply(argsBegin, argsEnd);
}

static bool isSymbol(lclValuePtr obj, const String& text)
{
    const lclSymbol* sym = DYNAMIC_CAST(lclSymbol, obj);
    return sym && (sym->value() == text);
}

//  Return arg when ast matches ('sym, arg), else NULL.
static lclValuePtr starts_with(const lclValuePtr ast, const char* sym)
{
    const lclList* list = DYNAMIC_CAST(lclList, ast);
    if (!list || list->isEmpty() || !isSymbol(list->item(0), sym))
        return NULL;
    checkArgsIs(sym, 1, list->count() - 1);
    return list->item(1);
}

static lclValuePtr quasiquote(lclValuePtr obj)
{
    if (DYNAMIC_CAST(lclSymbol, obj) || DYNAMIC_CAST(lclHash, obj))
        return lcl::list(lcl::symbol("quote"), obj);

    const lclSequence* seq = DYNAMIC_CAST(lclSequence, obj);
    if (!seq)
        return obj;

    const lclValuePtr unquoted = starts_with(obj, "unquote");
    if (unquoted)
        return unquoted;

    lclValuePtr res = lcl::list(new lclValueVec(0));
    for (int i=seq->count()-1; 0<=i; i--) {
        const lclValuePtr elt     = seq->item(i);
        const lclValuePtr spl_unq = starts_with(elt, "splice-unquote");
        if (spl_unq)
            res = lcl::list(lcl::symbol("concat"), spl_unq, res);
         else
            res = lcl::list(lcl::symbol("cons"), quasiquote(elt), res);
    }
    if (DYNAMIC_CAST(lclVector, obj))
        res = lcl::list(lcl::symbol("vec"), res);
    return res;
}

static const char* lclFunctionTable[] = {
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
    "(def! EOF -1)"
};

static void installFunctions(lclEnvPtr env) {
    for (auto &function : lclFunctionTable) {
        rep(function, env);
    }
}

static void installEvalCore(lclEnvPtr env) {
    for (auto &function : lclEvalFunctionTable) {
        env->set(function, lcl::builtin(true, function));
    }
}

static void openTile(const lclGui* tile)
{
    std::cout << "Name: "<< tile->value().name << std::endl;

    switch (tile->value().id) {
        case DIALOG:
        {
            const lclWidget* dlg = static_cast<const lclWidget*>(tile);
            dclDialog = dlg->widget();
            vLayout = new QVBoxLayout(dclDialog);
        }
            break;
        case ROW:
        {
            const lclRow* r = static_cast<const lclRow*>(tile);
            vLayout->addLayout(r->layout());
        }
            break;
        case COLUMN:
        {
            const lclColumn* c = static_cast<const lclColumn*>(tile);
            vLayout->addLayout(c->layout());
        }
            break;
        case TEXT:
        {
            const lclLabel* l = static_cast<const lclLabel*>(tile);
            vLayout->addWidget(l->label());
        }
            break;
        case BUTTON:
        {
            const lclButton* b = static_cast<const lclButton*>(tile);
            if (tile->value().key != "")
            {
                dclEnv->set(noQuotes(tile->value().key).c_str(), lcl::nilValue());
            }
#if 0
            switch (tile->value().alignment) {
            case LEFT:
                vLayout->
                break;
            case RIGHT:

                break;
            case TOP:

                break;
            case BOTTOM:

                break;
            case CENTERED:

                break;
            default:
                break;
            }
#endif
            vLayout->addWidget(b->button());
        }
            break;
        case RADIO_BUTTON:
        {
            const lclRadioButton* b = static_cast<const lclRadioButton*>(tile);
            if (tile->value().key != "")
            {
                dclEnv->set(noQuotes(tile->value().key).c_str(), lcl::nilValue());
            }
#if 0
            switch (tile->value().alignment) {
            case LEFT:
                vLayout->
                break;
            case RIGHT:

                break;
            case TOP:

                break;
            case BOTTOM:

                break;
            case CENTERED:

                break;
            default:
                break;
            }
#endif
            vLayout->addWidget(b->button());
        }

            break;
        default:
            break;
    }

    lclValueVec* tiles = new lclValueVec(tile->value().tiles->size());
    std::copy(tile->value().tiles->begin(), tile->value().tiles->end(), tiles->begin());

    for (auto it = tiles->begin(), end = tiles->end(); it != end; it++) {
        const lclGui* tile = DYNAMIC_CAST(lclGui, *it);
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

String noQuotes(const String& s)
{
    return s.substr(1, s.size() - 2);
}
