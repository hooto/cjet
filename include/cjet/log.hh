// Copyright 2017 Eryx <evorui аt gmаil dοt cοm>, All rights reserved.
//

#ifndef CJET_LOG_HH
#define CJET_LOG_HH

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <ctime>
#include <cstdarg>
#include <sys/time.h>
#include "fmt/format.h"
#include "fmt/time.h"

#define cjet_log_print(level, msg) \
    cjet::log::Print(__FILE__, __LINE__, level, msg)

#define cjet_log_printf(level, fmt, args...) \
    cjet::log::Printf(__FILE__, __LINE__, level, fmt, ##args)

#define CJET_LOG(level, fmt, args...) \
    cjet::log::Printf(__FILE__, __LINE__, level, fmt, ##args)

namespace cjet {
namespace log {

static const int log_msg_line_max = 200;

typedef struct timeEntry {
    ::timeval tv;
    timeEntry() {
        ::gettimeofday(&tv, NULL);
    };
    std::string Format(std::string fmtstr) {
        return fmt::format("{:" + fmtstr + "}", *::gmtime(&tv.tv_sec));
    };
} timeEntry;

extern std::string timefmt(std::string fmtstr);

class fileWriter;
class Logger {
   private:
    static const int level_cap = 10;
    std::string levels[level_cap] = {"DEBUG", "INFO", "WARN", "ERROR", "FATAL"};
    std::string level_char = "DIWEF";
    int level_num_max = 5;

    std::string arg_log_dir = "";
    bool arg_logtostderr = false;
    int arg_minLogLevel = 1;
    bool arg_logtolevels = false;

    // typedef std::unordered_map<std::string, int> item_map;
    // static item_map levelMap;
    // static item_map levelOut;
    // extern int item_map_get(item_map& map, std::string name);
    // static std::vector<std::string> log_bufs;

    size_t file_max_size = 1024 * 1024 * 1024;

    fileWriter* log_writer;
    bool inited = false;

    int level_num(std::string level);
    inline std::string str_to_upper(const std::string& str);
    int ready();
    std::string print_raw(const char* file, int line, int level_num,
                          const char* msg);

   public:
    Logger() : inited(false) {};
    ~Logger();
    void Print(const char* file, int line, const char* level, const char* msg);
    void Printf(const char* file, int line, const char* level,
                const char* fmtstr, ...);
};

class fileWriter {
   private:
    int m_fp;
    size_t m_size;
    std::string m_severity; // TODO

   public:
    fileWriter(int fp);
    ~fileWriter();
    int Write(std::string msg);
};

extern void Print(const char* file, int line, const char* level,
                  const char* msg);
extern void Printf(const char* file, int line, const char* level,
                   const char* fmt, ...);

} // namespace log
} // namespace cjet

#endif
