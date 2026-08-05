#include "Models/User.hpp"

namespace HostelCMS {
namespace Models {

std::string userRoleToString(UserRole role) {
    switch (role) {
        case UserRole::STUDENT: return "Student";
        case UserRole::WARDEN:  return "Warden";
        case UserRole::STAFF:   return "Staff";
        default:                return "Unknown";
    }
}

UserRole stringToUserRole(const std::string& str) {
    if (str == "Student" || str == "STUDENT" || str == "student") return UserRole::STUDENT;
    if (str == "Warden" || str == "WARDEN" || str == "warden") return UserRole::WARDEN;
    if (str == "Staff" || str == "STAFF" || str == "staff") return UserRole::STAFF;
    return UserRole::UNKNOWN;
}

User::User() 
    : id(""), username(""), passwordHash(""), name(""), email(""), phone(""), role(UserRole::UNKNOWN), createdAt("") {}

User::User(std::string id, std::string username, std::string passwordHash,
           std::string name, std::string email, std::string phone, UserRole role,
           std::string createdAt)
    : id(id), username(username), passwordHash(passwordHash),
      name(name), email(email), phone(phone), role(role), createdAt(createdAt) {}

} // namespace Models
} // namespace HostelCMS
