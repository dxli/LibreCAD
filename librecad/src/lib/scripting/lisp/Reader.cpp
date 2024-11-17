#include "LCL.h"
#include "Types.h"

#ifdef DEVELOPER

#include <regex>
#include <fstream>
#include <iostream>
#include <string.h>

#include <QDebug>

typedef std::regex              Regex;

static const Regex intRegex("^[-+]?\\d+$");
static const Regex floatRegex("^[+-]?\\d+[.]{1}\\d+$");
static const Regex closeRegex("[\\)\\]}]");
static const Regex dclRegex("^[A-Za-z0-9_]+");

static const Regex whitespaceRegex("[\\s,]+|;.*|//.*");
static const Regex tokenRegexes[] = {
    Regex("~ "),
    Regex("~@"),
    Regex("[\\[\\]{}()'`~^@]"),
    Regex("\"(?:\\\\.|[^\\\\\"])*\""),
    Regex("[^\\s\\[\\]{}('\"`,;)]+"),
};

std::vector<tile_t> dclProtoTile;


static attribute_prop_t dclAttribute[MAX_DCL_ATTR] = {
    { "action", ACTION },
    { "alignment", ALIGNMENT },
    { "allow_accept", ALLOW_ACCEPT },
    { "aspect_ratio", ASPECT_RATIO },
    { "big_increment", BIG_INCREMENT },
    { "children_alignment", CHILDREN_ALIGNMENT },
    { "children_fixed_height", CHILDREN_FIXED_HEIGHT },
    { "children_fixed_width", CHILDREN_FIXED_WIDTH },
    { "color", COLOR },
    { "edit_limit", EDIT_LIMIT },
    { "edit_width", EDIT_WIDTH },
    { "fixed_height", FIXED_HEIGHT },
    { "fixed_width", FIXED_WIDTH },
    { "fixed_width_font", FIXED_WIDTH_FONT },
    { "height", HEIGHT },
    { "initial_focus", INITIAL_FOCUS },
    { "is_bold", IS_BOLD },
    { "is_cancel", IS_CANCEL },
    { "is_default", IS_DEFAULT },
    { "is_enabled", IS_ENABLED },
    { "is_tab_stop", IS_TAB_STOP },
    { "key", KEY },
    { "label", LABEL },
    { "layout", LAYOUT },
    { "list", LIST },
    { "max_value", MAX_VALUE },
    { "min_value", MIN_VALUE },
    { "mnemonic", MNEMONIC },
    { "multiple_select", MULTIPLE_SELECT },
    { "password_char", PASSWORD_CHAR },
    { "small_increment", SMALL_INCREMENT },
    { "tabs", TABS },
    { "tab_truncate", TAB_TRUNCATE },
    { "value", VALUE },
    { "width", WIDTH }
};

static position_prop_t dclPosition[MAX_DCL_POS] = {
    { "nopos", NOPOS },
    { "left", LEFT },
    { "right", RIGHT },
    { "top", TOP },
    { "bottom", BOTTOM },
    { "centered", CENTERED },
    { "horizontal", HORIZONTAL },
    { "vertical", VERTICAL }
};

static color_prop_t dclColor[MAX_DCL_COLOR] = {
    { "dialog_line", DIALOG_LINE },
    { "dialog_foreground", DIALOG_FOREGROUND },
    { "dialog_background", DIALOG_BACKGROUND },
    { "graphics_background", GRAPHICS_BACKGROUND },
    { "black", BLACK },
    { "red", RED },
    { "yellow", YELLOW },
    { "green", GREEN },
    { "cyan", CYAN },
    { "blue", BLUE },
    { "magenta", MAGENTA },
    { "white", WHITE },
    { "graphics_foreground", GRAPHICS_FOREGROUND }
};


class Tokeniser
{
public:
    Tokeniser(const String& input);

    String peek() const {
        ASSERT(!eof(), "Tokeniser reading past EOF in peek\n");
        return m_token;
    }

    String next() {
        ASSERT(!eof(), "Tokeniser reading past EOF in next\n");
        String ret = peek();
        nextToken();
        return ret;
    }

    bool eof() const {
        return m_iter == m_end;
    }

private:
    void skipWhitespace();
    void nextToken();

    bool matchRegex(const Regex& regex);

    typedef String::const_iterator StringIter;

    String      m_token;
    String      m_lastToken = "";
    StringIter  m_iter;
    StringIter  m_end;
};

Tokeniser::Tokeniser(const String& input)
:   m_iter(input.begin())
,   m_end(input.end())
{
    nextToken();
}

bool Tokeniser::matchRegex(const Regex& regex)
{
    if (eof()) {
        return false;
    }

    std::smatch match;
    auto flags = std::regex_constants::match_continuous;
    if (!std::regex_search(m_iter, m_end, match, regex, flags)) {
        return false;
    }

    ASSERT(match.size() == 1, "Should only have one submatch, not %lu\n",
                              match.size());
    ASSERT(match.position(0) == 0, "Need to match first character\n");
    ASSERT(match.length(0) > 0, "Need to match a non-empty string\n");

    // Don't advance  m_iter now, do it after we've consumed the token in
    // next().  If we do it now, we hit eof() when there's still one token left.
    m_token = match.str(0);

    return true;
}

void Tokeniser::nextToken()
{
    m_iter += m_token.size();

    skipWhitespace();
    if (eof()) {
        return;
    }

    for (auto &it : tokenRegexes) {
        if (matchRegex(it)) {
            return;
        }
    }

    String mismatch(m_iter, m_end);
    if (mismatch[0] == '"') {
        LCL_CHECK(false, "expected '\"', got EOF");
    }
    else {
        LCL_CHECK(false, "unexpected '%s'", mismatch.c_str());
    }
}

void Tokeniser::skipWhitespace()
{
    while (matchRegex(whitespaceRegex)) {
        m_iter += m_token.size();
    }
}

static lclValuePtr readAtom(Tokeniser& tokeniser);
static lclValuePtr readForm(Tokeniser& tokeniser);
static void readList(Tokeniser& tokeniser, lclValueVec* items,
                      const String& end);
static lclValuePtr processMacro(Tokeniser& tokeniser, const String& symbol);

lclValuePtr readStr(const String& input)
{
    Tokeniser tokeniser(input);
    if (tokeniser.eof()) {
        throw lclEmptyInputException();
    }
    return readForm(tokeniser);
}

static void readTile(Tokeniser& tokeniser, tile_t& tile);
static void copyTile(const tile_t &a, tile_t &b);
static bool ends_with(const std::string& str, const std::string& suffix);
static bool getDclBool(const String& str);
static bool isdclAttribute(const String& str);
#if 0
static bool isdclTile(const String& str);
#endif
static pos_t getDclPos(const String& str);
static tile_id_t getDclId(const String& str);
static color_t getDclColor(const String& str);
static attribute_id_t getDclAttributeId(const String& str);
static lclValuePtr readDclFile(Tokeniser& tokeniser, bool start=false);
static lclValuePtr addTile(tile_t tile);

lclValuePtr loadDcl(const String& path)
{
    std::ifstream file(path.c_str());
    String data;
    data.reserve(file.tellg());
    file.seekg(0, std::ios::beg);
    data.append(std::istreambuf_iterator<char>(file.rdbuf()),
    std::istreambuf_iterator<char>());

    Tokeniser tokeniser(data);
    if (tokeniser.eof()) {
        throw lclEmptyInputException();
    }
    return readDclFile(tokeniser, true);
}

static lclValuePtr readForm(Tokeniser& tokeniser)
{
    LCL_CHECK(!tokeniser.eof(), "expected form, got EOF");
    String token = tokeniser.peek();

    LCL_CHECK(!std::regex_match(token, closeRegex),
            "unexpected '%s'", token.c_str());

    if (token == "(") {
        tokeniser.next();
        std::unique_ptr<lclValueVec> items(new lclValueVec);
        readList(tokeniser, items.get(), ")");
        return lcl::list(items.release());
    }
    if (token == "[") {
        tokeniser.next();
        std::unique_ptr<lclValueVec> items(new lclValueVec);
        readList(tokeniser, items.get(), "]");
        return lcl::vector(items.release());
    }
    if (token == "{") {
        tokeniser.next();
        lclValueVec items;
        readList(tokeniser, &items, "}");
        return lcl::hash(items.begin(), items.end(), false);
    }
    return readAtom(tokeniser);
}

static lclValuePtr readAtom(Tokeniser& tokeniser)
{
    struct ReaderMacro {
        const char* token;
        const char* symbol;
    };
    ReaderMacro macroTable[] = {
        { "@",   "deref" },
        { "`",   "quasiquote" },
        { "'",   "quote" },
        { "~@",  "splice-unquote" },
        { "~",   "unquote" },
    };

    struct Constant {
        const char* token;
        lclValuePtr value;
    };
    Constant constantTable[] = {
        { "false",      lcl::falseValue()  },
        { "nil",        lcl::nilValue()    },
        { "true",       lcl::trueValue()   },
        { "T",          lcl::trueValue()   },
        { "pi",         lcl::piValue()     },
        { "!false",     lcl::falseValue()  },
        { "!nil",       lcl::nilValue()    },
        { "!true",      lcl::trueValue()   },
        { "!T",         lcl::trueValue()   },
        { "!pi",        lcl::piValue()     },
        { "ATOM",       lcl::typeAtom()    },
        { "FILE",       lcl::typeFile()    },
        { "INT",        lcl::typeInteger() },
        { "LIST",       lcl::typeList()    },
        { "REAL",       lcl::typeReal()    },
        { "STR",        lcl::typeString()  },
        { "VEC",        lcl::typeVector()  },
        { "KEYW",       lcl::typeKeword()  }
    };

    String token = tokeniser.next();
    if (token[0] == '"') {
        return lcl::string(unescape(token));
    }
    if (token[0] == ':') {
        return lcl::keyword(token);
    }
    if (token == "^") {
        lclValuePtr meta = readForm(tokeniser);
        lclValuePtr value = readForm(tokeniser);
        // Note that meta and value switch places
        return lcl::list(lcl::symbol("with-meta"), value, meta);
    }

    for (Constant  &constant : constantTable) {
        if (token == constant.token) {
            return constant.value;
        }
    }
    for (auto &macro : macroTable) {
        if (token == macro.token) {
            return processMacro(tokeniser, macro.symbol);
        }
    }
    if (std::regex_match(token, intRegex)) {
        return lcl::integer(token);
    }
    if (std::regex_match(token, floatRegex)) {
        return lcl::mdouble(token);
    }
    if (token[0] == '!') {
        return lcl::symbol(token.erase(0, 1));
    }
    return lcl::symbol(token);
}

static void readList(Tokeniser& tokeniser, lclValueVec* items,
                      const String& end)
{
    while (1) {
        LCL_CHECK(!tokeniser.eof(), "expected '%s', got EOF", end.c_str());
        if (tokeniser.peek() == end) {
            tokeniser.next();
            return;
        }
        items->push_back(readForm(tokeniser));
    }
}

static lclValuePtr processMacro(Tokeniser& tokeniser, const String& symbol)
{
    return lcl::list(lcl::symbol(symbol), readForm(tokeniser));
}

static lclValuePtr readDclFile(Tokeniser& tokeniser, bool start)
{
    LCL_CHECK(!tokeniser.eof(), "expected form, got EOF");
    String token = tokeniser.peek();
    int i;

    if (start) {
        tile_t tile;
        tile.name = "*DCL-TILES*";
        tile.tiles = new lclValueVec;
        readTile(tokeniser, tile);
        std::cout << "<-- *DCL-TILES* " << std::endl;
        return lcl::gui(tile);
    }

    if (token.compare(":") == 0) {
        tokeniser.next();
        token = tokeniser.peek();
        std::cout << "readDclFile(): ':' Dcl Name: " << tokeniser.peek() << std::endl;
        i = 0;
        for (auto &it : dclProtoTile) {
            if (token == it.name) {
                tokeniser.next();
                std::cout << "readDclFile(): got proto Dcl Tile..." << std::endl;
                tile_t tile;
                copyTile(dclProtoTile[i], tile);
                tokeniser.next();
                readTile(tokeniser, tile);
                return addTile(tile);
            }
            i++;
        }
        for (auto &it : dclTile) {
            if (ends_with(token, it.name)) {
                tokeniser.next();
                std::cout << "readDclFile(): known Dcl Tile..." << std::endl;
                tile_t tile;
                tile.name = token;
                tile.id = getDclId(token);
                tile.tiles = new lclValueVec;
                token = tokeniser.peek();
                tokeniser.next();
                readTile(tokeniser, tile);
                //qDebug() << "Debug" << tile.label.c_str();
                return addTile(tile);
            }
            if (token == it.name) {
                tokeniser.next();
                std::cout << "readDclFile(): real Dcl Tile..." << std::endl;
                tile_t tile;
                tile.name = token;
                tile.id = getDclId(token);
                tile.tiles = new lclValueVec;
                token = tokeniser.peek();
                tokeniser.next();
                readTile(tokeniser, tile);
                return addTile(tile);
            }
        }
    }

    if (std::regex_match(tokeniser.peek(), dclRegex)) {
        token = tokeniser.peek();
        std::cout << "readDclFile(): 'dclRegex' Dcl Name: " << tokeniser.peek() << std::endl;
        for (auto &it : dclTile) { //errortile, spacer, ...
            if (token == it.name) {
                std::cout << "readDclFile(): 'dclTile': " << tokeniser.peek() << std::endl;
                tile_t tile;
                tile.name = token;
                tile.id = getDclId(token);
                tile.tiles = new lclValueVec(0);
                token = tokeniser.peek();
                tokeniser.next();
                std::cout << "readDclFile(): 'dclTile' next: " << tokeniser.peek() << std::endl;
                return addTile(tile);
            }
        }
        tokeniser.next();
        tokeniser.next();
        tile_t tile;
        tile.name = token;
        tile.id = getDclId(tokeniser.peek());
        tile.tiles = new lclValueVec;
        tokeniser.next();
        readTile(tokeniser, tile);
        for (auto &it : dclTile) {
            if (ends_with(token, String("_").append(it.name))) {
                std::cout <<"readDclFile(): ("<< tile.name <<" = proto) ";
                dclProtoTile.push_back(tile);
                return NULL;
            }
        }
        return addTile(tile);
    }
    std::cout << "X-( : " << tokeniser.peek() << std::endl;
    return NULL;
}

static void readTile(Tokeniser& tokeniser, tile_t& tile)
{
    //std::cout << "readTile() token: " << tokeniser.peek() << std::endl;
    String token;
    while (1) {

        if (tokeniser.eof()) {
            return;
        }

        if (tokeniser.peek() == "}") {
            std::cout <<"("<<tile.name<<") - end";
            std::cout << " ... } -> bye bye..."<< std::endl;
            tokeniser.next();
            return;
        }

        if (tokeniser.peek() == "{") {
            std::cout <<"("<<tile.name<<") - start";
            std::cout << " -> { ..."<< std::endl;
            tokeniser.next();
            continue;
        }

        if (tokeniser.peek() == ":") {
            std::cout <<"("<<tile.name<<") ";
            std::cout << "readTile() ':' [<- readDclFile(tokeniser, false) token: " << tokeniser.peek() << std::endl;
            tile.tiles->push_back(readDclFile(tokeniser, false));
        }

        std::cout << "readTile() isdclAttribute for: " << tile.name << " ? ";
        if (isdclAttribute(tokeniser.peek())) {
            std::cout << "got attribute..." << std::endl;
            token = tokeniser.peek();
            tokeniser.next();
            tokeniser.next();
            switch (getDclAttributeId(token)) {
            case ACTION:
              tile.action = tokeniser.peek();
                break;
            case ALIGNMENT:
              tile.alignment = getDclPos(tokeniser.peek());
                break;
            case ALLOW_ACCEPT:
              tile.allow_accept = getDclBool(tokeniser.peek());
                break;
            case ASPECT_RATIO:
              tile.aspect_ratio = atof(tokeniser.peek().c_str());
                break;
            case BIG_INCREMENT:
              tile.big_increment = atoi(tokeniser.peek().c_str());
                break;
            case CHILDREN_ALIGNMENT:
              tile.children_alignment = getDclPos(tokeniser.peek());
                break;
            case CHILDREN_FIXED_HEIGHT:
              tile.children_fixed_height = getDclBool(tokeniser.peek());
                break;
            case CHILDREN_FIXED_WIDTH:
              tile.children_fixed_width = getDclBool(tokeniser.peek());
                break;
            case COLOR:
              tile.color = getDclColor(tokeniser.peek());
                break;
            case EDIT_LIMIT:
              tile.edit_limit = atoi(tokeniser.peek().c_str());
                break;
            case EDIT_WIDTH:
              tile.edit_width = atof(tokeniser.peek().c_str());
                break;
            case FIXED_HEIGHT:
              tile.fixed_height = getDclBool(tokeniser.peek());
                break;
            case FIXED_WIDTH:
              tile.fixed_width = getDclBool(tokeniser.peek());
                break;
            case FIXED_WIDTH_FONT:
              tile.fixed_width_font = getDclBool(tokeniser.peek());
                break;
            case HEIGHT:
              tile.height = atof(tokeniser.peek().c_str());
                break;
            case INITIAL_FOCUS:
              tile.initial_focus = tokeniser.peek();
                break;
            case IS_BOLD:
              tile.is_bold = getDclBool(tokeniser.peek());
                break;
            case IS_CANCEL:
              tile.is_cancel = getDclBool(tokeniser.peek());
                break;
            case IS_DEFAULT:
              tile.is_default = getDclBool(tokeniser.peek());
                break;
            case IS_ENABLED:
              tile.is_enabled = getDclBool(tokeniser.peek());
                break;
            case IS_TAB_STOP:
              tile.is_tab_stop = getDclBool(tokeniser.peek());
                break;
            case KEY:
              tile.key = tokeniser.peek();
                break;
            case LABEL:
              tile.label = tokeniser.peek();
                break;
            case LAYOUT:
              tile.layout = getDclPos(tokeniser.peek());
                break;
            case LIST:
              tile.list = tokeniser.peek();
                break;
            case MAX_VALUE:
              tile.max_value = atoi(tokeniser.peek().c_str());
                break;
            case MIN_VALUE:
              tile.min_value = atoi(tokeniser.peek().c_str());
                break;
            case MNEMONIC:
              tile.mnemonic = tokeniser.peek();
                break;
            case MULTIPLE_SELECT:
              tile.multiple_select = getDclBool(tokeniser.peek());
                break;
            case PASSWORD_CHAR:
              tile.password_char = tokeniser.peek();
                break;
            case SMALL_INCREMENT:
              tile.small_increment = atoi(tokeniser.peek().c_str());
                break;
            case TABS:
              tile.tabs = tokeniser.peek();
                break;
            case TAB_TRUNCATE:
              tile.tab_truncate = getDclBool(tokeniser.peek());
                break;
            case VALUE:
              tile.value = tokeniser.peek();
                break;
            case WIDTH:
              tile.width = atof(tokeniser.peek().c_str());
                break;
            default:
                break;
            }
            std::cout << token << " = " << tokeniser.peek() << std::endl;
            tokeniser.next();
            continue;
        }
        else {
            std::cout << " no attribute" << std::endl;
        }

        if (std::regex_match(tokeniser.peek(), dclRegex)) {
            std::cout <<"("<<tile.name<<") ";
            std::cout << "readTile() 'dclRegex' [<- readDclFile(tokeniser, false) token: " << tokeniser.peek() << std::endl;
            lclValuePtr result = readDclFile(tokeniser, false);
            if (result) {
                 tile.tiles->push_back(result);
            }
        }
        std::cout <<"("<<tile.name<<") Bottom"<< std::endl;
    }
}

static bool ends_with(const std::string& str, const std::string& suffix)
{
    return str.size() >= suffix.size() && str.compare(str.size()-suffix.size(), suffix.size(), suffix) == 0;
}
#if 0
static bool isdclTile(const String& str) {
    for (int i =0; i < MAX_DCL_TILES; i++) {
        if (str == dclTile[i].name) {
            return true;
        }
    }
    return false;
}
#endif
static tile_id_t getDclId(const String& str)
{
    tile_id_t id = NONE;
    for (int i =0; i < MAX_DCL_TILES; i++) {
        if (ends_with(str, dclTile[i].name)) {
            return dclTile[i].id;
        }
    }
    return id;
}

static bool isdclAttribute(const String& str) {
    for (int i =0; i < MAX_DCL_ATTR; i++) {
        if (str == dclAttribute[i].name) {
            return true;
        }
    }
    return false;
}

static attribute_id_t getDclAttributeId(const String& str)
{
    attribute_id_t id = NOATTR;
    for (int i =0; i < MAX_DCL_ATTR; i++) {
        if (str == dclAttribute[i].name) {
            return dclAttribute[i].id;
        }
    }
    return id;
}

static pos_t getDclPos(const String& str)
{
    pos_t pos = NOPOS;
    for (int i =0; i < MAX_DCL_TILES; i++) {
        if (ends_with(str, dclPosition[i].name)) {
            return dclPosition[i].pos;
        }
    }
    return pos;
}

static color_t getDclColor(const String& str)
{
    color_t color = WHITE;
    for (int i =0; i < MAX_DCL_COLOR; i++) {
        if (ends_with(str, dclColor[i].name)) {
            return dclColor[i].color;
        }
    }
    return color;
}

static bool getDclBool(const String& str)
{
    bool val = false;
    if (str == "true") {
        return true;
    }
    if (str == "false") {
        return false;
    }

    return val;
}

static void copyTile(const tile_t &a, tile_t &b)
{

    b.tiles = new lclValueVec(a.tiles->size());
    std::copy(a.tiles->begin(), a.tiles->end(), b.tiles->begin());
    b.action                 = a.action;
    b.alignment              = a.alignment;
    b.allow_accept           = a.allow_accept;
    b.aspect_ratio           = a.aspect_ratio;
    b.big_increment          = a.big_increment;
    b.children_alignment     = a.children_alignment;
    b.children_fixed_height  = a.children_fixed_height;
    b.children_fixed_width   = a.children_fixed_width;
    b.color                  = a.color;
    b.edit_limit             = a.edit_limit;
    b.edit_width             = a.edit_width;
    b.fixed_height           = a.fixed_height;
    b.fixed_width            = a.fixed_width;
    b.fixed_width_font       = a.fixed_width_font;
    b.height                 = a.height;
    b.initial_focus          = a.initial_focus;
    b.id                     = a.id;
    b.is_bold                = a.is_bold;
    b.is_cancel              = a.is_cancel;
    b.is_default             = a.is_default;
    b.is_enabled             = a.is_enabled;
    b.is_tab_stop            = a.is_tab_stop;
    b.key                    = a.key;
    b.label                  = a.label;
    b.layout                 = a.layout;
    b.list                   = a.list;
    b.max_value              = a.max_value;
    b.min_value              = a.min_value;
    b.mnemonic               = a.mnemonic;
    b.multiple_select        = a.multiple_select;
    b.name                   = a.name;
    b.password_char          = a.password_char;
    b.small_increment        = a.small_increment;
    b.tabs                   = a.tabs;
    b.tab_truncate           = a.tab_truncate;
    b.value                  = a.value;
    b.width                  = a.width;
}

static lclValuePtr addTile(tile_t tile)
{
    qDebug() << __func__ << tile.label.c_str();

    switch(tile.id) {
#if 0
    case BOXED_COLUMN:
        return lcl::boxed_column(tile);
    case BOXED_RADIO_COLUMN:
        return lcl::boxed_radio_column(tile);
    case BOXED_RADIO_ROW:
        return lcl::boxed_radio_row(tile);
#endif
    case BOXED_ROW:
        return lcl::boxedrow(tile);
    case BUTTON:
        return lcl::button(tile);

    case COLUMN:
        return lcl::column(tile);
#if 0
    case CONCATENATION:
        return lcl::concatenation(tile);
#endif
    case DIALOG:
        return lcl::widget(tile);
#if 0
    case EDIT_BOX:
        return lcl::edit_box(tile);
    case ERRTILE:
        return lcl::errtile(tile);
    case IMAGE:
        return lcl::image(tile);
    case IMAGE_BUTTON:
        return lcl::image_button(tile);
    case LIST_BOX:
        return lcl::list_box(tile);
    case OK_CANCEL:
        return lcl::ok_cancel(tile);
    case OK_CANCEL_HELP:
        return lcl::ok_cancel_help(tile);
    case OK_CANCEL_HELP_ERRTILE:
        return lcl::ok_cancel_help_errtile(tile);
    case OK_CANCEL_HELP_INFO:
        return lcl::ok_cancel_help_info(tile);
    case OK_ONLY:
        return lcl::ok_only(tile);
    case PARAGRAPH:
        return lcl::paragraph(tile);
    case POPUP_LIST:
        return lcl::popup_list(tile);
#endif
    case RADIO_BUTTON:
        return lcl::radiobutton(tile);
#if 0
    case RADIO_COLUMN:
        return lcl::radio_column(tile);
    case RADIO_ROW:
        return lcl::radio_row(tile);
#endif
    case ROW:
        return lcl::row(tile);
#if 0
    case SLIDER:
        return lcl::slider(tile);
    case SPACER:
        return lcl::spacer(tile);
    case SPACER_0:
        return lcl::spacer_0(tile);
    case SPACER_1:
        return lcl::spacer_1(tile);
#endif
    case TEXT:
        return lcl::label(tile);
#if 0
    case TEXT_PART:
        return lcl::text_part(tile);
    case TOGGLE:
        return lcl::toggle(tile);
#endif
    default:
        return lcl::gui(tile);
    }
}

#endif // DEVELOPER
