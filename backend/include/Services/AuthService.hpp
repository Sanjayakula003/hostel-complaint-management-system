#ifndef HOSTEL_CMS_AUTH_SERVICE_HPP
#define HOSTEL_CMS_AUTH_SERVICE_HPP

#include <string>
#include <memory>
#include "Models/User.hpp"
#include "Models/Student.hpp"
#include "Models/Warden.hpp"
#include "Models/Staff.hpp"

namespace HostelCMS {
namespace Services {

class AuthService {
private:
    std::shared_ptr<Models::User> currentUser;

public:
    AuthService();

    bool registerStudent(const std::string& username, const std::string& rawPassword,
                        const std::string& name, const std::string& email,
                        const std::string& phone, const std::string& rollNumber,
                        const std::string& roomNumber, const std::string& hostelBlock,
                        std::string& outErrorMessage);

    bool login(const std::string& username, const std::string& rawPassword,
               Models::UserRole expectedRole, std::string& outErrorMessage);

    void logout();

    bool isLoggedIn() const { return currentUser != nullptr; }
    std::shared_ptr<Models::User> getCurrentUser() const { return currentUser; }
};

} // namespace Services
} // namespace HostelCMS

#endif // HOSTEL_CMS_AUTH_SERVICE_HPP
