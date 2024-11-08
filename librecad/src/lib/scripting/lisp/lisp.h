#ifndef LISP_H
#define LISP_H
#include <MAL.h>

#include <stdio.h>
#include <streambuf>
#include <string>
#include <ostream>
#include <iostream>
#include <sstream>

/**
 * console streambuf
 */
#if 0
namespace emb
{
sstream cout;

class streambuf : public std::streambuf
{
public:
    // Somehow store a pointer to to_append.
    explicit streambuf(std::string &to_append) :
        m_to_append(&to_append){}

    virtual int_type overflow (int_type c) {
        if (c != EOF) {
            m_to_append->push_back(c);
        }
        return c;
    }

    virtual std::streamsize xsputn (const char* s, std::streamsize n) {
        m_to_append->insert(m_to_append->end(), s, s + n);
        return n;
    }

private:
    std::string *m_to_append;
};

} // namespace emb
#endif
int Lisp_Initialize(int argc=0, char* argv[]=NULL);

int LispRun_SimpleFile(const char *filename);

int LispRun_SimpleString(const char *command);

std::string Lisp_EvalString(const String& input);

#endif // LISP_H
