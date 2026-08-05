#include "Utils/Logger.hpp"

namespace HostelCMS {
namespace Utils {

// ANSI Color Codes
constexpr const char* RESET   = "\033[0m";
constexpr const char* CYAN    = "\033[36m";
constexpr const char* GREEN   = "\033[32m";
constexpr const char* YELLOW  = "\033[33m";
constexpr const char* RED     = "\033[31m";
constexpr const char* MAGENTA = "\033[35m";
constexpr const char* BOLD    = "\033[1m";

void Logger::log(LogLevel level, const std::string& message) {
    switch (level) {
        case LogLevel::INFO:
            std::cout << CYAN << "[INFO] " << RESET << message << std::endl;
            break;
        case LogLevel::SUCCESS:
            std::cout << GREEN << BOLD << "[SUCCESS] " << RESET << GREEN << message << RESET << std::endl;
            break;
        case LogLevel::WARNING:
            std::cout << YELLOW << BOLD << "[WARNING] " << RESET << YELLOW << message << RESET << std::endl;
            break;
        case LogLevel::ERR:
            std::cerr << RED << BOLD << "[ERROR] " << RESET << RED << message << RESET << std::endl;
            break;
        case LogLevel::DEBUG:
            std::cout << MAGENTA << "[DEBUG] " << RESET << message << std::endl;
            break;
    }
}

void Logger::info(const std::string& message) {
    log(LogLevel::INFO, message);
}

void Logger::success(const std::string& message) {
    log(LogLevel::SUCCESS, message);
}

void Logger::warning(const std::string& message) {
    log(LogLevel::WARNING, message);
}

void Logger::error(const std::string& message) {
    log(LogLevel::ERR, message);
}

void Logger::debug(const std::string& message) {
    log(LogLevel::DEBUG, message);
}

} // namespace Utils
} // namespace HostelCMS
