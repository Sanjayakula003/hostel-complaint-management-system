#ifndef HOSTEL_CMS_SECURITY_UTILS_HPP
#define HOSTEL_CMS_SECURITY_UTILS_HPP

#include <string>

namespace HostelCMS {
namespace Utils {

class SecurityUtils {
public:
    static std::string hashPassword(const std::string& password);
    static bool verifyPassword(const std::string& password, const std::string& hash);
    static std::string generateSalt();
    static std::string sanitizeString(const std::string& input);
};

} // namespace Utils
} // namespace HostelCMS

#endif // HOSTEL_CMS_SECURITY_UTILS_HPP
