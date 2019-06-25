// Copyright 2017 Eryx <evorui аt gmаil dοt cοm>, All rights reserved.
//

#ifndef CJET_FLAG_HH
#define CJET_FLAG_HH

#include <string>
#include <unordered_map>

namespace cjet {
namespace flag {

extern void Init(int argc, char** argv);
extern std::string Program();
extern std::string Value(std::string name);
extern size_t ValueSize(std::string name, size_t min, size_t max);
extern bool Has(std::string name);

static inline size_t str_to_size_t(const std::string &str) {
    char *end;
    return (size_t)std::strtoll(str.c_str(), &end, 10);
}

} // namespace flag
} // namespace cjet

#endif
