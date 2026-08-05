#ifndef HOSTEL_CMS_STORAGE_ENGINE_HPP
#define HOSTEL_CMS_STORAGE_ENGINE_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "Models/User.hpp"
#include "Models/Student.hpp"
#include "Models/Warden.hpp"
#include "Models/Staff.hpp"
#include "Models/Complaint.hpp"
#include "Models/Feedback.hpp"

namespace HostelCMS {
namespace Database {

class StorageEngine {
private:
    std::string usersFilePath;
    std::string complaintsFilePath;
    std::string feedbackFilePath;

    std::vector<std::shared_ptr<Models::User>> users;
    std::vector<Models::Complaint> complaints;
    std::vector<Models::Feedback> feedbacks;

    std::unordered_map<std::string, std::shared_ptr<Models::User>> userByIdMap;
    std::unordered_map<std::string, std::shared_ptr<Models::User>> userByUsernameMap;
    std::unordered_map<std::string, Models::Complaint> complaintByIdMap;

    void saveUsersToFile();
    void saveComplaintsToFile();
    void saveFeedbacksToFile();

    void loadUsersFromFile();
    void loadComplaintsFromFile();
    void loadFeedbacksFromFile();

    void seedInitialData();

public:
    StorageEngine(const std::string& dataDir = "data");
    ~StorageEngine();

    void initialize();

    // User Operations
    bool addUser(const std::shared_ptr<Models::User>& user);
    std::shared_ptr<Models::User> getUserById(const std::string& id);
    std::shared_ptr<Models::User> getUserByUsername(const std::string& username);
    std::vector<std::shared_ptr<Models::User>> getAllUsers();
    std::vector<std::shared_ptr<Models::User>> getUsersByRole(Models::UserRole role);

    // Complaint Operations
    bool addComplaint(const Models::Complaint& complaint);
    bool updateComplaint(const Models::Complaint& complaint);
    bool deleteComplaint(const std::string& complaintId);
    Models::Complaint getComplaintById(const std::string& complaintId);
    std::vector<Models::Complaint> getAllComplaints();
    std::vector<Models::Complaint> getComplaintsByStudentId(const std::string& studentId);
    std::vector<Models::Complaint> getComplaintsByStaffId(const std::string& staffId);

    // Feedback Operations
    bool addFeedback(const Models::Feedback& feedback);
    Models::Feedback getFeedbackByComplaintId(const std::string& complaintId);
    std::vector<Models::Feedback> getAllFeedbacks();

    // ID Generators
    std::string generateNextUserId(Models::UserRole role);
    std::string generateNextComplaintId();
    std::string generateNextFeedbackId();
};

} // namespace Database
} // namespace HostelCMS

#endif // HOSTEL_CMS_STORAGE_ENGINE_HPP
