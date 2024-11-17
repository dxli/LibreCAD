#pragma once

#ifndef INCLUDE_TYPES_H
#define INCLUDE_TYPES_H

#include "MAL.h"

#include <exception>
#include <stdio.h>
#include <stdlib.h>
#include <map>

#include <iostream>
#include <QWidget>
#include <QPushButton>
#include <QRadioButton>
#include <QLabel>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

class malEmptyInputException : public std::exception { };

enum class MALTYPE { ATOM, BUILTIN, BOOLEAN, FILE, GUI, INT, LIST, MAP, REAL, STR, SYM, UNDEF, VEC, KEYW };

#define MAX_DCL_TILES 31
#define MAX_DCL_ATTR 35
#define MAX_DCL_POS 8
#define MAX_DCL_COLOR 13

class malValue : public RefCounted {
public:
    malValue() {
        TRACE_OBJECT("Creating malValue %p\n", this);
    }
    malValue(malValuePtr meta) : m_meta(meta) {
        TRACE_OBJECT("Creating malValue %p\n", this);
    }
    virtual ~malValue() {
        TRACE_OBJECT("Destroying malValue %p\n", this);
    }

    malValuePtr withMeta(malValuePtr meta) const;
    virtual malValuePtr doWithMeta(malValuePtr meta) const = 0;
    malValuePtr meta() const;

    bool isTrue() const;

    bool isEqualTo(const malValue* rhs) const;

    virtual malValuePtr eval(malEnvPtr env);

    virtual String print(bool readably) const = 0;

    virtual MALTYPE type() const { return MALTYPE::UNDEF; }

protected:
    virtual bool doIsEqualTo(const malValue* rhs) const = 0;

    malValuePtr m_meta;
};

template<class T>
T* value_cast(malValuePtr obj, const char* typeName) {
    T* dest = dynamic_cast<T*>(obj.ptr());
    MAL_CHECK(dest != NULL, "'%s' is not a %s",
              obj->print(true).c_str(), typeName);
    return dest;
}

#define VALUE_CAST(Type, Value)    value_cast<Type>(Value, #Type)
#define DYNAMIC_CAST(Type, Value)  (dynamic_cast<Type*>((Value).ptr()))
#define STATIC_CAST(Type, Value)   (static_cast<Type*>((Value).ptr()))

#define WITH_META(Type) \
    virtual malValuePtr doWithMeta(malValuePtr meta) const { \
        return new Type(*this, meta); \
    } \

class malConstant : public malValue {
public:
    malConstant(String name) : m_name(name) { }
    malConstant(const malConstant& that, malValuePtr meta)
        : malValue(meta), m_name(that.m_name) { }

    virtual String print(bool) const { return m_name; }

    virtual MALTYPE type() const {
        if ((m_name.compare("false") ||
            (m_name.compare("true")))) {
            return MALTYPE::BOOLEAN; }
        else {
            return MALTYPE::UNDEF; }
    }

    virtual bool doIsEqualTo(const malValue* rhs) const {
        return this == rhs; // these are singletons
    }

    WITH_META(malConstant)

private:
    const String m_name;
};

class malInteger : public malValue {
public:
    malInteger(int64_t value) : m_value(value) { }
    malInteger(const malInteger& that, malValuePtr meta)
        : malValue(meta), m_value(that.m_value) { }

    virtual String print(bool) const {
        return std::to_string(m_value);
    }

    virtual MALTYPE type() const { return MALTYPE::INT; }

    int64_t value() const { return m_value; }

    virtual bool doIsEqualTo(const malValue* rhs) const;

    WITH_META(malInteger)

private:
    const int64_t m_value;
};

class malDouble : public malValue {
public:
    malDouble(double value) : m_value(value) { }
    malDouble(const malDouble& that, malValuePtr meta)
        : malValue(meta), m_value(that.m_value) { }

    virtual String print(bool) const {
        return std::to_string(m_value);
    }

    virtual bool isFloat() const { return true; }
    virtual MALTYPE type() const { return MALTYPE::REAL; }

    double value() const { return m_value; }

    virtual bool doIsEqualTo(const malValue* rhs) const;

    WITH_META(malDouble)

private:
    const double m_value;
};

class malFile : public malValue {
public:
    malFile(const char *path, const char &mode)
        : m_path(path)
        , m_mode(mode)
    {
    }
    malFile(const malFile& that, malValuePtr meta)
        : malValue(meta), m_value(that.m_value) { }

    virtual String print(bool) const {
        String path = "#<file \"";
        path += m_path;
        path += "\">";
        return path;
    }

    virtual MALTYPE type() const { return MALTYPE::FILE; }

    virtual bool doIsEqualTo(const malValue* rhs) const {
        return value() == static_cast<const malFile*>(rhs)->value();
    }

    ::FILE *value() const { return m_value; }

    WITH_META(malFile)

    malValuePtr close();
    malValuePtr open();
    malValuePtr readLine();
    malValuePtr readChar();
    malValuePtr writeChar(const char &c);
    malValuePtr writeLine(const String &line);

private:
    String m_path;
    ::FILE *m_value;
    char m_mode;
};

class malStringBase : public malValue {
public:
    malStringBase(const String& token)
        : m_value(token) { }
    malStringBase(const malStringBase& that, malValuePtr meta)
        : malValue(meta), m_value(that.value()) { }

    virtual String print(bool) const { return m_value; }

    String value() const { return m_value; }

private:
    const String m_value;
};

class malString : public malStringBase {
public:
    malString(const String& token)
        : malStringBase(token) { }
    malString(const malString& that, malValuePtr meta)
        : malStringBase(that, meta) { }

    virtual String print(bool readably) const;
    virtual MALTYPE type() const { return MALTYPE::STR; }

    String escapedValue() const;

    virtual bool doIsEqualTo(const malValue* rhs) const {
        return value() == static_cast<const malString*>(rhs)->value();
    }

    WITH_META(malString)
};

class malKeyword : public malStringBase {
public:
    malKeyword(const String& token)
        : malStringBase(token) { }
    malKeyword(const malKeyword& that, malValuePtr meta)
        : malStringBase(that, meta) { }

    virtual bool doIsEqualTo(const malValue* rhs) const {
        return value() == static_cast<const malKeyword*>(rhs)->value();
    }

    virtual MALTYPE type() const { return MALTYPE::KEYW; }

    WITH_META(malKeyword)
};

class malSymbol : public malStringBase {
public:
    malSymbol(const String& token)
        : malStringBase(token) { }
    malSymbol(const malSymbol& that, malValuePtr meta)
        : malStringBase(that, meta) { }

    virtual malValuePtr eval(malEnvPtr env);

    virtual bool doIsEqualTo(const malValue* rhs) const {
        return value() == static_cast<const malSymbol*>(rhs)->value();
    }

    virtual MALTYPE type() const { return MALTYPE::SYM; }

    WITH_META(malSymbol)
};

class malSequence : public malValue {
public:
    malSequence(malValueVec* items);
    malSequence(malValueIter begin, malValueIter end);
    malSequence(const malSequence& that, malValuePtr meta);
    virtual ~malSequence();

    virtual String print(bool readably) const;

    malValueVec* evalItems(malEnvPtr env) const;
    int count() const { return m_items->size(); }
    bool isEmpty() const { return m_items->empty(); }
    bool isDotted() const;
    malValuePtr item(int index) const { return (*m_items)[index]; }

    malValueIter begin() const { return m_items->begin(); }
    malValueIter end()   const { return m_items->end(); }

    virtual bool doIsEqualTo(const malValue* rhs) const;

    virtual malValuePtr conj(malValueIter argsBegin,
                              malValueIter argsEnd) const = 0;
#if 0
    virtual malValuePtr append(malValueIter argsBegin,
                             malValueIter argsEnd) const;
#endif
    virtual malValueVec* append(malValueIter argsBegin,
                             malValueIter argsEnd) const;

    malValuePtr first() const;
    malValuePtr reverse(malValueIter argsBegin, malValueIter argsEnd) const;
    virtual malValuePtr rest() const;
    virtual malValuePtr dotted() const;

private:
    malValueVec* const m_items;
};

class malList : public malSequence {
public:
    malList(malValueVec* items) : malSequence(items) { }
    malList(malValueIter begin, malValueIter end)
        : malSequence(begin, end) { }
    malList(const malList& that, malValuePtr meta)
        : malSequence(that, meta) { }

    virtual String print(bool readably) const;
    virtual MALTYPE type() const { return MALTYPE::LIST; }
    virtual malValuePtr eval(malEnvPtr env);

    virtual malValuePtr conj(malValueIter argsBegin,
                             malValueIter argsEnd) const;

    WITH_META(malList)
};

class malVector : public malSequence {
public:
    malVector(malValueVec* items) : malSequence(items) { }
    malVector(malValueIter begin, malValueIter end)
        : malSequence(begin, end) { }
    malVector(const malVector& that, malValuePtr meta)
        : malSequence(that, meta) { }

    virtual malValuePtr eval(malEnvPtr env);
    virtual String print(bool readably) const;
    virtual MALTYPE type() const { return MALTYPE::VEC; }

    virtual malValuePtr conj(malValueIter argsBegin,
                             malValueIter argsEnd) const;

    WITH_META(malVector)
};

class malApplicable : public malValue {
public:
    malApplicable() { }
    malApplicable(malValuePtr meta) : malValue(meta) { }

    virtual malValuePtr apply(malValueIter argsBegin,
                               malValueIter argsEnd) const = 0;
};

class malHash : public malValue {
public:
    typedef std::map<String, malValuePtr> Map;

    malHash(malValueIter argsBegin, malValueIter argsEnd, bool isEvaluated);
    malHash(const malHash::Map& map);
    malHash(const malHash& that, malValuePtr meta)
    : malValue(meta), m_map(that.m_map), m_isEvaluated(that.m_isEvaluated) { }

    malValuePtr assoc(malValueIter argsBegin, malValueIter argsEnd) const;
    malValuePtr dissoc(malValueIter argsBegin, malValueIter argsEnd) const;
    bool contains(malValuePtr key) const;
    malValuePtr eval(malEnvPtr env);
    malValuePtr get(malValuePtr key) const;
    malValuePtr keys() const;
    malValuePtr values() const;

    virtual String print(bool readably) const;

    virtual bool doIsEqualTo(const malValue* rhs) const;

    virtual MALTYPE type() const { return MALTYPE::MAP; }

    WITH_META(malHash)

private:
    const Map m_map;
    const bool m_isEvaluated;
};

class malBuiltIn : public malApplicable {
public:
    typedef malValuePtr (ApplyFunc)(const String& name,
                                    malValueIter argsBegin,
                                    malValueIter argsEnd);

    malBuiltIn(const String& name, ApplyFunc* handler)
    : m_name(name), m_handler(handler) { }

    malBuiltIn(bool eval, const String& name)
    : m_inEval(eval), m_name(name) { }

    malBuiltIn(const malBuiltIn& that, malValuePtr meta)
    : malApplicable(meta), m_name(that.m_name), m_handler(that.m_handler) { }

    virtual malValuePtr apply(malValueIter argsBegin,
                              malValueIter argsEnd) const;

    virtual String print(bool) const {
        return STRF("#builtin-function(%s)", m_name.c_str());
    }

    virtual bool doIsEqualTo(const malValue* rhs) const {
        return this == rhs; // these are singletons
    }

    String name() const { return m_name; }

    virtual MALTYPE type() const { return MALTYPE::BUILTIN; }

    WITH_META(malBuiltIn)

private:
    [[maybe_unused]] bool m_inEval;
    const String m_name;
    ApplyFunc* m_handler;
};

class malLambda : public malApplicable {
public:
    malLambda(const StringVec& bindings, malValuePtr body, malEnvPtr env);
    malLambda(const malLambda& that, malValuePtr meta);
    malLambda(const malLambda& that, bool isMacro);

    virtual malValuePtr apply(malValueIter argsBegin,
                              malValueIter argsEnd) const;

    malValuePtr getBody() const { return m_body; }
    malEnvPtr makeEnv(malValueIter argsBegin, malValueIter argsEnd) const;

    virtual bool doIsEqualTo(const malValue* rhs) const {
        return this == rhs; // do we need to do a deep inspection?
    }

    virtual String print(bool) const {
        return STRF("#user-%s(%p)", m_isMacro ? "macro" : "function", this);
    }

    bool isMacro() const { return m_isMacro; }

    virtual malValuePtr doWithMeta(malValuePtr meta) const;

private:
    const StringVec   m_bindings;
    const malValuePtr m_body;
    const malEnvPtr   m_env;
    const bool        m_isMacro;
};

class malAtom : public malValue {
public:
    malAtom(malValuePtr value) : m_value(value) { }
    malAtom(const malAtom& that, malValuePtr meta)
        : malValue(meta), m_value(that.m_value) { }

    virtual bool doIsEqualTo(const malValue* rhs) const {
        return this->m_value->isEqualTo(rhs);
    }

    virtual String print(bool readably) const {
        return "(atom " + m_value->print(readably) + ")";
    };

    virtual MALTYPE type() const { return MALTYPE::ATOM; }

    malValuePtr deref() const { return m_value; }

    malValuePtr reset(malValuePtr value) { return m_value = value; }

    WITH_META(malAtom)

private:
    malValuePtr m_value;
};

enum TILE_ID {
    NONE,
    BOXED_COLUMN,
    BOXED_RADIO_COLUMN,
    BOXED_RADIO_ROW,
    BOXED_ROW,
    BUTTON,
    COLUMN,
    CONCATENATION,
    DIALOG,
    EDIT_BOX,
    ERRTILE,
    IMAGE,
    IMAGE_BUTTON,
    LIST_BOX,
    OK_CANCEL,
    OK_CANCEL_HELP,
    OK_CANCEL_HELP_ERRTILE,
    OK_CANCEL_HELP_INFO,
    OK_ONLY,
    PARAGRAPH,
    POPUP_LIST,
    RADIO_BUTTON,
    RADIO_COLUMN,
    RADIO_ROW,
    ROW,
    SLIDER,
    SPACER,
    SPACER_0,
    SPACER_1,
    TEXT,
    TEXT_PART,
    TOGGLE
};

typedef enum TILE_ID tile_id_t;

typedef struct tile_prop {
    const char* name;
    tile_id_t id;
} tile_prop_t;

static const tile_prop_t dclTile[MAX_DCL_TILES] = {
   { "boxed_column", BOXED_COLUMN },
   { "boxed_radio_column", BOXED_RADIO_COLUMN },
   { "boxed_radio_row", BOXED_RADIO_ROW },
   { "boxed_row",BOXED_ROW },
   { "button", BUTTON },
   { "column", COLUMN },
   { "concatenation", CONCATENATION },
   { "dialog", DIALOG },
   { "edit_box", EDIT_BOX },
   { "errtile", ERRTILE },
   { "image", IMAGE },
   { "image_button", IMAGE_BUTTON },
   { "list_box", LIST_BOX },
   { "ok_cancel", OK_CANCEL },
   { "ok_cancel_help", OK_CANCEL_HELP },
   { "ok_cancel_help_errtile", OK_CANCEL_HELP_ERRTILE },
   { "ok_cancel_help_info", OK_CANCEL_HELP_INFO },
   { "ok_only", OK_ONLY },
   { "paragraph", PARAGRAPH },
   { "popup_list", POPUP_LIST },
   { "radio_button", RADIO_BUTTON },
   { "radio_column", RADIO_COLUMN },
   { "radio_row", RADIO_ROW },
   { "row", ROW },
   { "slider", SLIDER },
   { "spacer", SPACER },
   { "spacer_0", SPACER_0 },
   { "spacer_1", SPACER_1 },
   { "text", TEXT },
   { "text_part", TEXT_PART },
   { "toggle", TOGGLE }
};

enum ATTRIBUTE_ID {
    NOATTR,
    ACTION,
    ALIGNMENT,
    ALLOW_ACCEPT,
    ASPECT_RATIO,
    BIG_INCREMENT,
    CHILDREN_ALIGNMENT,
    CHILDREN_FIXED_HEIGHT,
    CHILDREN_FIXED_WIDTH,
    COLOR,
    EDIT_LIMIT,
    EDIT_WIDTH,
    FIXED_HEIGHT,
    FIXED_WIDTH,
    FIXED_WIDTH_FONT,
    HEIGHT,
    INITIAL_FOCUS,
    IS_BOLD,
    IS_CANCEL,
    IS_DEFAULT,
    IS_ENABLED,
    IS_TAB_STOP,
    KEY,
    LABEL,
    LAYOUT,
    LIST,
    MAX_VALUE,
    MIN_VALUE,
    MNEMONIC,
    MULTIPLE_SELECT,
    PASSWORD_CHAR,
    SMALL_INCREMENT,
    TABS,
    TAB_TRUNCATE,
    VALUE,
    WIDTH
};

typedef enum ATTRIBUTE_ID attribute_id_t;

typedef struct attribute_prop {
    const char* name;
    attribute_id_t id;
} attribute_prop_t;

enum POS {
    NOPOS,
    LEFT,
    RIGHT,
    TOP,
    BOTTOM,
    CENTERED,
    HORIZONTAL,
    VERTICAL
};

typedef enum POS pos_t;
typedef struct position_prop {
    const char* name;
    pos_t pos;
} position_prop_t;

enum COLOR {
    DIALOG_LINE,
    DIALOG_FOREGROUND,
    DIALOG_BACKGROUND,
    GRAPHICS_BACKGROUND = 0,
    BLACK = 0,
    RED,
    YELLOW,
    GREEN,
    CYAN,
    BLUE,
    MAGENTA,
    WHITE,
    GRAPHICS_FOREGROUND = 7
};

typedef enum COLOR color_t;
typedef struct color_prop {
    const char* name;
    color_t color;
} color_prop_t;

typedef struct guitile {
    malValueVec*    tiles;
    QWidget*        parent = nullptr;
    String          action = "";
    pos_t           alignment = LEFT;
    bool            allow_accept = false;
    double          aspect_ratio = 0.0;
    int64_t         big_increment = 0;
    pos_t           children_alignment = LEFT;
    bool            children_fixed_height = false;
    bool            children_fixed_width = false;
    color_t         color = WHITE;
    int64_t         edit_limit = 132;
    double          edit_width = 0.0;
    bool            fixed_height = false;
    bool            fixed_width = false;
    bool            fixed_width_font = false;
    double          height = 0.0;
    String          initial_focus = "";
    tile_id_t       id;
    bool            is_bold = false;
    bool            is_cancel = false;
    bool            is_default = false;
    bool            is_enabled = true;
    bool            is_tab_stop = true;
    String          key = "";
    String          label = "";
    pos_t           layout = HORIZONTAL;
    String          list = "";
    int64_t         max_value = 10000;
    int64_t         min_value = 0;
    String          mnemonic = "";
    bool            multiple_select = false;
    String          name = "";
    String          password_char = "";
    int64_t         small_increment = 0;
    String          tabs = "";
    bool            tab_truncate = false;
    String          value = "";
    double          width = 0.0;
} tile_t;

class malGui : public malValue {
public:
    malGui(const tile_t& tile) : m_value(tile) { }
    malGui(const malGui& that, malValuePtr meta)
        : malValue(meta), m_value(that.m_value) { }

    virtual ~malGui() { delete m_value.tiles; }

    tile_t value() const { return m_value; }

    virtual String print(bool) const {
        return STRF("#builtin-gui(%s)", m_value.name.c_str());
    }

    virtual MALTYPE type() const { return MALTYPE::GUI; }

    virtual bool doIsEqualTo(const malValue*) const { return false; }

    virtual malValuePtr conj(malValueIter argsBegin,
                             malValueIter argsEnd) const;

    WITH_META(malGui)

private:
    const tile_t m_value;
};

class malWidget : public malGui {
public:
    malWidget(const tile_t& tile);
    malWidget(const malWidget& that, malValuePtr meta)
        : malGui(that, meta) { }

    virtual ~malWidget() { delete m_widget; }

    WITH_META(malWidget)

    QWidget* widget() const { return m_widget; }

private:
    QWidget* m_widget;
};

class malButton : public malGui {

public:
    malButton(const tile_t& tile);
    malButton(const malButton& that, malValuePtr meta)
        : malGui(that, meta) { }

    virtual ~malButton() { delete m_button; }

    WITH_META(malButton)

    QPushButton* button() const { return m_button; }

    void clicked(bool checked);

private:
    QPushButton* m_button;
};

class malRadioButton : public malGui {

public:
    malRadioButton(const tile_t& tile);
    malRadioButton(const malRadioButton& that, malValuePtr meta)
        : malGui(that, meta) { }

    virtual ~malRadioButton() { delete m_button; }

    WITH_META(malRadioButton)

    QRadioButton* button() const { return m_button; }

    void clicked(bool checked);

private:
    QRadioButton* m_button;
};

class malLabel : public malGui {

public:
    malLabel(const tile_t& tile);
    malLabel(const malLabel& that, malValuePtr meta)
        : malGui(that, meta) { }

    virtual ~malLabel() { delete m_label; }

    WITH_META(malLabel)

    QLabel* label() const { return m_label; }

private:
    QLabel* m_label;
};

class malRow : public malGui {
public:
    malRow(const tile_t& tile);
    malRow(const malRow& that, malValuePtr meta)
        : malGui(that, meta) { }

    virtual ~malRow() { delete m_layout; }

    WITH_META(malRow)

    QHBoxLayout* layout() const { return m_layout; }

private:
    QHBoxLayout* m_layout;
};

class malBoxedRow : public malGui {
public:
    malBoxedRow(const tile_t& tile);
    malBoxedRow(const malBoxedRow& that, malValuePtr meta)
        : malGui(that, meta) { }

    virtual ~malBoxedRow() { delete m_layout; delete m_groupbox; }

    WITH_META(malBoxedRow)

    QHBoxLayout* layout() const { return m_layout; }
    QGroupBox* groupbox() const { return m_groupbox; }

private:
    QHBoxLayout* m_layout;
    QGroupBox* m_groupbox;
};

class malColumn : public malGui {
public:
    malColumn(const tile_t& tile);
    malColumn(const malColumn& that, malValuePtr meta)
        : malGui(that, meta) { }

    virtual ~malColumn() { delete m_layout; }

    WITH_META(malColumn)

    QVBoxLayout* layout() const { return m_layout; }

private:
    QVBoxLayout* m_layout;
};

class malBoxedColumn : public malGui {
public:
    malBoxedColumn(const tile_t& tile);
    malBoxedColumn(const malBoxedColumn& that, malValuePtr meta)
        : malGui(that, meta) { }

    virtual ~malBoxedColumn() { delete m_layout; delete m_groupbox; }

    WITH_META(malBoxedColumn)

    QVBoxLayout* layout() const { return m_layout; }
    QGroupBox* groupbox() const { return m_groupbox; }

private:
    QVBoxLayout* m_layout;
    QGroupBox* m_groupbox;
};

namespace mal {
    malValuePtr atom(malValuePtr value);
    malValuePtr boolean(bool value);
    malValuePtr builtin(const String& name, malBuiltIn::ApplyFunc handler);
    malValuePtr builtin(bool eval, const String&);
    malValuePtr falseValue();
    malValuePtr file(const char *path, const char &mode);
    malValuePtr gui(const tile_t& tile);
    malValuePtr hash(malValueIter argsBegin, malValueIter argsEnd,
                     bool isEvaluated);
    malValuePtr hash(const malHash::Map& map);
    malValuePtr integer(int64_t value);
    malValuePtr integer(const String& token);
    malValuePtr keyword(const String& token);
    malValuePtr lambda(const StringVec&, malValuePtr, malEnvPtr);
    malValuePtr list(malValueVec* items);
    malValuePtr list(malValueIter begin, malValueIter end);
    malValuePtr list(malValuePtr a);
    malValuePtr list(malValuePtr a, malValuePtr b);
    malValuePtr list(malValuePtr a, malValuePtr b, malValuePtr c);
    malValuePtr macro(const malLambda& lambda);
    malValuePtr mdouble(double value);
    malValuePtr mdouble(const String& token);
    malValuePtr nilValue();
    malValuePtr nullValue();
    malValuePtr string(const String& token);
    malValuePtr symbol(const String& token);
    malValuePtr trueValue();
    malValuePtr type(MALTYPE type);
    malValuePtr typeAtom();
    malValuePtr typeBuiltin();
    malValuePtr typeFile();
    malValuePtr typeInteger();
    malValuePtr typeList();
    malValuePtr typeMap();
    malValuePtr typeReal();
    malValuePtr typeString();
    malValuePtr typeSymbol();
    malValuePtr typeUndef();
    malValuePtr typeVector();
    malValuePtr typeKeword();
    malValuePtr piValue();
    malValuePtr vector(malValueVec* items);
    malValuePtr vector(malValueIter begin, malValueIter end);

    malValuePtr widget(const tile_t& tile);
    malValuePtr boxedcolumn(const tile_t& tile);
    malValuePtr boxedrow(const tile_t& tile);
    malValuePtr button(const tile_t& tile);
    malValuePtr column(const tile_t& tile);
    malValuePtr label(const tile_t& tile);
    malValuePtr row(const tile_t& tile);
    malValuePtr radiobutton(const tile_t& tile);

}

#endif // INCLUDE_TYPES_H
