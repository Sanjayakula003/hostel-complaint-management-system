#ifndef HOSTEL_CMS_WARDEN_HPP
#define HOSTEL_CMS_WARDEN_HPP

#include "Models/User.hpp"

namespace HostelCMS {
namespace Models {

class Warden : public User {
private:
    std::string hostelBlockAssigned;
    std::string officeRoom;

public:
    Warden();
    Warden(std::string id, std::string username, std::string passwordHash,
           std::string name, std::string email, std::string phone,
           std::string hostelBlockAssigned, std::string officeRoom,
           std::string createdAt = "");

    // Getters & Setters
    std::string getHostelBlockAssigned() const { return hostelBlockAssigned; }
    std::string getOfficeRoom() const { return officeRoom; }

    void setHostelBlockAssigned(const std::string& val) { hostelBlockAssigned = val; }
    void setOfficeRoom(const std::string& val) { officeRoom = val; }

    // Override pure virtual methods
    void displayDetails() const override;
    std::string getRoleName() const override { return "Warden"; }
};

} // namespace Models
} // namespace HostelCMS

#endif // HOSTEL_CMS_WARDEN_HPP
