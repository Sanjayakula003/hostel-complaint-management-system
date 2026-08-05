#include <iostream>
#include "Database/DatabaseManager.hpp"
#include "Api/RestServer.hpp"
#include "Utils/Logger.hpp"

int main() {
    try {
        HostelCMS::Utils::Logger::info("Initializing Hostel Complaint Management System REST Server...");

        // Initialize Database Manager
        auto db = HostelCMS::Database::DatabaseManager::getInstance();
        db->initialize("127.0.0.1", "root", "password", "hostel_cms", 3306);

        // Run REST Server on port 8080
        HostelCMS::Api::RestServer apiServer("0.0.0.0", 8080);
        apiServer.start();

        HostelCMS::Utils::Logger::info("Server shutdown cleanly.");
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Fatal Exception: " << ex.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown Fatal Exception Occurred!" << std::endl;
        return 1;
    }
}
