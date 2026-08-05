#ifndef HOSTEL_CMS_DATE_TIME_UTILS_HPP
#define HOSTEL_CMS_DATE_TIME_UTILS_HPP

#include <string>
#include <chrono>

namespace HostelCMS {
namespace Utils {

class DateTimeUtils {
public:
    static std::string getCurrentDateTimeString();
    static std::string getCurrentDateString();
    static std::int64_t getCurrentTimestamp();
    static std::int64_t parseDateTimeString(const std::string& dtStr);
    static double getDifferenceInHours(std::int64_t startTimestamp, std::int64_t endTimestamp);
    static double getDifferenceInDays(std::int64_t startTimestamp, std::int64_t endTimestamp);
    static std::string formatTimestamp(std::int64_t timestamp);
};

} // namespace Utils
} // namespace HostelCMS

#endif // HOSTEL_CMS_DATE_TIME_UTILS_HPP
