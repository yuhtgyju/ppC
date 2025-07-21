#pragma once

#ifndef VERSION_HPP
#define VERSION_HPP

#if defined(DEBUG) || !defined(NDEBUG)
#define IsDebug true
#else
#define IsDebug false
#endif

#include <string>

namespace version {
    inline std::string getVersion() {
        return "0.0.1 " + (IsDebug ? std::string("Debug") : std::string("Release"));
    }
}

#endif //VERSION_HPP
