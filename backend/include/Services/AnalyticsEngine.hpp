#ifndef HOSTEL_CMS_ANALYTICS_ENGINE_HPP
#define HOSTEL_CMS_ANALYTICS_ENGINE_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include "Models/Complaint.hpp"

namespace HostelCMS {
namespace Services {

struct RoomHotspot {
    std::string roomNumber;
    int complaintCount;
};

struct CategoryStat {
    std::string category;
    int count;
    double percentage;
};

struct MonthlyStat {
    std::string yearMonth; // e.g., "2026-07", "2026-08"
    int totalComplaints;
    int resolvedComplaints;
    double avgResolutionHours;
};

struct AnalyticsMetrics {
    int totalComplaints;
    int pendingCount;
    int assignedCount;
    int inProgressCount;
    int resolvedCount;
    int closedCount;
    int criticalCount;
    int highCount;
    int mediumCount;
    int lowCount;
    
    std::string mostCommonCategory;
    double averageResolutionTimeHours;
    double averageRating;

    std::vector<CategoryStat> categoryBreakdown;
    std::vector<RoomHotspot> topHotspotRooms;
    std::vector<MonthlyStat> monthlyStats;
};

class AnalyticsEngine {
public:
    static AnalyticsMetrics generateAnalytics(const std::vector<Models::Complaint>& complaints);
};

} // namespace Services
} // namespace HostelCMS

#endif // HOSTEL_CMS_ANALYTICS_ENGINE_HPP
