#include "Models/Staff.hpp"
#include <iostream>

namespace HostelCMS {
namespace Models {

Staff::Staff()
    : User(), department("General"), isAvailable("Available") {
    this->role = UserRole::STAFF;
}

Staff::Staff(std::string id, std::string username, std::string passwordHash,
             std::string name, std::string email, std::string phone,
             std::string department, std::string isAvailable,
             std::string createdAt)
    : User(id, username, passwordHash, name, email, phone, UserRole::STAFF, createdAt),
      department(department), isAvailable(isAvailable) {}

void Staff::displayDetails() const {
    std::cout << "--- Staff Profile ---" << std::endl;
    std::cout << "ID          : " << id << std::endl;
    std::cout << "Username    : " << username << std::endl;
    std::cout << "Name        : " << name << std::endl;
    std::cout << "Department  : " << department << std::endl;
    std::cout << "Status      : " << isAvailable << std::endl;
    std::cout << "Email       : " << email << std::endl;
    std::cout << "Phone       : " << phone << std::endl;
}

} // namespace Models
} // namespace HostelCMS
