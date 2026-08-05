#ifndef HOSTEL_CMS_REPORT_GENERATOR_HPP
#define HOSTEL_CMS_REPORT_GENERATOR_HPP

#include <string>
#include <vector>
#include "Models/Complaint.hpp"

namespace HostelCMS {
namespace Services {

struct StaffPerformanceRecord {
    std::string staffId;
    std::string staffName;
    std::string department;
    int assignedCount;
    int resolvedCount;
    double resolutionRatePercentage;
    double avgResolutionHours;
    double avgRating;
};

class ReportGenerator {
public:
    static std::string generateTimeBasedReport(const std::vector<Models::Complaint>& complaints, const std::string& timePeriodName, int daysFilter = 0);
    static std::string generateCategoryWiseReport(const std::vector<Models::Complaint>& complaints);
    static std::string generatePriorityWiseReport(const std::vector<Models::Complaint>& complaints);
    static std::string generateRoomWiseReport(const std::vector<Models::Complaint>& complaints);
    static std::vector<StaffPerformanceRecord> generateStaffPerformanceReport(const std::vector<Models::Complaint>& complaints);
};

} // namespace Services
} // namespace HostelCMS

#endif // HOSTEL_CMS_REPORT_GENERATOR_HPP
