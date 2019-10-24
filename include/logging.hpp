#ifndef _LOGGING_H_
#define _LOGGING_H_

#include "main.h"
#include <fstream>

namespace logging {
    constexpr char LOG_FILE_PATH[] = "/usd/motor_log.csv";

    static void clearLogFile() {
        std::ofstream(LOG_FILE_PATH);
    }

    namespace impl {
        static void logHelper(std::ofstream& out) {}

        template<class T, class... Rest>
        void logHelper(std::ofstream& out, T&& value, Rest&&... rest) {
            out << ',' << std::forward<T>(value);
            logHelper(out, std::forward<Rest>(rest)...);
        }
    }

    template<class... Args>
    void log(Args&&... args) {
        std::ofstream out(LOG_FILE_PATH);
        out << pros::millis();
        impl::logHelper(out, std::forward<Args>(args)...);
        out << '\n';
    }
}

#endif
