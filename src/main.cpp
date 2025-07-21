#include <iostream>
#include <fstream>
#include <deque>
#include <string>
#include <cstdlib>
#include <stdexcept>
#include "argparse.hpp"
#include "build.hpp"
#include "OSUtils.hpp"
#include "version.hpp"

bool findArgType(const std::deque<std::string> &arg, const std::string &type) {
    for (const auto& argstr : arg) {
        if (argstr == type) {
            return true;
        }
    }
    return false;
}

std::string reverseString(const std::string &s) {
    std::string out;
    for (std::size_t i = s.length(); i > 0; i--) {
        if (s[i - 1] == '(') {
            out += ')';
            continue;
        } if (s[i - 1] == ')') {
            out += '(';
            continue;
        } if (s[i - 1] == '{') {
            out += '}';
            continue;
        } if (s[i - 1] == '}') {
            out += '{';
            continue;
        } if (s[i - 1] == '<') {
            out += '>';
            continue;
        } if (s[i - 1] == '>') {
            out += '<';
            continue;
        } if (s[i - 1] == '[') {
            out += ']';
            continue;
        } if (s[i - 1] == ']') {
            out += '[';
            continue;
        }
        out += s[i - 1];
    }
    return out;
}

std::string removeExtension(const std::string& fileName) {
    size_t dotPos = fileName.find_last_of('.');
    if (dotPos != std::string::npos) {
        return fileName.substr(0, dotPos);
    }
    return fileName; // No extension found
}

#include <filesystem>
namespace fs = std::filesystem;

bool buildFile(const std::string &fileName) {
    const bool clang = isCompilerAvailable("clang++");
    const bool gcc = isCompilerAvailable("g++");
    std::string command;
    if (clang) command = "clang++";
    else if (gcc) command = "g++";
    else {
        std::cerr << "No suitable compiler found (clang++ or g++ required)." << std::endl;
        return false;
    }

    std::string cppFile = fileName + ".cpp";
    std::ofstream outfile(cppFile);
    if (!outfile) {
        std::cerr << "Failed to open output file for writing: " << cppFile << std::endl;
        return false;
    }

    std::ifstream infile(fileName);
    if (!infile) {
        std::cerr << "Input file not found: " << fileName << std::endl;
        return false;
    }

    std::deque<std::string> lines;
    std::deque<std::string> reversed;
    std::string line;
    while (std::getline(infile, line)) {
        lines.push_back(line);
    }

    for (auto it = lines.rbegin(); it != lines.rend(); ++it) {
        reversed.emplace_back(reverseString(*it));
    }

    for (const auto& liner : reversed) {
        outfile << liner << "\n";
    }
    outfile.close();

    std::string outputFileName = removeExtension(fileName);
#ifdef _WIN32
    outputFileName += ".exe";
#endif

    std::string absCppFile = fs::absolute(cppFile).string();
    std::string absOutputFile = fs::absolute(outputFileName).string();

    //std::cout << "[BUILD] Compiling: " << absCppFile << std::endl;
    //std::cout << "[BUILD] Output:    " << absOutputFile << std::endl;

    bool buildSuccess = runBuildCommand(command, absCppFile, absOutputFile);

    if (!buildSuccess) {
        std::cerr << "Build failed." << std::endl;
        return false;
    }

    if (!removeFile(cppFile)) {
        std::cerr << "Warning: Failed to remove temp file " << cppFile << std::endl;
    }

    return true;
}

int main(const int argc, const char **argv) {
    std::deque<std::string> args = argParse(argc, argv);
    if (findArgType(args, "help") || findArgType(args, "h")) {
        std::cout << "   --help | -h > Print this menu\n"
                     "--version | -v > Print version info\n"
                     "  --build | -b > Build ++C source\n"
                     "    --AST      > View the ast ++C uses" << '\n';
        std::flush(std::cout); // flush the output buffer
        return 0; // Exit early.
    }

    if (findArgType(args, "AST")) {
        std::cerr << "++C doesn't use an AST." << '\n';
        return 12526246;
    }

    if (findArgType(args, "version") || findArgType(args, "v")) {
        std::cout << "++C version: " << version::getVersion() << '\n';
        std::flush(std::cout);
        return 0;
    }

    if (findArgType(args, "build") || findArgType(args, "b")) {
        try {
            std::cout << "Building ++C source... " << args.at(2) << '\n';
        } catch ([[maybe_unused]] std::out_of_range &e) {
            std::cerr << "Error: No selected file" << '\n';
            return -1;
        }
        return buildFile(args.at(2)) - 1; // true = 1 - 1 = 0. false = 0 - 1 = -1.
    }

    return 0;
}