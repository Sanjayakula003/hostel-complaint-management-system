#include "Database/StorageEngine.hpp"
#include "Utils/SecurityUtils.hpp"
#include "Utils/DateTimeUtils.hpp"
#include "Utils/Logger.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <filesystem>

namespace HostelCMS {
namespace Database {

StorageEngine::StorageEngine(const std::string& dataDir) {
    namespace fs = std::filesystem;
    if (!fs::exists(dataDir)) {
        fs::create_directories(dataDir);
    }
    usersFilePath = dataDir + "/users.csv";
    complaintsFilePath = dataDir + "/complaints.csv";
    feedbackFilePath = dataDir + "/feedback.csv";
}

StorageEngine::~StorageEngine() {
    saveUsersToFile();
    saveComplaintsToFile();
    saveFeedbacksToFile();
}

void StorageEngine::initialize() {
    loadUsersFromFile();
    loadComplaintsFromFile();
    loadFeedbacksFromFile();

    if (users.empty()) {
        seedInitialData();
    }
}

void StorageEngine::seedInitialData() {
    using namespace Models;
    using namespace Utils;

    // Default Passwords (hashed via SecurityUtils)
    std::string defaultHash = SecurityUtils::hashPassword("Pass@123");

    // 1. Wardens
    auto w1 = std::make_shared<Warden>("W101", "warden_rahul", defaultHash, "Dr. Rahul Sharma", "rahul.warden@hostel.edu", "+919876543210", "A", "Warden Office A-101", DateTimeUtils::getCurrentDateTimeString());
    auto w2 = std::make_shared<Warden>("W102", "warden_priya", defaultHash, "Dr. Priya Verma", "priya.warden@hostel.edu", "+919876543211", "B", "Warden Office B-102", DateTimeUtils::getCurrentDateTimeString());

    // 2. Staff
    auto s1 = std::make_shared<Staff>("ST101", "staff_ramesh", defaultHash, "Ramesh Kumar", "ramesh.electric@hostel.edu", "+919123456780", "Electrical", "Available", DateTimeUtils::getCurrentDateTimeString());
    auto s2 = std::make_shared<Staff>("ST102", "staff_suresh", defaultHash, "Suresh Patel", "suresh.plumb@hostel.edu", "+919123456781", "Plumbing", "Available", DateTimeUtils::getCurrentDateTimeString());
    auto s3 = std::make_shared<Staff>("ST103", "staff_anita", defaultHash, "Anita Roy", "anita.wifi@hostel.edu", "+919123456782", "WiFi", "Available", DateTimeUtils::getCurrentDateTimeString());
    auto s4 = std::make_shared<Staff>("ST104", "staff_vikram", defaultHash, "Vikram Singh", "vikram.carpenter@hostel.edu", "+919123456783", "Furniture", "Available", DateTimeUtils::getCurrentDateTimeString());

    // 3. Students
    auto st1 = std::make_shared<Student>("S101", "student_aarav", defaultHash, "Aarav Gupta", "aarav@student.edu", "+919988776655", "CS2026-01", "A-204", "A", DateTimeUtils::getCurrentDateTimeString());
    auto st2 = std::make_shared<Student>("S102", "student_diya", defaultHash, "Diya Sharma", "diya@student.edu", "+919988776656", "EC2026-15", "B-305", "B", DateTimeUtils::getCurrentDateTimeString());
    auto st3 = std::make_shared<Student>("S103", "student_karan", defaultHash, "Karan Mehta", "karan@student.edu", "+919988776657", "ME2026-42", "A-108", "A", DateTimeUtils::getCurrentDateTimeString());

    addUser(w1);
    addUser(w2);
    addUser(s1);
    addUser(s2);
    addUser(s3);
    addUser(s4);
    addUser(st1);
    addUser(st2);
    addUser(st3);

    // 4. Sample Complaints
    Complaint c1("CMP-1001", "S101", "Aarav Gupta", "A-204", "Electrical", "Short Circuit in Switchboard", "Sparks coming out of main switchboard in room.", PriorityLevel::CRITICAL, ComplaintStatus::ASSIGNED, "2026-08-01 10:00:00", "2026-08-01 11:00:00", "", "ST101", "Ramesh Kumar", "Inspecting wiring.");
    Complaint c2("CMP-1002", "S102", "Diya Sharma", "B-305", "Plumbing", "Severe Water Leakage", "Bathroom pipe leaking continuously onto floor.", PriorityLevel::HIGH, ComplaintStatus::PENDING, "2026-08-02 09:30:00", "2026-08-02 09:30:00", "", "", "", "");
    Complaint c3("CMP-1003", "S103", "Karan Mehta", "A-108", "WiFi", "No Internet Connection", "WiFi access point not broadcasting SSID in block A.", PriorityLevel::MEDIUM, ComplaintStatus::IN_PROGRESS, "2026-08-02 14:20:00", "2026-08-03 10:00:00", "", "ST103", "Anita Roy", "Resetting router configuration.");
    Complaint c4("CMP-1004", "S101", "Aarav Gupta", "A-204", "Furniture", "Broken Chair Leg", "Study chair leg cracked and unsafe to sit.", PriorityLevel::LOW, ComplaintStatus::RESOLVED, "2026-07-28 16:00:00", "2026-07-29 12:00:00", "2026-07-29 12:00:00", "ST104", "Vikram Singh", "Replaced with new study chair.");
    Complaint c5("CMP-1005", "S102", "Diya Sharma", "B-305", "Cleaning", "Room Cleaning Request", "Request deep cleaning of corridor and room dustbin.", PriorityLevel::LOW, ComplaintStatus::CLOSED, "2026-07-25 11:00:00", "2026-07-26 15:00:00", "2026-07-26 15:00:00", "ST102", "Suresh Patel", "Cleaning completed.");

    Feedback f4("FB-1001", "CMP-1004", "S101", 5, "Fast resolution and great service by Vikram!", "2026-07-29 14:00:00");
    Feedback f5("FB-1002", "CMP-1005", "S102", 4, "Room is clean now. Thanks.", "2026-07-26 16:30:00");

    c4.setFeedback(f4);
    c5.setFeedback(f5);

    addComplaint(c1);
    addComplaint(c2);
    addComplaint(c3);
    addComplaint(c4);
    addComplaint(c5);

    addFeedback(f4);
    addFeedback(f5);

    saveUsersToFile();
    saveComplaintsToFile();
    saveFeedbacksToFile();
}

bool StorageEngine::addUser(const std::shared_ptr<Models::User>& user) {
    if (!user) return false;
    if (userByUsernameMap.find(user->getUsername()) != userByUsernameMap.end()) {
        return false;
    }
    users.push_back(user);
    userByIdMap[user->getId()] = user;
    userByUsernameMap[user->getUsername()] = user;
    saveUsersToFile();
    return true;
}

std::shared_ptr<Models::User> StorageEngine::getUserById(const std::string& id) {
    auto it = userByIdMap.find(id);
    if (it != userByIdMap.end()) return it->second;
    return nullptr;
}

std::shared_ptr<Models::User> StorageEngine::getUserByUsername(const std::string& username) {
    auto it = userByUsernameMap.find(username);
    if (it != userByUsernameMap.end()) return it->second;
    return nullptr;
}

std::vector<std::shared_ptr<Models::User>> StorageEngine::getAllUsers() {
    return users;
}

std::vector<std::shared_ptr<Models::User>> StorageEngine::getUsersByRole(Models::UserRole role) {
    std::vector<std::shared_ptr<Models::User>> result;
    for (const auto& u : users) {
        if (u->getRole() == role) {
            result.push_back(u);
        }
    }
    return result;
}

bool StorageEngine::addComplaint(const Models::Complaint& complaint) {
    if (complaintByIdMap.find(complaint.getId()) != complaintByIdMap.end()) {
        return false;
    }
    complaints.push_back(complaint);
    complaintByIdMap[complaint.getId()] = complaint;
    saveComplaintsToFile();
    return true;
}

bool StorageEngine::updateComplaint(const Models::Complaint& complaint) {
    auto it = complaintByIdMap.find(complaint.getId());
    if (it == complaintByIdMap.end()) return false;

    it->second = complaint;
    for (auto& c : complaints) {
        if (c.getId() == complaint.getId()) {
            c = complaint;
            break;
        }
    }
    saveComplaintsToFile();
    return true;
}

bool StorageEngine::deleteComplaint(const std::string& complaintId) {
    auto it = complaintByIdMap.find(complaintId);
    if (it == complaintByIdMap.end()) return false;

    complaintByIdMap.erase(it);
    complaints.erase(std::remove_if(complaints.begin(), complaints.end(),
        [&complaintId](const Models::Complaint& c) { return c.getId() == complaintId; }),
        complaints.end());

    saveComplaintsToFile();
    return true;
}

Models::Complaint StorageEngine::getComplaintById(const std::string& complaintId) {
    auto it = complaintByIdMap.find(complaintId);
    if (it != complaintByIdMap.end()) return it->second;
    return Models::Complaint();
}

std::vector<Models::Complaint> StorageEngine::getAllComplaints() {
    return complaints;
}

std::vector<Models::Complaint> StorageEngine::getComplaintsByStudentId(const std::string& studentId) {
    std::vector<Models::Complaint> res;
    for (const auto& c : complaints) {
        if (c.getStudentId() == studentId) {
            res.push_back(c);
        }
    }
    return res;
}

std::vector<Models::Complaint> StorageEngine::getComplaintsByStaffId(const std::string& staffId) {
    std::vector<Models::Complaint> res;
    for (const auto& c : complaints) {
        if (c.getAssignedStaffId() == staffId) {
            res.push_back(c);
        }
    }
    return res;
}

bool StorageEngine::addFeedback(const Models::Feedback& feedback) {
    feedbacks.push_back(feedback);
    auto c = getComplaintById(feedback.getComplaintId());
    if (!c.getId().empty()) {
        c.setFeedback(feedback);
        updateComplaint(c);
    }
    saveFeedbacksToFile();
    return true;
}

Models::Feedback StorageEngine::getFeedbackByComplaintId(const std::string& complaintId) {
    for (const auto& f : feedbacks) {
        if (f.getComplaintId() == complaintId) {
            return f;
        }
    }
    return Models::Feedback();
}

std::vector<Models::Feedback> StorageEngine::getAllFeedbacks() {
    return feedbacks;
}

std::string StorageEngine::generateNextUserId(Models::UserRole role) {
    int maxId = 100;
    std::string prefix = "U";
    if (role == Models::UserRole::STUDENT) prefix = "S";
    else if (role == Models::UserRole::WARDEN) prefix = "W";
    else if (role == Models::UserRole::STAFF) prefix = "ST";

    for (const auto& u : users) {
        if (u->getRole() == role) {
            std::string idStr = u->getId();
            if (idStr.rfind(prefix, 0) == 0) {
                try {
                    int num = std::stoi(idStr.substr(prefix.length()));
                    if (num > maxId) maxId = num;
                } catch (...) {}
            }
        }
    }
    return prefix + std::to_string(maxId + 1);
}

std::string StorageEngine::generateNextComplaintId() {
    int maxId = 1000;
    for (const auto& c : complaints) {
        std::string idStr = c.getId();
        if (idStr.rfind("CMP-", 0) == 0) {
            try {
                int num = std::stoi(idStr.substr(4));
                if (num > maxId) maxId = num;
            } catch (...) {}
        }
    }
    return "CMP-" + std::to_string(maxId + 1);
}

std::string StorageEngine::generateNextFeedbackId() {
    int maxId = 1000;
    for (const auto& f : feedbacks) {
        std::string idStr = f.getId();
        if (idStr.rfind("FB-", 0) == 0) {
            try {
                int num = std::stoi(idStr.substr(3));
                if (num > maxId) maxId = num;
            } catch (...) {}
        }
    }
    return "FB-" + std::to_string(maxId + 1);
}

void StorageEngine::saveUsersToFile() {
    std::ofstream file(usersFilePath);
    if (!file.is_open()) return;
    for (const auto& u : users) {
        file << u->getId() << "|"
             << u->getUsername() << "|"
             << u->getPasswordHash() << "|"
             << u->getName() << "|"
             << u->getEmail() << "|"
             << u->getPhone() << "|"
             << Models::userRoleToString(u->getRole()) << "|";

        if (u->getRole() == Models::UserRole::STUDENT) {
            auto st = std::dynamic_pointer_cast<Models::Student>(u);
            file << st->getRollNumber() << "|" << st->getRoomNumber() << "|" << st->getHostelBlock() << "||";
        } else if (u->getRole() == Models::UserRole::WARDEN) {
            auto w = std::dynamic_pointer_cast<Models::Warden>(u);
            file << "|||" << w->getHostelBlockAssigned() << "|" << w->getOfficeRoom();
        } else if (u->getRole() == Models::UserRole::STAFF) {
            auto s = std::dynamic_pointer_cast<Models::Staff>(u);
            file << "||||" << s->getDepartment() << "|" << s->getIsAvailable();
        }
        file << "|" << u->getCreatedAt() << "\n";
    }
}

void StorageEngine::loadUsersFromFile() {
    std::ifstream file(usersFilePath);
    if (!file.is_open()) return;
    users.clear();
    userByIdMap.clear();
    userByUsernameMap.clear();

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string id, uname, passHash, name, email, phone, roleStr, p1, p2, p3, p4, createdAt;

        std::getline(ss, id, '|');
        std::getline(ss, uname, '|');
        std::getline(ss, passHash, '|');
        std::getline(ss, name, '|');
        std::getline(ss, email, '|');
        std::getline(ss, phone, '|');
        std::getline(ss, roleStr, '|');
        std::getline(ss, p1, '|');
        std::getline(ss, p2, '|');
        std::getline(ss, p3, '|');
        std::getline(ss, p4, '|');
        std::getline(ss, createdAt, '\n');

        Models::UserRole role = Models::stringToUserRole(roleStr);
        if (role == Models::UserRole::STUDENT) {
            auto st = std::make_shared<Models::Student>(id, uname, passHash, name, email, phone, p1, p2, p3, createdAt);
            users.push_back(st);
            userByIdMap[id] = st;
            userByUsernameMap[uname] = st;
        } else if (role == Models::UserRole::WARDEN) {
            auto w = std::make_shared<Models::Warden>(id, uname, passHash, name, email, phone, p3, p4, createdAt);
            users.push_back(w);
            userByIdMap[id] = w;
            userByUsernameMap[uname] = w;
        } else if (role == Models::UserRole::STAFF) {
            auto s = std::make_shared<Models::Staff>(id, uname, passHash, name, email, phone, p3, p4, createdAt);
            users.push_back(s);
            userByIdMap[id] = s;
            userByUsernameMap[uname] = s;
        }
    }
}

void StorageEngine::saveComplaintsToFile() {
    std::ofstream file(complaintsFilePath);
    if (!file.is_open()) return;
    for (const auto& c : complaints) {
        file << c.getId() << "|"
             << c.getStudentId() << "|"
             << c.getStudentName() << "|"
             << c.getRoomNumber() << "|"
             << c.getCategory() << "|"
             << c.getTitle() << "|"
             << c.getDescription() << "|"
             << Models::priorityToString(c.getPriority()) << "|"
             << Models::statusToString(c.getStatus()) << "|"
             << c.getCreatedAt() << "|"
             << c.getUpdatedAt() << "|"
             << c.getResolvedAt() << "|"
             << c.getAssignedStaffId() << "|"
             << c.getAssignedStaffName() << "|"
             << c.getRemarks() << "|"
             << (c.getIsEscalated() ? "1" : "0") << "\n";
    }
}

void StorageEngine::loadComplaintsFromFile() {
    std::ifstream file(complaintsFilePath);
    if (!file.is_open()) return;
    complaints.clear();
    complaintByIdMap.clear();

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string id, stId, stName, room, cat, title, desc, prioStr, statStr, created, updated, resolved, staffId, staffName, remarks, escStr;

        std::getline(ss, id, '|');
        std::getline(ss, stId, '|');
        std::getline(ss, stName, '|');
        std::getline(ss, room, '|');
        std::getline(ss, cat, '|');
        std::getline(ss, title, '|');
        std::getline(ss, desc, '|');
        std::getline(ss, prioStr, '|');
        std::getline(ss, statStr, '|');
        std::getline(ss, created, '|');
        std::getline(ss, updated, '|');
        std::getline(ss, resolved, '|');
        std::getline(ss, staffId, '|');
        std::getline(ss, staffName, '|');
        std::getline(ss, remarks, '|');
        std::getline(ss, escStr, '\n');

        Models::PriorityLevel prio = Models::stringToPriority(prioStr);
        Models::ComplaintStatus stat = Models::stringToStatus(statStr);
        bool esc = (escStr == "1" || escStr == "true");

        Models::Complaint c(id, stId, stName, room, cat, title, desc, prio, stat, created, updated, resolved, staffId, staffName, remarks, esc);
        complaints.push_back(c);
        complaintByIdMap[id] = c;
    }
}

void StorageEngine::saveFeedbacksToFile() {
    std::ofstream file(feedbackFilePath);
    if (!file.is_open()) return;
    for (const auto& f : feedbacks) {
        file << f.getId() << "|"
             << f.getComplaintId() << "|"
             << f.getStudentId() << "|"
             << f.getRating() << "|"
             << f.getComments() << "|"
             << f.getCreatedAt() << "\n";
    }
}

void StorageEngine::loadFeedbacksFromFile() {
    std::ifstream file(feedbackFilePath);
    if (!file.is_open()) return;
    feedbacks.clear();

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string id, cmpId, stId, ratStr, comm, created;

        std::getline(ss, id, '|');
        std::getline(ss, cmpId, '|');
        std::getline(ss, stId, '|');
        std::getline(ss, ratStr, '|');
        std::getline(ss, comm, '|');
        std::getline(ss, created, '\n');

        int rating = 5;
        try { rating = std::stoi(ratStr); } catch (...) {}

        Models::Feedback f(id, cmpId, stId, rating, comm, created);
        feedbacks.push_back(f);
        auto cIt = complaintByIdMap.find(cmpId);
        if (cIt != complaintByIdMap.end()) {
            cIt->second.setFeedback(f);
        }
    }
}

} // namespace Database
} // namespace HostelCMS
