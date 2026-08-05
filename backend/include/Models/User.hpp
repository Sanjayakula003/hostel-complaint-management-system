#ifndef HOSTEL_CMS_USER_HPP
#define HOSTEL_CMS_USER_HPP

#include <string>
#include <iostream>

namespace HostelCMS {
namespace Models {

enum class UserRole {
    STUDENT,
    WARDEN,
    STAFF,
    UNKNOWN
};

std::string userRoleToString(UserRole role);
UserRole stringToUserRole(const std::string& str);

class User {
protected:
    std::string id;
    std::string username;
    std::string passwordHash;
    std::string name;
    std::string email;
    std::string phone;
    UserRole role;
    std::string createdAt;

public:
    User();
    User(std::string id, std::string username, std::string passwordHash,
         std::string name, std::string email, std::string phone, UserRole role,
         std::string createdAt = "");
    virtual ~User() = default;

    // Getters
    std::string getId() const { return id; }
    std::string getUsername() const { return username; }
    std::string getPasswordHash() const { return passwordHash; }
    std::string getName() const { return name; }
    std::string getEmail() const { return email; }
    std::string getPhone() const { return phone; }
    UserRole getRole() const { return role; }
    std::string getCreatedAt() const { return createdAt; }

    // Setters
    void setId(const std::string& val) { id = val; }
    void setUsername(const std::string& val) { username = val; }
    void setPasswordHash(const std::string& val) { passwordHash = val; }
    void setName(const std::string& val) { name = val; }
    void setEmail(const std::string& val) { email = val; }
    void setPhone(const std::string& val) { phone = val; }
    void setRole(UserRole val) { role = val; }
    void setCreatedAt(const std::string& val) { createdAt = val; }

    // Pure virtual function for OOP Polymorphism
    virtual void displayDetails() const = 0;
    virtual std::string getRoleName() const = 0;
};

} // namespace Models
} // namespace HostelCMS

#endif // HOSTEL_CMS_USER_HPP
