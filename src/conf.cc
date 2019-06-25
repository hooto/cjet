// Copyright 2017 Eryx <evorui аt gmаil dοt cοm>, All rights reserved.
//

#include <iostream>
#include <string.h>
#include "cjet/log.hh"
#include "cjet/conf.hh"
#include "util.hh"

namespace cjet {
namespace conf {

static ConfigCommon cfg;

int Config::Init(const std::string &filename) { return cfg.Init(filename); }

bool Config::Inited() { return cfg.Inited(); }

std::string Config::Value(std::string name) { return cfg.Value(name); }

size_t Config::ValueSize(std::string name, size_t min, size_t max) {

    size_t i = cjet::util::str_to_size_t(cfg.Value(name));
    if (i < min) {
        i = min;
    } else if (i > max) {
        i = max;
    }

    return i;
}

int ConfigCommon::Init(const std::string &filename) {

    if (filename.size() == 0) {
        std::cout << "no config file found\n";
        return 0;
    }

    FILE *fp = fopen(filename.c_str(), "r");
    if (!fp) {
        std::cout << "failed on open config file " << strerror(errno) << "\n";
        return -1;
    }

    filename_ = filename;
    std::string section_open = "main";
    std::string tag_open = "";
    std::string tag_value = "";

    char buf[kConfigFileMaxSize + 1];
    while (fgets(buf, kConfigFileMaxSize, fp)) {

        if (strlen(buf) <= 1) {
            continue;
        }

        std::string line(buf);
        line = cjet::util::str_trim(line, " \t\r\n");

        // std::cout << "line " << line << "\n";

        if (line.size() < 3) {
            continue;
        }

        if (line[0] == kConfigSectionStart &&
            line[line.size() - 1] == kConfigSectionClose) {
            std::string section = line.substr(1, line.size() - 2);
            items_[section] = "enable";
            if (section != section_open) {
                if (tag_open != "") {
                    items_[section_open + "/" + tag_open] = tag_value;
                    tag_open = "";
                    tag_value = "";
                }
                section_open = section;
            }
            continue;
        }

        if (line[0] == '#' && tag_open == "") {
            continue;
        }

        if (line[0] == '%') {
            if (tag_open != "") {
                items_[section_open + "/" + tag_open] = tag_value;
            }

            if (line.size() > 1) {
                tag_open = line.substr(1);
            } else {
                tag_open = "";
            }

            tag_value = "";
            continue;
        }

        if (tag_open != "") {

            if (tag_value.size() > 0) {
                tag_value += "\n";
            }

            tag_value += line;
            continue;
        }

        int eqi = line.find_first_of("=");
        if (eqi > 0) {
            std::string key =
                cjet::util::str_trim(line.substr(0, eqi), " \t\r\n");
            std::string val =
                cjet::util::str_trim(line.substr(eqi + 1), " \t\r\n");
            if (key.size() > 0 && val.size() > 0) {
                items_[section_open + "/" + key] = val;
            }
        }
    }
    fclose(fp);

    if (tag_open != "") {
        items_[section_open + "/" + tag_open] = tag_value;
    }

    for (const auto &v : items_) {
        CJET_LOG("info", "config %s = %s", v.first.c_str(), v.second.c_str());
    }

    inited_ = true;

    return 0;
}

bool ConfigCommon::Inited() { return inited_; }

std::string ConfigCommon::Value(std::string name) {
    if (items_.size() > 0) {
        StringMap::iterator it = items_.find(name);
        if (it != items_.end()) {
            return it->second;
        }
    }
    return "";
}

} // namespace ini
} // namespace cjet
