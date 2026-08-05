#ifndef HOSTEL_CMS_LOGGER_HPP
#define HOSTEL_CMS_LOGGER_HPP

#include <string>
#include <iostream>

namespace HostelCMS {
namespace Utils {

enum class LogLevel {
    INFO,
    SUCCESS,
    WARNING,
    ERR,
    DEBUG
};

class Logger {
public:
    static void log(LogLevel level, const std::string& message);
    static void info(const std::string& message);
    static void success(const std::string& message);
    static void warning(const std::string& message);
    static void error(const std::string& message);
    static void debug(const std::string& message);
};

} // namespace Utils
} // namespace HostelCMS

#endif // HOSTEL_CMS_LOGGER_HPP
