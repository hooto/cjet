// Copyright 2017 Eryx <evorui аt gmаil dοt cοm>, All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdarg>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include "fmt/format.h"

#include "cjet/log.hh"
#include "cjet/flag.hh"

namespace cjet {
namespace log {

static Logger log_logger_instance;

Logger::~Logger() {
    if (log_writer != NULL) {
        delete log_writer;
    }
}

int Logger::ready() {

    if (!inited) {

        std::string value = flag::Value("log_dir");
        if (value != "") {
            arg_log_dir = value;
        }

        value = flag::Value("logtostderr");
        if (value == "true") {
            arg_logtostderr = true;
        }

        value = flag::Value("minLogLevel");
        if (value != "") {
            int n = std::atoi(value.c_str());
            if (n >= 0 && n <= level_num_max) {
                arg_minLogLevel = n;
            }
        }

        value = flag::Value("logtolevels");
        if (value == "true") {
            arg_logtolevels = true;
        }

        if (arg_log_dir.size() > 1) {

            auto fname =
                fmt::sprintf("%s/%s.%s.log", arg_log_dir, flag::Program(),
                             timefmt("%Y%m%d_%H%M%S"));
            int fp = ::open(fname.c_str(), O_CREAT | O_WRONLY | O_APPEND, 0644);
            if (fp == -1) {
                std::cout << "Can not open log file " << fname << "\n";
            } else {
                if (log_writer != NULL) {
                    delete log_writer;
                }
                log_writer = new fileWriter(fp);
            }
        }

        inited = true;
    }

    if (!arg_logtostderr && log_writer == NULL) {
        return -1;
    }

    return 0;
}

std::string timefmt(std::string fmtstr) {
    struct timeEntry tn;
    return tn.Format(fmtstr);
}

std::string Logger::str_to_upper(const std::string &str) {
    std::string rs = str;
    for (int i = 0; i < str.size(); i++) {
        rs[i] =
            (str[i] <= 'z' && str[i] >= 'a') ? str[i] - ('a' - 'A') : str[i];
    }
    return rs;
}

int Logger::level_num(std::string level) {

    if (level.size() == 0) {
        return -1;
    }

    level = str_to_upper(level);

    char c = level[0];
    if (c <= 'z' && c >= 'a') {
        c = (c - ('a' - 'A'));
    }

    if (c < 'A' && c > 'Z') {
        return -1;
    }

    for (int i = 0; i < sizeof(level_char); i++) {
        if (c == level_char[i]) {
            return i;
        }
    }

    return -1;
}

std::string Logger::print_raw(const char *fname, int line, int level_num,
                              const char *msg) {
    struct timeEntry tn;
    if (arg_minLogLevel == 0 && line > 0 && sizeof(fname) > 0) {
        return fmt::sprintf("%c %s.%06d %s:%d] %s\n", level_char[level_num],
                            tn.Format("%Y-%m-%d %H:%M:%S"), tn.tv.tv_usec,
                            fname, line, msg);
    }

    return fmt::sprintf("%c %s.%06d %s\n", level_char[level_num],
                        tn.Format("%Y-%m-%d %H:%M:%S"), tn.tv.tv_usec, msg);
}

void Logger::Print(const char *fname, int line, const char *level,
                   const char *msg) {
    if (ready() != 0) {
        return;
    }

    int n = level_num(level);
    if (n == -1) {
        return;
    }

    std::string msgf = print_raw(fname, line, n, msg);
    if (arg_logtostderr) {
        std::cout << msgf;
    }

    if (log_writer != NULL) {
        log_writer->Write(msgf);
    }
}

void Logger::Printf(const char *fname, int line, const char *level,
                    const char *fmtstr, ...) {
    if (ready() != 0) {
        return;
    }

    int n = level_num(level);
    if (n == -1) {
        return;
    }

    char vmsg[log_msg_line_max];

    va_list ap;
    va_start(ap, fmtstr);
    vsnprintf(vmsg, log_msg_line_max, fmtstr, ap);
    va_end(ap);

    std::string msgf = print_raw(fname, line, n, vmsg);
    if (arg_logtostderr) {
        std::cout << msgf;
    }

    if (log_writer != NULL) {
        log_writer->Write(msgf);
    }
}

fileWriter::fileWriter(int fp) {
    m_fp = fp;

    Write(fmt::sprintf("Log file created at %s UTC\n",
                       timefmt("%Y-%m-%d %H:%M:%S")));

    Write(fmt::sprintf("Binary: Built at %s %s\n", __DATE__, __TIME__));

    Write("Log line format: [*] yyyy-mm-dd hh:ii:ss.uuuuuu file:line] msg\n");
}

fileWriter::~fileWriter() {
    if (m_fp > 0) {
        ::close(m_fp);
    }
}

int fileWriter::Write(std::string msg) {
    if (m_fp > 0) {
        if (write(m_fp, msg.c_str(), msg.size()) == -1) {
            std::cout << "write msg error\n";
        }
        m_size += msg.size();
    }
    return 0;
}

//
void Print(const char *fname, int line, const char *level, const char *msg) {
    log_logger_instance.Print(fname, line, level, msg);
}

void Printf(const char *file, int line, const char *level, const char *fmtstr,
            ...) {

    char vmsg[log_msg_line_max];

    va_list ap;
    va_start(ap, fmtstr);
    vsnprintf(vmsg, log_msg_line_max, fmtstr, ap);
    va_end(ap);

    log_logger_instance.Print(file, line, level, vmsg);
}

} // namespace log
} // namespace cjet
