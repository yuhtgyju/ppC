#ifndef BUILD_HPP
#define BUILD_HPP

#if defined(__LINUX__) || defined(__APPLE__)
#include <cstdlib>
#include <string>

inline bool isCompilerAvailable(const std::string &compiler) {
    const std::string command = "command -v " + compiler + " > /dev/null 2>&1";
    return (std::system(command.c_str()) == 0);
}
#elif defined(_WIN32)
#include <cstdlib>
#include <string>

inline bool isCompilerAvailable(const std::string &compiler) {
    const std::string command = "cmd.exe /c where " + compiler + " > nul 2>&1";
    return (std::system(command.c_str()) == 0);
}

#else // Hopefully always unreachable.
inline bool isCompilerAvailable(const std::string &compiler) {
    std::cerr << "Unknown OS" << '\n';
    exit(-2);
}
#endif

#endif //BUILD_HPP
