#ifndef HOSTEL_CMS_JSON_UTILS_HPP
#define HOSTEL_CMS_JSON_UTILS_HPP

#include "json.hpp"
#include "Models/User.hpp"
#include "Models/Student.hpp"
#include "Models/Warden.hpp"
#include "Models/Staff.hpp"
#include "Models/Complaint.hpp"
#include "Models/Feedback.hpp"
#include "Services/AnalyticsEngine.hpp"
#include "Services/ReportGenerator.hpp"
#include "Services/ComplaintService.hpp"
#include <memory>
#include <vector>

namespace HostelCMS {
namespace Api {

class JsonUtils {
public:
    static Json::Value userToJson(const std::shared_ptr<Models::User>& user) {
        Json::Value obj;
        if (!user) return obj;

        obj.set("id", user->getId());
        obj.set("username", user->getUsername());
        obj.set("name", user->getName());
        obj.set("email", user->getEmail());
        obj.set("phone", user->getPhone());
        obj.set("role", (int)user->getRole());
        obj.set("roleStr", Models::userRoleToString(user->getRole()));

        if (user->getRole() == Models::UserRole::STUDENT) {
            auto student = std::dynamic_pointer_cast<Models::Student>(user);
            if (student) {
                obj.set("rollNumber", student->getRollNumber());
                obj.set("roomNumber", student->getRoomNumber());
                obj.set("hostelBlock", student->getHostelBlock());
            }
        } else if (user->getRole() == Models::UserRole::WARDEN) {
            auto warden = std::dynamic_pointer_cast<Models::Warden>(user);
            if (warden) {
                obj.set("assignedBlock", warden->getHostelBlockAssigned());
                obj.set("officeRoom", warden->getOfficeRoom());
            }
        } else if (user->getRole() == Models::UserRole::STAFF) {
            auto staff = std::dynamic_pointer_cast<Models::Staff>(user);
            if (staff) {
                obj.set("department", staff->getDepartment());
                obj.set("isAvailable", staff->getIsAvailable());
            }
        }
        return obj;
    }

    static Json::Value complaintToJson(const Models::Complaint& c) {
        Json::Value obj;
        obj.set("id", c.getId());
        obj.set("studentId", c.getStudentId());
        obj.set("studentName", c.getStudentName());
        obj.set("roomNumber", c.getRoomNumber());
        obj.set("category", c.getCategory());
        obj.set("title", c.getTitle());
        obj.set("description", c.getDescription());
        obj.set("priority", (int)c.getPriority());
        obj.set("priorityStr", Models::priorityToString(c.getPriority()));
        obj.set("status", (int)c.getStatus());
        obj.set("statusStr", Models::statusToString(c.getStatus()));
        obj.set("assignedStaffId", c.getAssignedStaffId());
        obj.set("assignedStaffName", c.getAssignedStaffName());
        obj.set("createdAt", c.getCreatedAt());
        obj.set("updatedAt", c.getUpdatedAt());
        obj.set("resolvedAt", c.getResolvedAt());
        obj.set("closedAt", c.getStatus() == Models::ComplaintStatus::CLOSED ? c.getResolvedAt() : "");
        obj.set("resolutionRemarks", c.getRemarks());
        obj.set("rating", c.getFeedback().getRating());
        obj.set("feedbackComments", c.getFeedback().getComments());
        return obj;
    }

    static Json::Value complaintVectorToJson(const std::vector<Models::Complaint>& list) {
        Json::Value arr;
        for (const auto& c : list) {
            arr.push(complaintToJson(c));
        }
        return arr;
    }

    static Json::Value feedbackToJson(const Models::Feedback& f) {
        Json::Value obj;
        obj.set("id", f.getId());
        obj.set("complaintId", f.getComplaintId());
        obj.set("studentId", f.getStudentId());
        obj.set("rating", f.getRating());
        obj.set("comments", f.getComments());
        obj.set("createdAt", f.getCreatedAt());
        return obj;
    }

    static Json::Value analyticsToJson(const Services::AnalyticsMetrics& metrics) {
        Json::Value obj;
        obj.set("totalComplaints", metrics.totalComplaints);
        obj.set("pendingCount", metrics.pendingCount);
        obj.set("assignedCount", metrics.assignedCount);
        obj.set("inProgressCount", metrics.inProgressCount);
        obj.set("resolvedCount", metrics.resolvedCount);
        obj.set("closedCount", metrics.closedCount);
        obj.set("criticalCount", metrics.criticalCount);
        obj.set("highCount", metrics.highCount);
        obj.set("mediumCount", metrics.mediumCount);
        obj.set("lowCount", metrics.lowCount);
        obj.set("mostCommonCategory", metrics.mostCommonCategory);
        obj.set("averageResolutionTimeHours", metrics.averageResolutionTimeHours);
        obj.set("averageRating", metrics.averageRating);

        Json::Value categories;
        for (const auto& cat : metrics.categoryBreakdown) {
            Json::Value item;
            item.set("category", cat.category);
            item.set("count", cat.count);
            item.set("percentage", cat.percentage);
            categories.push(item);
        }
        obj.set("categoryBreakdown", categories);

        Json::Value hotspots;
        for (const auto& h : metrics.topHotspotRooms) {
            Json::Value item;
            item.set("roomNumber", h.roomNumber);
            item.set("complaintCount", h.complaintCount);
            hotspots.push(item);
        }
        obj.set("topHotspotRooms", hotspots);

        Json::Value monthly;
        for (const auto& m : metrics.monthlyStats) {
            Json::Value item;
            item.set("yearMonth", m.yearMonth);
            item.set("totalComplaints", m.totalComplaints);
            item.set("resolvedComplaints", m.resolvedComplaints);
            item.set("avgResolutionHours", m.avgResolutionHours);
            monthly.push(item);
        }
        obj.set("monthlyStats", monthly);

        return obj;
    }

    static Json::Value staffPerformanceToJson(const std::vector<Services::StaffPerformanceRecord>& records) {
        Json::Value arr;
        for (const auto& r : records) {
            Json::Value item;
            item.set("staffId", r.staffId);
            item.set("staffName", r.staffName);
            item.set("department", r.department);
            item.set("assignedCount", r.assignedCount);
            item.set("resolvedCount", r.resolvedCount);
            item.set("resolutionRatePercentage", r.resolutionRatePercentage);
            item.set("avgResolutionHours", r.avgResolutionHours);
            item.set("avgRating", r.avgRating);
            arr.push(item);
        }
        return arr;
    }

    static Json::Value duplicateWarningToJson(const Services::DuplicateWarning& dup) {
        Json::Value obj;
        obj.set("isDuplicateFound", dup.isDuplicateFound);
        obj.set("existingComplaintId", dup.existingComplaintId);
        obj.set("existingStatus", dup.existingStatus);
        obj.set("existingCreatedAt", dup.existingCreatedAt);
        obj.set("message", dup.message);
        return obj;
    }
};

} // namespace Api
} // namespace HostelCMS

#endif // HOSTEL_CMS_JSON_UTILS_HPP
