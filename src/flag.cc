// Copyright 2017 Eryx <evorui аt gmаil dοt cοm>, All rights reserved.
//

#include <iostream>
#include <string>
#include "cjet/flag.hh"

namespace cjet {
namespace flag {

typedef std::unordered_map<std::string, std::string> item_map;
static item_map items;
static std::string program_name;

void Init(int argc, char** argv) {

    program_name = (std::string)argv[0];
    if (program_name.rfind('/') >= 0) {
        program_name = program_name.substr(program_name.rfind('/') + 1);
    }

    if (argc < 2) {
        return;
    }

    std::string key_open = "";
    for (int i = 1; i < argc; i++) {

        std::string v = argv[i];

        if (v[0] == '=' && v.size() == 1) {
            continue;
        } else if (v[0] == '-' && v.size() >= 2) {

            if (key_open.size() > 0) {
                items[key_open] = "";
            }

            if (v[1] == '-') {
                key_open = v.substr(2);
            } else {
                key_open = v.substr(1);
            }

            if (key_open.size() == 0) {
                continue;
            }

            if (key_open[key_open.size() - 1] == '=') {
                if (key_open.size() == 1) {
                    continue;
                }
                key_open = key_open.substr(0, key_open.size() - 1);
            }

            int n = key_open.find("=");
            if (n == -1) {
                continue;
            } else if (n == 0) {
                key_open = "";
            } else {
                items[key_open.substr(0, n)] = key_open.substr(n + 1);
                key_open = "";
            }

        } else if (key_open.size() > 0) {
            if (v[0] == '=') {
                v = v.substr(1);
            }
            items[key_open] = v;
            key_open = "";
        }
    }

    if (key_open.size() > 0) {
        items[key_open] = "";
    }

    // for (auto p = items.begin(); p != items.end(); ++p) {
    //     std::cout << "ARG " << p->first << ": " << p->second << "\n";
    // }
};

std::string Program() { return program_name; }

std::string Value(std::string name) {
    if (items.size() > 0) {
        auto it = items.find(name);
        if (it != items.end()) {
            return it->second;
        }
    }
    return "";
}

size_t ValueSize(std::string name, size_t min, size_t max) {

    size_t i = str_to_size_t(Value(name));
    if (i < min) {
        i = min;
    } else if (i > max) {
        i = max;
    }

    return i;
}

bool Has(std::string name) {
    if (items.size() > 0) {
        auto it = items.find(name);
        if (it != items.end()) {
            return true;
        }
    }
    return false;
}

} // namespace flag
} // namespace cjet
