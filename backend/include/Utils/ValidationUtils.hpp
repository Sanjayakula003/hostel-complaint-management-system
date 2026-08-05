#ifndef HOSTEL_CMS_VALIDATION_UTILS_HPP
#define HOSTEL_CMS_VALIDATION_UTILS_HPP

#include <string>

namespace HostelCMS {
namespace Utils {

class ValidationUtils {
public:
    static bool isNotEmpty(const std::string& str);
    static bool isValidRoomNumber(const std::string& roomNo);
    static bool isValidPassword(const std::string& password, std::string& outErrorMessage);
    static bool isValidEmail(const std::string& email);
    static bool isValidPhone(const std::string& phone);
    static bool isValidRating(int rating);
    static bool isValidRollNumber(const std::string& rollNo);
};

} // namespace Utils
} // namespace HostelCMS

#endif // HOSTEL_CMS_VALIDATION_UTILS_HPP
