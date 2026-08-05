#include "Models/Warden.hpp"
#include <iostream>

namespace HostelCMS {
namespace Models {

Warden::Warden()
    : User(), hostelBlockAssigned(""), officeRoom("") {
    this->role = UserRole::WARDEN;
}

Warden::Warden(std::string id, std::string username, std::string passwordHash,
               std::string name, std::string email, std::string phone,
               std::string hostelBlockAssigned, std::string officeRoom,
               std::string createdAt)
    : User(id, username, passwordHash, name, email, phone, UserRole::WARDEN, createdAt),
      hostelBlockAssigned(hostelBlockAssigned), officeRoom(officeRoom) {}

void Warden::displayDetails() const {
    std::cout << "--- Warden Profile ---" << std::endl;
    std::cout << "ID          : " << id << std::endl;
    std::cout << "Username    : " << username << std::endl;
    std::cout << "Name        : " << name << std::endl;
    std::cout << "Assigned    : Block " << hostelBlockAssigned << std::endl;
    std::cout << "Office Room : " << officeRoom << std::endl;
    std::cout << "Email       : " << email << std::endl;
    std::cout << "Phone       : " << phone << std::endl;
}

} // namespace Models
} // namespace HostelCMS
