#ifndef HOSTEL_CMS_STAFF_HPP
#define HOSTEL_CMS_STAFF_HPP

#include "Models/User.hpp"

namespace HostelCMS {
namespace Models {

class Staff : public User {
private:
    std::string department; // Electrical, Plumbing, WiFi, Furniture, Cleaning, etc.
    std::string isAvailable; // "Available", "Busy"

public:
    Staff();
    Staff(std::string id, std::string username, std::string passwordHash,
          std::string name, std::string email, std::string phone,
          std::string department, std::string isAvailable = "Available",
          std::string createdAt = "");

    // Getters & Setters
    std::string getDepartment() const { return department; }
    std::string getIsAvailable() const { return isAvailable; }

    void setDepartment(const std::string& val) { department = val; }
    void setIsAvailable(const std::string& val) { isAvailable = val; }

    // Override pure virtual methods
    void displayDetails() const override;
    std::string getRoleName() const override { return "Maintenance Staff"; }
};

} // namespace Models
} // namespace HostelCMS

#endif // HOSTEL_CMS_STAFF_HPP
