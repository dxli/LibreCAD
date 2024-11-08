#ifndef INCLUDE_ENVIRONMENT_H
#define INCLUDE_ENVIRONMENT_H

#include "MAL.h"

#include <map>

class malEnv : public RefCounted {
public:
    malEnv(malEnvPtr outer = NULL);
    malEnv(malEnvPtr outer,
           const StringVec& bindings,
           malValueIter argsBegin,
           malValueIter argsEnd);

    ~malEnv();

    void setLamdaMode(bool mode) { m_isLamda = mode; }
    bool isLamda() const { return m_isLamda; }

    malValuePtr get(const String& symbol);
    malEnvPtr   find(const String& symbol);
    malValuePtr set(const String& symbol, malValuePtr value);
    malEnvPtr   getRoot();

private:
    typedef std::map<String, malValuePtr> Map;
    Map m_map;
    malEnvPtr m_outer;
    StringVec m_bindings;
    bool m_isLamda = false;
};

#endif // INCLUDE_ENVIRONMENT_H
