#ifndef HOSTEL_CMS_REST_SERVER_HPP
#define HOSTEL_CMS_REST_SERVER_HPP

#include "httplib.hpp"
#include "Services/AuthService.hpp"
#include "Services/ComplaintService.hpp"
#include "Services/AnalyticsEngine.hpp"
#include "Services/ReportGenerator.hpp"
#include "Database/DatabaseManager.hpp"
#include <memory>
#include <string>

namespace HostelCMS {
namespace Api {

class RestServer {
private:
    httplib::Server server;
    std::shared_ptr<Services::AuthService> authService;
    std::shared_ptr<Services::ComplaintService> complaintService;
    std::shared_ptr<Database::DatabaseManager> dbManager;
    int port;
    std::string host;

    void setupRoutes();

    // Endpoints
    void handleLogin(const httplib::Request& req, httplib::Response& res);
    void handleRegister(const httplib::Request& req, httplib::Response& res);
    void handleGetComplaints(const httplib::Request& req, httplib::Response& res);
    void handleGetComplaintById(const httplib::Request& req, httplib::Response& res);
    void handleCreateComplaint(const httplib::Request& req, httplib::Response& res);
    void handleUpdateComplaint(const httplib::Request& req, httplib::Response& res);
    void handleDeleteComplaint(const httplib::Request& req, httplib::Response& res);
    void handleGetDashboard(const httplib::Request& req, httplib::Response& res);
    void handleGetAnalytics(const httplib::Request& req, httplib::Response& res);
    void handleGetReports(const httplib::Request& req, httplib::Response& res);
    void handlePostFeedback(const httplib::Request& req, httplib::Response& res);
    void handleGetStaffList(const httplib::Request& req, httplib::Response& res);
    void handleCheckDuplicate(const httplib::Request& req, httplib::Response& res);

public:
    RestServer(const std::string& host = "0.0.0.0", int port = 8080);
    ~RestServer() = default;

    void start();
    void stop();
};

} // namespace Api
} // namespace HostelCMS

#endif // HOSTEL_CMS_REST_SERVER_HPP
