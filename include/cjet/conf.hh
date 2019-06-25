// Copyright 2017 Eryx <evorui аt gmаil dοt cοm>, All rights reserved.
//

#ifndef CJET_CONF_HH
#define CJET_CONF_HH

#include <unordered_map>

namespace cjet {
namespace conf {

static const int kConfigFileMaxSize = 8192; // 8 KB
static const char kConfigSectionStart = '[';
static const char kConfigSectionClose = ']';

typedef std::unordered_map<std::string, std::string> StringMap;

class ConfigCommon {
   private:
    std::string filename_;
    StringMap items_;
    bool inited_ = false;

   public:
    ConfigCommon() : filename_("") {};
    int Init(const std::string &filename);
    bool Inited();
    std::string Value(std::string name);
}; // class ConfigCommon

class Config {
   public:
    static int Init(const std::string &filename);
    static bool Inited();
    static std::string Value(std::string name);
    static size_t ValueSize(std::string name, size_t min, size_t max);
}; // class Config

} // namespace ini
} // namespace cjet

#endif
