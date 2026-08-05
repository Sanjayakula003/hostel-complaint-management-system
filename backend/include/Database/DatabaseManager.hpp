#ifndef HOSTEL_CMS_DATABASE_MANAGER_HPP
#define HOSTEL_CMS_DATABASE_MANAGER_HPP

#include <string>
#include <vector>
#include <memory>
#include "Database/StorageEngine.hpp"
#include "Models/User.hpp"
#include "Models/Student.hpp"
#include "Models/Warden.hpp"
#include "Models/Staff.hpp"
#include "Models/Complaint.hpp"
#include "Models/Feedback.hpp"

namespace HostelCMS {
namespace Database {

class DatabaseManager {
private:
    static std::shared_ptr<DatabaseManager> instance;
    std::unique_ptr<StorageEngine> storageEngine;
    bool isConnectedToMySQL;
    std::string dbHost;
    std::string dbUser;
    std::string dbPass;
    std::string dbName;
    int dbPort;

    DatabaseManager();

public:
    static std::shared_ptr<DatabaseManager> getInstance();

    bool initialize(const std::string& host = "127.0.0.1", 
                    const std::string& user = "root", 
                    const std::string& pass = "password", 
                    const std::string& db = "hostel_cms", 
                    int port = 3306);

    bool checkMySQLConnection();
    bool isUsingMySQL() const { return isConnectedToMySQL; }
    std::string getStorageModeName() const { return isConnectedToMySQL ? "MySQL 8.0 Engine" : "Local Embedded Engine"; }

    // User CRUD
    bool registerUser(const std::shared_ptr<Models::User>& user);
    std::shared_ptr<Models::User> authenticateUser(const std::string& username, const std::string& rawPassword);
    std::shared_ptr<Models::User> getUserByUsername(const std::string& username);
    std::shared_ptr<Models::User> getUserById(const std::string& userId);
    std::vector<std::shared_ptr<Models::User>> getAllUsers();
    std::vector<std::shared_ptr<Models::User>> getUsersByRole(Models::UserRole role);

    // Complaint CRUD
    bool saveComplaint(const Models::Complaint& complaint);
    bool updateComplaint(const Models::Complaint& complaint);
    bool deleteComplaint(const std::string& complaintId);
    Models::Complaint getComplaintById(const std::string& complaintId);
    std::vector<Models::Complaint> getAllComplaints();
    std::vector<Models::Complaint> getComplaintsByStudentId(const std::string& studentId);
    std::vector<Models::Complaint> getComplaintsByStaffId(const std::string& staffId);

    // Feedback CRUD
    bool saveFeedback(const Models::Feedback& feedback);
    Models::Feedback getFeedbackByComplaintId(const std::string& complaintId);

    // ID Generators
    std::string generateNextUserId(Models::UserRole role);
    std::string generateNextComplaintId();
    std::string generateNextFeedbackId();
};

} // namespace Database
} // namespace HostelCMS

#endif // HOSTEL_CMS_DATABASE_MANAGER_HPP
