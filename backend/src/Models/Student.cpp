#include "Models/Student.hpp"
#include <iostream>

namespace HostelCMS {
namespace Models {

Student::Student()
    : User(), rollNumber(""), roomNumber(""), hostelBlock("") {
    this->role = UserRole::STUDENT;
}

Student::Student(std::string id, std::string username, std::string passwordHash,
                 std::string name, std::string email, std::string phone,
                 std::string rollNumber, std::string roomNumber, std::string hostelBlock,
                 std::string createdAt)
    : User(id, username, passwordHash, name, email, phone, UserRole::STUDENT, createdAt),
      rollNumber(rollNumber), roomNumber(roomNumber), hostelBlock(hostelBlock) {}

void Student::displayDetails() const {
    std::cout << "--- Student Profile ---" << std::endl;
    std::cout << "ID          : " << id << std::endl;
    std::cout << "Username    : " << username << std::endl;
    std::cout << "Name        : " << name << std::endl;
    std::cout << "Roll No     : " << rollNumber << std::endl;
    std::cout << "Room No     : " << roomNumber << std::endl;
    std::cout << "Hostel Block: " << hostelBlock << std::endl;
    std::cout << "Email       : " << email << std::endl;
    std::cout << "Phone       : " << phone << std::endl;
}

} // namespace Models
} // namespace HostelCMS
