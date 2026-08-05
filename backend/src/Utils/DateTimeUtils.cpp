#include "Utils/DateTimeUtils.hpp"
#include <ctime>
#include <iomanip>
#include <sstream>

namespace HostelCMS {
namespace Utils {

std::string DateTimeUtils::getCurrentDateTimeString() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm tm_buf{};
#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&tm_buf, &now_c);
#else
    localtime_r(&now_c, &tm_buf);
#endif
    std::ostringstream ss;
    ss << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string DateTimeUtils::getCurrentDateString() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm tm_buf{};
#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&tm_buf, &now_c);
#else
    localtime_r(&now_c, &tm_buf);
#endif
    std::ostringstream ss;
    ss << std::put_time(&tm_buf, "%Y-%m-%d");
    return ss.str();
}

std::int64_t DateTimeUtils::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
}

std::int64_t DateTimeUtils::parseDateTimeString(const std::string& dtStr) {
    if (dtStr.empty()) return 0;
    std::tm tm_buf{};
    std::istringstream ss(dtStr);
    ss >> std::get_time(&tm_buf, "%Y-%m-%d %H:%M:%S");
    if (ss.fail()) {
        ss.clear();
        ss.str(dtStr);
        ss >> std::get_time(&tm_buf, "%Y-%m-%d");
        if (ss.fail()) return 0;
    }
    std::time_t time_c = std::mktime(&tm_buf);
    return static_cast<std::int64_t>(time_c);
}

double DateTimeUtils::getDifferenceInHours(std::int64_t startTimestamp, std::int64_t endTimestamp) {
    if (startTimestamp <= 0 || endTimestamp <= 0 || endTimestamp < startTimestamp) {
        return 0.0;
    }
    return static_cast<double>(endTimestamp - startTimestamp) / 3600.0;
}

double DateTimeUtils::getDifferenceInDays(std::int64_t startTimestamp, std::int64_t endTimestamp) {
    if (startTimestamp <= 0 || endTimestamp <= 0 || endTimestamp < startTimestamp) {
        return 0.0;
    }
    return static_cast<double>(endTimestamp - startTimestamp) / 86400.0;
}

std::string DateTimeUtils::formatTimestamp(std::int64_t timestamp) {
    if (timestamp <= 0) return "N/A";
    std::time_t time_c = static_cast<std::time_t>(timestamp);
    std::tm tm_buf{};
#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&tm_buf, &time_c);
#else
    localtime_r(&time_c, &tm_buf);
#endif
    std::ostringstream ss;
    ss << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

} // namespace Utils
} // namespace HostelCMS
