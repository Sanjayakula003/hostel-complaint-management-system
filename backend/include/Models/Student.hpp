#ifndef HOSTEL_CMS_STUDENT_HPP
#define HOSTEL_CMS_STUDENT_HPP

#include "Models/User.hpp"

namespace HostelCMS {
namespace Models {

class Student : public User {
private:
    std::string rollNumber;
    std::string roomNumber;
    std::string hostelBlock;

public:
    Student();
    Student(std::string id, std::string username, std::string passwordHash,
            std::string name, std::string email, std::string phone,
            std::string rollNumber, std::string roomNumber, std::string hostelBlock,
            std::string createdAt = "");

    // Getters & Setters
    std::string getRollNumber() const { return rollNumber; }
    std::string getRoomNumber() const { return roomNumber; }
    std::string getHostelBlock() const { return hostelBlock; }

    void setRollNumber(const std::string& val) { rollNumber = val; }
    void setRoomNumber(const std::string& val) { roomNumber = val; }
    void setHostelBlock(const std::string& val) { hostelBlock = val; }

    // Override pure virtual methods
    void displayDetails() const override;
    std::string getRoleName() const override { return "Student"; }
};

} // namespace Models
} // namespace HostelCMS

#endif // HOSTEL_CMS_STUDENT_HPP
