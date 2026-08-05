#include "Utils/ValidationUtils.hpp"
#include <cctype>
#include <regex>

namespace HostelCMS {
namespace Utils {

bool ValidationUtils::isNotEmpty(const std::string& str) {
    for (char c : str) {
        if (!std::isspace(static_cast<unsigned char>(c))) {
            return true;
        }
    }
    return false;
}

bool ValidationUtils::isValidRoomNumber(const std::string& roomNo) {
    if (!isNotEmpty(roomNo)) return false;
    // Room number pattern: Block letter optionally followed by dash/space and 3-4 digits (e.g. A-101, B202, H3-104)
    std::regex pattern("^[A-Za-z][0-9A-Za-z\\-]{1,7}$");
    return std::regex_match(roomNo, pattern);
}

bool ValidationUtils::isValidPassword(const std::string& password, std::string& outErrorMessage) {
    if (password.length() < 8) {
        outErrorMessage = "Password must be at least 8 characters long.";
        return false;
    }
    bool hasUpper = false;
    bool hasLower = false;
    bool hasDigit = false;
    bool hasSpecial = false;

    for (char c : password) {
        if (std::isupper(static_cast<unsigned char>(c))) hasUpper = true;
        else if (std::islower(static_cast<unsigned char>(c))) hasLower = true;
        else if (std::isdigit(static_cast<unsigned char>(c))) hasDigit = true;
        else hasSpecial = true;
    }

    if (!hasUpper) {
        outErrorMessage = "Password must contain at least one uppercase letter (A-Z).";
        return false;
    }
    if (!hasLower) {
        outErrorMessage = "Password must contain at least one lowercase letter (a-z).";
        return false;
    }
    if (!hasDigit) {
        outErrorMessage = "Password must contain at least one digit (0-9).";
        return false;
    }
    if (!hasSpecial) {
        outErrorMessage = "Password must contain at least one special character (!@#$%^&*...).";
        return false;
    }

    return true;
}

bool ValidationUtils::isValidEmail(const std::string& email) {
    if (!isNotEmpty(email)) return false;
    std::regex pattern("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    return std::regex_match(email, pattern);
}

bool ValidationUtils::isValidPhone(const std::string& phone) {
    if (!isNotEmpty(phone)) return false;
    std::regex pattern("^\\+?[0-9]{10,14}$");
    return std::regex_match(phone, pattern);
}

bool ValidationUtils::isValidRating(int rating) {
    return rating >= 1 && rating <= 5;
}

bool ValidationUtils::isValidRollNumber(const std::string& rollNo) {
    if (!isNotEmpty(rollNo)) return false;
    std::regex pattern("^[A-Za-z0-9\\-]{3,15}$");
    return std::regex_match(rollNo, pattern);
}

} // namespace Utils
} // namespace HostelCMS
