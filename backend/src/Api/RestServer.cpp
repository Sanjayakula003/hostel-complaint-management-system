#include "Api/RestServer.hpp"
#include "Api/JsonUtils.hpp"
#include "Utils/Logger.hpp"
#include <iostream>

namespace HostelCMS {
namespace Api {

static int safeStoi(const std::string& str, int defaultValue = 5) {
    if (str.empty()) return defaultValue;
    try {
        return std::stoi(str);
    } catch (...) {
        return defaultValue;
    }
}

RestServer::RestServer(const std::string& hostStr, int portNum)
    : port(portNum), host(hostStr) {
    authService = std::make_shared<Services::AuthService>();
    complaintService = std::make_shared<Services::ComplaintService>();
    dbManager = Database::DatabaseManager::getInstance();

    // Reload complaint service DSA containers
    complaintService->loadFromDatabase();
}

void RestServer::setupRoutes() {
    // Authentication Endpoints
    server.Post("/api/login", [this](const httplib::Request& req, httplib::Response& res) {
        handleLogin(req, res);
    });

    server.Post("/api/register", [this](const httplib::Request& req, httplib::Response& res) {
        handleRegister(req, res);
    });

    // Complaint Endpoints
    server.Get("/api/complaints", [this](const httplib::Request& req, httplib::Response& res) {
        handleGetComplaints(req, res);
    });

    server.Post("/api/complaints/check-duplicate", [this](const httplib::Request& req, httplib::Response& res) {
        handleCheckDuplicate(req, res);
    });

    server.Get("/api/complaints/{id}", [this](const httplib::Request& req, httplib::Response& res) {
        handleGetComplaintById(req, res);
    });

    server.Post("/api/complaints", [this](const httplib::Request& req, httplib::Response& res) {
        handleCreateComplaint(req, res);
    });

    server.Put("/api/complaints/{id}", [this](const httplib::Request& req, httplib::Response& res) {
        handleUpdateComplaint(req, res);
    });

    server.Delete("/api/complaints/{id}", [this](const httplib::Request& req, httplib::Response& res) {
        handleDeleteComplaint(req, res);
    });

    // Dashboard, Analytics & Reports
    server.Get("/api/dashboard", [this](const httplib::Request& req, httplib::Response& res) {
        handleGetDashboard(req, res);
    });

    server.Get("/api/analytics", [this](const httplib::Request& req, httplib::Response& res) {
        handleGetAnalytics(req, res);
    });

    server.Get("/api/reports", [this](const httplib::Request& req, httplib::Response& res) {
        handleGetReports(req, res);
    });

    // Feedback
    server.Post("/api/feedback", [this](const httplib::Request& req, httplib::Response& res) {
        handlePostFeedback(req, res);
    });

    // Staff List
    server.Get("/api/staff", [this](const httplib::Request& req, httplib::Response& res) {
        handleGetStaffList(req, res);
    });
}

void RestServer::handleLogin(const httplib::Request& req, httplib::Response& res) {
    auto data = Json::parse_object_flat(req.body);
    std::string username = data["username"];
    std::string password = data["password"];
    std::string roleStr = data["role"];

    Models::UserRole expectedRole = Models::UserRole::STUDENT;
    if (roleStr == "1" || roleStr == "WARDEN" || roleStr == "warden") expectedRole = Models::UserRole::WARDEN;
    else if (roleStr == "2" || roleStr == "STAFF" || roleStr == "staff") expectedRole = Models::UserRole::STAFF;

    std::string errorMsg;
    if (authService->login(username, password, expectedRole, errorMsg)) {
        auto user = authService->getCurrentUser();
        Json::Value resObj;
        resObj.set("success", true);
        resObj.set("message", "Login successful");
        resObj.set("user", JsonUtils::userToJson(user));
        res.set_content(resObj.dump(), "application/json");
    } else {
        res.status = 401;
        Json::Value resObj;
        resObj.set("success", false);
        resObj.set("error", errorMsg.empty() ? "Invalid username or password" : errorMsg);
        res.set_content(resObj.dump(), "application/json");
    }
}

void RestServer::handleRegister(const httplib::Request& req, httplib::Response& res) {
    auto data = Json::parse_object_flat(req.body);
    std::string username = data["username"];
    std::string password = data["password"];
    std::string name = data["name"];
    std::string email = data["email"];
    std::string phone = data["phone"];
    std::string rollNumber = data["rollNumber"];
    std::string roomNumber = data["roomNumber"];
    std::string hostelBlock = data["hostelBlock"];

    std::string errorMsg;
    if (authService->registerStudent(username, password, name, email, phone, rollNumber, roomNumber, hostelBlock, errorMsg)) {
        Json::Value resObj;
        resObj.set("success", true);
        resObj.set("message", "Student registration successful");
        res.set_content(resObj.dump(), "application/json");
    } else {
        res.status = 400;
        Json::Value resObj;
        resObj.set("success", false);
        resObj.set("error", errorMsg);
        res.set_content(resObj.dump(), "application/json");
    }
}

void RestServer::handleGetComplaints(const httplib::Request& req, httplib::Response& res) {
    // Extract query parameters if present or headers
    auto studentId = req.get_header_value("X-Student-Id");
    auto staffId = req.get_header_value("X-Staff-Id");

    std::vector<Models::Complaint> list;
    if (!studentId.empty()) {
        list = complaintService->getComplaintsByStudent(studentId);
    } else if (!staffId.empty()) {
        list = complaintService->getComplaintsByStaff(staffId);
    } else {
        list = complaintService->getAllComplaints();
    }

    Json::Value resObj;
    resObj.set("success", true);
    resObj.set("complaints", JsonUtils::complaintVectorToJson(list));
    res.set_content(resObj.dump(), "application/json");
}

void RestServer::handleCheckDuplicate(const httplib::Request& req, httplib::Response& res) {
    auto data = Json::parse_object_flat(req.body);
    std::string roomNumber = data["roomNumber"];
    std::string category = data["category"];

    auto dup = complaintService->checkForDuplicate(roomNumber, category, 48.0);
    Json::Value resObj;
    resObj.set("success", true);
    resObj.set("duplicate", JsonUtils::duplicateWarningToJson(dup));
    res.set_content(resObj.dump(), "application/json");
}

void RestServer::handleGetComplaintById(const httplib::Request& req, httplib::Response& res) {
    std::string id = req.matches.count("id") ? req.matches.at("id") : "";
    auto c = complaintService->getComplaintById(id);
    if (!c.getId().empty()) {
        Json::Value resObj;
        resObj.set("success", true);
        resObj.set("complaint", JsonUtils::complaintToJson(c));
        res.set_content(resObj.dump(), "application/json");
    } else {
        res.status = 404;
        Json::Value resObj;
        resObj.set("success", false);
        resObj.set("error", "Complaint not found");
        res.set_content(resObj.dump(), "application/json");
    }
}

void RestServer::handleCreateComplaint(const httplib::Request& req, httplib::Response& res) {
    auto data = Json::parse_object_flat(req.body);
    std::string studentId = data["studentId"];
    std::string studentName = data["studentName"];
    std::string roomNumber = data["roomNumber"];
    std::string category = data["category"];
    std::string title = data["title"];
    std::string description = data["description"];

    std::string outComplaintId;
    std::string errorMsg;
    if (complaintService->registerComplaint(studentId, studentName, roomNumber, category, title, description, outComplaintId, errorMsg)) {
        Json::Value resObj;
        resObj.set("success", true);
        resObj.set("message", "Complaint registered successfully");
        resObj.set("complaintId", outComplaintId);
        res.set_content(resObj.dump(), "application/json");
    } else {
        res.status = 400;
        Json::Value resObj;
        resObj.set("success", false);
        resObj.set("error", errorMsg);
        res.set_content(resObj.dump(), "application/json");
    }
}

void RestServer::handleUpdateComplaint(const httplib::Request& req, httplib::Response& res) {
    std::string id = req.matches.count("id") ? req.matches.at("id") : "";
    auto data = Json::parse_object_flat(req.body);
    std::string action = data["action"];
    std::string errorMsg;
    bool success = false;

    if (action == "assign_staff") {
        std::string staffId = data["staffId"];
        std::string staffName = data["staffName"];
        success = complaintService->assignStaff(id, staffId, staffName, errorMsg);
    } else if (action == "mark_in_progress") {
        std::string staffId = data["staffId"];
        success = complaintService->markInProgress(id, staffId, errorMsg);
    } else if (action == "mark_completed_by_staff") {
        std::string staffId = data["staffId"];
        std::string remarks = data["remarks"];
        success = complaintService->markCompletedByStaff(id, staffId, remarks, errorMsg);
    } else if (action == "approve_warden") {
        std::string wardenId = data["wardenId"];
        success = complaintService->approveByWarden(id, wardenId, errorMsg);
    } else if (action == "send_back_warden") {
        std::string wardenId = data["wardenId"];
        std::string rejectionRemarks = data["rejectionRemarks"];
        success = complaintService->sendBackByWarden(id, wardenId, rejectionRemarks, errorMsg);
    } else if (action == "mark_resolved") {
        std::string staffId = data["staffId"];
        std::string remarks = data["remarks"];
        success = complaintService->markResolved(id, staffId, remarks, errorMsg);
    } else if (action == "close") {
        std::string studentId = data["studentId"];
        int rating = safeStoi(data["rating"], 5);
        std::string comments = data["comments"];
        success = complaintService->verifyAndCloseComplaint(id, studentId, rating, comments, errorMsg);
    } else if (action == "update_details") {
        std::string studentId = data["studentId"];
        std::string title = data["title"];
        std::string description = data["description"];
        success = complaintService->updateComplaintDetails(id, studentId, title, description, errorMsg);
    } else {
        errorMsg = "Invalid action specified";
    }

    if (success) {
        Json::Value resObj;
        resObj.set("success", true);
        resObj.set("message", "Complaint updated successfully");
        res.set_content(resObj.dump(), "application/json");
    } else {
        res.status = 400;
        Json::Value resObj;
        resObj.set("success", false);
        resObj.set("error", errorMsg);
        res.set_content(resObj.dump(), "application/json");
    }
}

void RestServer::handleDeleteComplaint(const httplib::Request& req, httplib::Response& res) {
    std::string id = req.matches.count("id") ? req.matches.at("id") : "";
    std::string errorMsg;
    if (complaintService->deleteComplaint(id, errorMsg)) {
        Json::Value resObj;
        resObj.set("success", true);
        resObj.set("message", "Complaint deleted successfully");
        res.set_content(resObj.dump(), "application/json");
    } else {
        res.status = 400;
        Json::Value resObj;
        resObj.set("success", false);
        resObj.set("error", errorMsg);
        res.set_content(resObj.dump(), "application/json");
    }
}

void RestServer::handleGetDashboard(const httplib::Request& req, httplib::Response& res) {
    (void)req;
    auto allComplaints = complaintService->getAllComplaints();
    auto metrics = Services::AnalyticsEngine::generateAnalytics(allComplaints);

    Json::Value resObj;
    resObj.set("success", true);
    resObj.set("metrics", JsonUtils::analyticsToJson(metrics));
    resObj.set("recentComplaints", JsonUtils::complaintVectorToJson(allComplaints));
    res.set_content(resObj.dump(), "application/json");
}

void RestServer::handleGetAnalytics(const httplib::Request& req, httplib::Response& res) {
    (void)req;
    auto allComplaints = complaintService->getAllComplaints();
    auto metrics = Services::AnalyticsEngine::generateAnalytics(allComplaints);

    Json::Value resObj;
    resObj.set("success", true);
    resObj.set("analytics", JsonUtils::analyticsToJson(metrics));
    res.set_content(resObj.dump(), "application/json");
}

void RestServer::handleGetReports(const httplib::Request& req, httplib::Response& res) {
    (void)req;
    auto allComplaints = complaintService->getAllComplaints();
    auto staffReport = Services::ReportGenerator::generateStaffPerformanceReport(allComplaints);

    Json::Value resObj;
    resObj.set("success", true);
    resObj.set("staffPerformance", JsonUtils::staffPerformanceToJson(staffReport));
    resObj.set("timeReport7Days", Services::ReportGenerator::generateTimeBasedReport(allComplaints, "Last 7 Days", 7));
    resObj.set("categoryReport", Services::ReportGenerator::generateCategoryWiseReport(allComplaints));
    resObj.set("roomReport", Services::ReportGenerator::generateRoomWiseReport(allComplaints));
    res.set_content(resObj.dump(), "application/json");
}

void RestServer::handlePostFeedback(const httplib::Request& req, httplib::Response& res) {
    auto data = Json::parse_object_flat(req.body);
    std::string complaintId = data["complaintId"];
    std::string studentId = data["studentId"];
    int rating = safeStoi(data["rating"], 5);
    std::string comments = data["comments"];

    std::string errorMsg;
    if (complaintService->verifyAndCloseComplaint(complaintId, studentId, rating, comments, errorMsg)) {
        Json::Value resObj;
        resObj.set("success", true);
        resObj.set("message", "Feedback submitted successfully");
        res.set_content(resObj.dump(), "application/json");
    } else {
        res.status = 400;
        Json::Value resObj;
        resObj.set("success", false);
        resObj.set("error", errorMsg);
        res.set_content(resObj.dump(), "application/json");
    }
}

void RestServer::handleGetStaffList(const httplib::Request& req, httplib::Response& res) {
    (void)req;
    auto staffUsers = dbManager->getUsersByRole(Models::UserRole::STAFF);
    Json::Value arr;
    for (const auto& u : staffUsers) {
        arr.push(JsonUtils::userToJson(u));
    }
    Json::Value resObj;
    resObj.set("success", true);
    resObj.set("staffList", arr);
    res.set_content(resObj.dump(), "application/json");
}

void RestServer::start() {
    setupRoutes();
    server.listen(host, port);
}

void RestServer::stop() {
    server.stop();
}

} // namespace Api
} // namespace HostelCMS
