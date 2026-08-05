#include "Services/AuthService.hpp"
#include "Database/DatabaseManager.hpp"
#include "Utils/ValidationUtils.hpp"
#include "Utils/SecurityUtils.hpp"
#include "Utils/DateTimeUtils.hpp"

namespace HostelCMS {
namespace Services {

AuthService::AuthService() : currentUser(nullptr) {}

bool AuthService::registerStudent(const std::string& username, const std::string& rawPassword,
                                   const std::string& name, const std::string& email,
                                   const std::string& phone, const std::string& rollNumber,
                                   const std::string& roomNumber, const std::string& hostelBlock,
                                   std::string& outErrorMessage) {
    using namespace Utils;
    using namespace Database;

    if (!ValidationUtils::isNotEmpty(username) || !ValidationUtils::isNotEmpty(name)) {
        outErrorMessage = "Username and Name cannot be empty.";
        return false;
    }

    if (!ValidationUtils::isValidPassword(rawPassword, outErrorMessage)) {
        return false;
    }

    if (!ValidationUtils::isValidEmail(email)) {
        outErrorMessage = "Invalid email address format.";
        return false;
    }

    if (!ValidationUtils::isValidPhone(phone)) {
        outErrorMessage = "Invalid phone number format.";
        return false;
    }

    if (!ValidationUtils::isValidRoomNumber(roomNumber)) {
        outErrorMessage = "Invalid room number format (e.g., A-101 or B-204).";
        return false;
    }

    if (!ValidationUtils::isValidRollNumber(rollNumber)) {
        outErrorMessage = "Invalid roll number format.";
        return false;
    }

    auto db = DatabaseManager::getInstance();
    if (db->getUserByUsername(username) != nullptr) {
        outErrorMessage = "Username '" + username + "' is already taken.";
        return false;
    }

    std::string userId = db->generateNextUserId(Models::UserRole::STUDENT);
    std::string hash = SecurityUtils::hashPassword(rawPassword);
    std::string nowStr = DateTimeUtils::getCurrentDateTimeString();

    auto newStudent = std::make_shared<Models::Student>(
        userId, username, hash, name, email, phone, rollNumber, roomNumber, hostelBlock, nowStr
    );

    if (db->registerUser(newStudent)) {
        return true;
    } else {
        outErrorMessage = "Failed to save student user to database.";
        return false;
    }
}

bool AuthService::login(const std::string& username, const std::string& rawPassword,
                        Models::UserRole expectedRole, std::string& outErrorMessage) {
    using namespace Database;

    if (!Utils::ValidationUtils::isNotEmpty(username) || !Utils::ValidationUtils::isNotEmpty(rawPassword)) {
        outErrorMessage = "Username and Password are required.";
        return false;
    }

    auto db = DatabaseManager::getInstance();
    auto user = db->authenticateUser(username, rawPassword);

    if (!user) {
        outErrorMessage = "Invalid username or password.";
        return false;
    }

    if (user->getRole() != expectedRole) {
        outErrorMessage = "Role mismatch! User '" + username + "' is not registered as " + Models::userRoleToString(expectedRole) + ".";
        return false;
    }

    currentUser = user;
    return true;
}

void AuthService::logout() {
    currentUser = nullptr;
}

} // namespace Services
} // namespace HostelCMS
