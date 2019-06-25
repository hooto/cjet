// Copyright 2018 Eryx <evorui аt gmail dοt com>, All rights reserved.
//

#ifndef CJET_UTIL_HH
#define CJET_UTIL_HH

namespace cjet {
namespace util {

static inline size_t str_to_size_t(const std::string &str) {
    char *end;
    return (size_t)std::strtoll(str.c_str(), &end, 10);
}

static inline std::string str_trim(const std::string &s, std::string cutset) {
    if (cutset.size() > 0 && s.size() > 0) {
        int i = s.find_first_not_of(cutset);
        int j = s.find_last_not_of(cutset);
        if (i == -1) {
            return "";
        } else {
            return s.substr(i, j - i + 1);
        }
    }
    return s;
}

} // namespace util
} // namespace cjet

#endif
