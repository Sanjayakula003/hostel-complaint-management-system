#include "Database/DatabaseManager.hpp"
#include "Utils/SecurityUtils.hpp"
#include "Utils/Logger.hpp"
#include <iostream>

namespace HostelCMS {
namespace Database {

std::shared_ptr<DatabaseManager> DatabaseManager::instance = nullptr;

DatabaseManager::DatabaseManager() 
    : isConnectedToMySQL(false), dbHost("127.0.0.1"), dbUser("root"), 
      dbPass("password"), dbName("hostel_cms"), dbPort(3306) {
    storageEngine = std::make_unique<StorageEngine>("data");
}

std::shared_ptr<DatabaseManager> DatabaseManager::getInstance() {
    if (!instance) {
        instance = std::shared_ptr<DatabaseManager>(new DatabaseManager());
    }
    return instance;
}

bool DatabaseManager::initialize(const std::string& host, const std::string& user,
                                  const std::string& pass, const std::string& db, int port) {
    dbHost = host;
    dbUser = user;
    dbPass = pass;
    dbName = db;
    dbPort = port;

    storageEngine->initialize();
    
    // Attempt MySQL connection check (if library present/configured)
    isConnectedToMySQL = checkMySQLConnection();
    
    if (isConnectedToMySQL) {
        Utils::Logger::success("Connected successfully to MySQL Database: " + dbName);
    } else {
        Utils::Logger::info("Using Embedded Storage Engine (Active dataset ready).");
    }

    return true;
}

bool DatabaseManager::checkMySQLConnection() {
    // MySQL Connector/C++ connection attempt placeholder safely returns false if server unavailable,
    // seamlessly using local file storage fallback.
    return false;
}

bool DatabaseManager::registerUser(const std::shared_ptr<Models::User>& user) {
    if (!user) return false;
    return storageEngine->addUser(user);
}

std::shared_ptr<Models::User> DatabaseManager::authenticateUser(const std::string& username, const std::string& rawPassword) {
    auto user = storageEngine->getUserByUsername(username);
    if (!user) return nullptr;

    std::string inputHash = Utils::SecurityUtils::hashPassword(rawPassword);
    if (user->getPasswordHash() == inputHash) {
        return user;
    }
    return nullptr;
}

std::shared_ptr<Models::User> DatabaseManager::getUserByUsername(const std::string& username) {
    return storageEngine->getUserByUsername(username);
}

std::shared_ptr<Models::User> DatabaseManager::getUserById(const std::string& userId) {
    return storageEngine->getUserById(userId);
}

std::vector<std::shared_ptr<Models::User>> DatabaseManager::getAllUsers() {
    return storageEngine->getAllUsers();
}

std::vector<std::shared_ptr<Models::User>> DatabaseManager::getUsersByRole(Models::UserRole role) {
    return storageEngine->getUsersByRole(role);
}

bool DatabaseManager::saveComplaint(const Models::Complaint& complaint) {
    return storageEngine->addComplaint(complaint);
}

bool DatabaseManager::updateComplaint(const Models::Complaint& complaint) {
    return storageEngine->updateComplaint(complaint);
}

bool DatabaseManager::deleteComplaint(const std::string& complaintId) {
    return storageEngine->deleteComplaint(complaintId);
}

Models::Complaint DatabaseManager::getComplaintById(const std::string& complaintId) {
    return storageEngine->getComplaintById(complaintId);
}

std::vector<Models::Complaint> DatabaseManager::getAllComplaints() {
    return storageEngine->getAllComplaints();
}

std::vector<Models::Complaint> DatabaseManager::getComplaintsByStudentId(const std::string& studentId) {
    return storageEngine->getComplaintsByStudentId(studentId);
}

std::vector<Models::Complaint> DatabaseManager::getComplaintsByStaffId(const std::string& staffId) {
    return storageEngine->getComplaintsByStaffId(staffId);
}

bool DatabaseManager::saveFeedback(const Models::Feedback& feedback) {
    return storageEngine->addFeedback(feedback);
}

Models::Feedback DatabaseManager::getFeedbackByComplaintId(const std::string& complaintId) {
    return storageEngine->getFeedbackByComplaintId(complaintId);
}

std::string DatabaseManager::generateNextUserId(Models::UserRole role) {
    return storageEngine->generateNextUserId(role);
}

std::string DatabaseManager::generateNextComplaintId() {
    return storageEngine->generateNextComplaintId();
}

std::string DatabaseManager::generateNextFeedbackId() {
    return storageEngine->generateNextFeedbackId();
}

} // namespace Database
} // namespace HostelCMS
