#ifndef OSUTILS_HPP
#define OSUTILS_HPP
#include <string>
#include <cstdlib>
#include <filesystem>

#ifdef _WIN32
inline std::string deleteCommandPrefix = "cmd.exe /c DEL /F /Q ";
inline std::string execCommandPrefix = "cmd.exe /c ";
#else
inline std::string deleteCommandPrefix = "rm ";
inline std::string execCommandPrefix = "";
#endif
namespace fs = std::filesystem;

inline fs::path absolute(const fs::path& p) {
    if (p.is_absolute()) {
        return p;
    } else {
        return fs::current_path() / p;
    }
}

// Returns true on successfully removing a file.
inline bool removeFile(const std::string &file) {
    std::string absPath = std::filesystem::absolute(file).string();
    return std::system((deleteCommandPrefix + "\"" + absPath + "\"").c_str()) == 0;
}


#include <filesystem>

namespace fs = std::filesystem;

inline bool runBuildCommand(const std::string &InCommand, const std::string &file, const std::string &OutCommand) {
    fs::path filePath = fs::absolute(file);
    fs::path outPath = fs::absolute(OutCommand);

    std::string cmd = execCommandPrefix + InCommand + " \"" + filePath.string() + "\" -o \"" + outPath.string() + "\"";
    //std::cout << "[BUILD CMD] " << cmd << std::endl;

    return std::system(cmd.c_str()) == 0;
}


#endif // OSUTILS_HPP
