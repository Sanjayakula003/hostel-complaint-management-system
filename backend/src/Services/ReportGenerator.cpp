#include "Services/ReportGenerator.hpp"
#include "Database/DatabaseManager.hpp"
#include "Utils/DateTimeUtils.hpp"
#include <sstream>
#include <iomanip>
#include <map>
#include <algorithm>

namespace HostelCMS {
namespace Services {

std::string ReportGenerator::generateTimeBasedReport(const std::vector<Models::Complaint>& complaints, 
                                                    const std::string& timePeriodName, 
                                                    int daysFilter) {
    std::ostringstream ss;
    ss << "========================================================================\n";
    ss << "                    " << timePeriodName << " COMPLAINT SUMMARY REPORT\n";
    ss << "========================================================================\n";

    std::int64_t nowTs = Utils::DateTimeUtils::getCurrentTimestamp();
    int count = 0;
    int resolved = 0;

    for (const auto& c : complaints) {
        bool include = true;
        if (daysFilter > 0) {
            std::int64_t createdTs = Utils::DateTimeUtils::parseDateTimeString(c.getCreatedAt());
            double days = Utils::DateTimeUtils::getDifferenceInDays(createdTs, nowTs);
            if (days > daysFilter) include = false;
        }

        if (include) {
            count++;
            if (c.getStatus() == Models::ComplaintStatus::RESOLVED || c.getStatus() == Models::ComplaintStatus::CLOSED) {
                resolved++;
            }
            ss << std::left << std::setw(10) << c.getId()
               << " | " << std::setw(8) << c.getRoomNumber()
               << " | " << std::setw(12) << c.getCategory()
               << " | " << std::setw(10) << Models::priorityToString(c.getPriority())
               << " | " << std::setw(12) << Models::statusToString(c.getStatus())
               << " | " << c.getCreatedAt() << "\n";
        }
    }

    ss << "------------------------------------------------------------------------\n";
    ss << " Total Complaints: " << count << " | Resolved/Closed: " << resolved
       << " | Pending/Active: " << (count - resolved) << "\n";
    ss << "========================================================================\n";

    return ss.str();
}

std::string ReportGenerator::generateCategoryWiseReport(const std::vector<Models::Complaint>& complaints) {
    std::map<std::string, std::vector<Models::Complaint>> catMap;
    for (const auto& c : complaints) {
        catMap[c.getCategory()].push_back(c);
    }

    std::ostringstream ss;
    ss << "========================================================================\n";
    ss << "                     CATEGORY-WISE COMPLAINT REPORT                     \n";
    ss << "========================================================================\n";

    for (const auto& kv : catMap) {
        ss << "\n[ Category: " << kv.first << " (" << kv.second.size() << " complaints) ]\n";
        ss << "------------------------------------------------------------------------\n";
        for (const auto& c : kv.second) {
            ss << "  " << std::left << std::setw(10) << c.getId()
               << " | Room: " << std::setw(6) << c.getRoomNumber()
               << " | Prio: " << std::setw(8) << Models::priorityToString(c.getPriority())
               << " | Status: " << std::setw(12) << Models::statusToString(c.getStatus())
               << " | Title: " << c.getTitle() << "\n";
        }
    }
    ss << "========================================================================\n";
    return ss.str();
}

std::string ReportGenerator::generatePriorityWiseReport(const std::vector<Models::Complaint>& complaints) {
    std::map<std::string, std::vector<Models::Complaint>> prioMap;
    for (const auto& c : complaints) {
        prioMap[Models::priorityToString(c.getPriority())].push_back(c);
    }

    std::ostringstream ss;
    ss << "========================================================================\n";
    ss << "                     PRIORITY-WISE COMPLAINT REPORT                     \n";
    ss << "========================================================================\n";

    std::vector<std::string> order = {"Critical", "High", "Medium", "Low"};
    for (const auto& pName : order) {
        if (prioMap.find(pName) != prioMap.end()) {
            const auto& list = prioMap[pName];
            ss << "\n[ Priority Level: " << pName << " (" << list.size() << " complaints) ]\n";
            ss << "------------------------------------------------------------------------\n";
            for (const auto& c : list) {
                ss << "  " << std::left << std::setw(10) << c.getId()
                   << " | Category: " << std::setw(12) << c.getCategory()
                   << " | Room: " << std::setw(6) << c.getRoomNumber()
                   << " | Status: " << std::setw(12) << Models::statusToString(c.getStatus())
                   << " | Title: " << c.getTitle() << "\n";
            }
        }
    }
    ss << "========================================================================\n";
    return ss.str();
}

std::string ReportGenerator::generateRoomWiseReport(const std::vector<Models::Complaint>& complaints) {
    std::map<std::string, std::vector<Models::Complaint>> roomMap;
    for (const auto& c : complaints) {
        roomMap[c.getRoomNumber()].push_back(c);
    }

    std::ostringstream ss;
    ss << "========================================================================\n";
    ss << "                       ROOM-WISE COMPLAINT REPORT                       \n";
    ss << "========================================================================\n";

    for (const auto& kv : roomMap) {
        ss << "\n[ Room: " << kv.first << " | Total Complaints: " << kv.second.size() << " ]\n";
        for (const auto& c : kv.second) {
            ss << "  " << std::left << std::setw(10) << c.getId()
               << " | " << std::setw(12) << c.getCategory()
               << " | " << std::setw(8) << Models::priorityToString(c.getPriority())
               << " | " << std::setw(12) << Models::statusToString(c.getStatus())
               << " | " << c.getCreatedAt() << "\n";
        }
    }
    ss << "========================================================================\n";
    return ss.str();
}

std::vector<StaffPerformanceRecord> ReportGenerator::generateStaffPerformanceReport(const std::vector<Models::Complaint>& complaints) {
    auto db = Database::DatabaseManager::getInstance();
    auto staffMembers = db->getUsersByRole(Models::UserRole::STAFF);

    std::vector<StaffPerformanceRecord> records;

    for (const auto& u : staffMembers) {
        auto staff = std::dynamic_pointer_cast<Models::Staff>(u);
        if (!staff) continue;

        StaffPerformanceRecord spr;
        spr.staffId = staff->getId();
        spr.staffName = staff->getName();
        spr.department = staff->getDepartment();
        spr.assignedCount = 0;
        spr.resolvedCount = 0;
        spr.resolutionRatePercentage = 0.0;
        spr.avgResolutionHours = 0.0;
        spr.avgRating = 0.0;

        double totalHours = 0.0;
        double totalRating = 0.0;
        int ratingCount = 0;

        for (const auto& c : complaints) {
            if (c.getAssignedStaffId() == staff->getId()) {
                spr.assignedCount++;
                if (c.getStatus() == Models::ComplaintStatus::RESOLVED || c.getStatus() == Models::ComplaintStatus::CLOSED) {
                    spr.resolvedCount++;

                    if (!c.getCreatedAt().empty() && !c.getResolvedAt().empty()) {
                        std::int64_t startTs = Utils::DateTimeUtils::parseDateTimeString(c.getCreatedAt());
                        std::int64_t endTs = Utils::DateTimeUtils::parseDateTimeString(c.getResolvedAt());
                        if (endTs > startTs) {
                            totalHours += Utils::DateTimeUtils::getDifferenceInHours(startTs, endTs);
                        }
                    }

                    if (c.getFeedback().getRating() > 0) {
                        totalRating += c.getFeedback().getRating();
                        ratingCount++;
                    }
                }
            }
        }

        if (spr.assignedCount > 0) {
            spr.resolutionRatePercentage = (static_cast<double>(spr.resolvedCount) / spr.assignedCount) * 100.0;
        }

        if (spr.resolvedCount > 0) {
            spr.avgResolutionHours = totalHours / spr.resolvedCount;
        }

        if (ratingCount > 0) {
            spr.avgRating = totalRating / ratingCount;
        }

        records.push_back(spr);
    }

    std::sort(records.begin(), records.end(),
        [](const StaffPerformanceRecord& a, const StaffPerformanceRecord& b) {
            return a.resolvedCount > b.resolvedCount;
        });

    return records;
}

} // namespace Services
} // namespace HostelCMS
