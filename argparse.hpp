#pragma once

#ifndef ARGPARSE_HPP
#define ARGPARSE_HPP

#include <deque>
#include <string>

inline std::deque<std::string> argParse(const int argAmount, const char **args) {
    std::deque<std::string> result;

    for (int i = 0; i < argAmount; i++) {
        result.emplace_back(args[i]);
    }
    int i = 0;
    for (auto &s : result) { // cleaner
        if (s.starts_with("--")) {
            result.at(i) = s.erase(0, 2);
        } else if (s.starts_with('-')) {
            result.at(i) = s.erase(0, 1);
        }
        i++;
    }
    return result;
}

#endif //ARGPARSE_HPP
