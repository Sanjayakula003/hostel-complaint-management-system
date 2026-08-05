#include "Services/AnalyticsEngine.hpp"
#include "Utils/DateTimeUtils.hpp"
#include <algorithm>
#include <map>

namespace HostelCMS {
namespace Services {

AnalyticsMetrics AnalyticsEngine::generateAnalytics(const std::vector<Models::Complaint>& complaints) {
    AnalyticsMetrics metrics{};
    metrics.totalComplaints = static_cast<int>(complaints.size());

    if (complaints.empty()) {
        metrics.mostCommonCategory = "None";
        return metrics;
    }

    std::unordered_map<std::string, int> categoryCounts;
    std::unordered_map<std::string, int> roomCounts;
    std::map<std::string, std::pair<int, int>> monthlyData; // YYYY-MM -> <total, resolved>
    std::map<std::string, double> monthlyResolutionTimes; // YYYY-MM -> totalHours

    double totalResolutionHours = 0.0;
    int resolvedWithTimeCount = 0;
    double totalRatingSum = 0.0;
    int ratingCount = 0;

    for (const auto& c : complaints) {
        // Status counts
        switch (c.getStatus()) {
            case Models::ComplaintStatus::PENDING:            metrics.pendingCount++; break;
            case Models::ComplaintStatus::ASSIGNED:           metrics.assignedCount++; break;
            case Models::ComplaintStatus::IN_PROGRESS:        metrics.inProgressCount++; break;
            case Models::ComplaintStatus::COMPLETED_BY_STAFF: metrics.inProgressCount++; break;
            case Models::ComplaintStatus::RESOLVED:           metrics.resolvedCount++; break;
            case Models::ComplaintStatus::CLOSED:             metrics.closedCount++; break;
        }

        // Priority counts
        switch (c.getPriority()) {
            case Models::PriorityLevel::CRITICAL: metrics.criticalCount++; break;
            case Models::PriorityLevel::HIGH:     metrics.highCount++; break;
            case Models::PriorityLevel::MEDIUM:   metrics.mediumCount++; break;
            case Models::PriorityLevel::LOW:      metrics.lowCount++; break;
        }

        // Category frequency
        categoryCounts[c.getCategory()]++;

        // Room Hotspots
        if (!c.getRoomNumber().empty()) {
            roomCounts[c.getRoomNumber()]++;
        }

        // Resolution Time
        if (!c.getResolvedAt().empty() && !c.getCreatedAt().empty()) {
            std::int64_t startTs = Utils::DateTimeUtils::parseDateTimeString(c.getCreatedAt());
            std::int64_t endTs = Utils::DateTimeUtils::parseDateTimeString(c.getResolvedAt());
            if (endTs > startTs) {
                double hours = Utils::DateTimeUtils::getDifferenceInHours(startTs, endTs);
                totalResolutionHours += hours;
                resolvedWithTimeCount++;

                std::string monthKey = c.getCreatedAt().substr(0, 7); // YYYY-MM
                monthlyResolutionTimes[monthKey] += hours;
            }
        }

        // Rating
        if (c.getFeedback().getRating() > 0) {
            totalRatingSum += c.getFeedback().getRating();
            ratingCount++;
        }

        // Monthly Stats
        if (c.getCreatedAt().length() >= 7) {
            std::string monthKey = c.getCreatedAt().substr(0, 7);
            monthlyData[monthKey].first++;
            if (c.getStatus() == Models::ComplaintStatus::RESOLVED || c.getStatus() == Models::ComplaintStatus::CLOSED) {
                monthlyData[monthKey].second++;
            }
        }
    }

    if (resolvedWithTimeCount > 0) {
        metrics.averageResolutionTimeHours = totalResolutionHours / resolvedWithTimeCount;
    } else {
        metrics.averageResolutionTimeHours = 0.0;
    }

    if (ratingCount > 0) {
        metrics.averageRating = totalRatingSum / ratingCount;
    } else {
        metrics.averageRating = 0.0;
    }

    // Category Breakdown and Most Common
    int maxCatCount = 0;
    for (const auto& kv : categoryCounts) {
        CategoryStat cs;
        cs.category = kv.first;
        cs.count = kv.second;
        cs.percentage = (static_cast<double>(kv.second) / metrics.totalComplaints) * 100.0;
        metrics.categoryBreakdown.push_back(cs);

        if (kv.second > maxCatCount) {
            maxCatCount = kv.second;
            metrics.mostCommonCategory = kv.first;
        }
    }

    std::sort(metrics.categoryBreakdown.begin(), metrics.categoryBreakdown.end(),
        [](const CategoryStat& a, const CategoryStat& b) {
            return a.count > b.count;
        });

    // Top Room Hotspots
    for (const auto& kv : roomCounts) {
        RoomHotspot rh;
        rh.roomNumber = kv.first;
        rh.complaintCount = kv.second;
        metrics.topHotspotRooms.push_back(rh);
    }

    std::sort(metrics.topHotspotRooms.begin(), metrics.topHotspotRooms.end(),
        [](const RoomHotspot& a, const RoomHotspot& b) {
            return a.complaintCount > b.complaintCount;
        });

    if (metrics.topHotspotRooms.size() > 5) {
        metrics.topHotspotRooms.resize(5);
    }

    // Monthly Statistics
    for (const auto& kv : monthlyData) {
        MonthlyStat ms;
        ms.yearMonth = kv.first;
        ms.totalComplaints = kv.second.first;
        ms.resolvedComplaints = kv.second.second;
        if (ms.resolvedComplaints > 0 && monthlyResolutionTimes.find(kv.first) != monthlyResolutionTimes.end()) {
            ms.avgResolutionHours = monthlyResolutionTimes[kv.first] / ms.resolvedComplaints;
        } else {
            ms.avgResolutionHours = 0.0;
        }
        metrics.monthlyStats.push_back(ms);
    }

    return metrics;
}

} // namespace Services
} // namespace HostelCMS
