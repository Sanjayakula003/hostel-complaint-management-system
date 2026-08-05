#include "Models/Complaint.hpp"

namespace HostelCMS {
namespace Models {

std::string priorityToString(PriorityLevel priority) {
    switch (priority) {
        case PriorityLevel::CRITICAL: return "Critical";
        case PriorityLevel::HIGH:     return "High";
        case PriorityLevel::MEDIUM:   return "Medium";
        case PriorityLevel::LOW:      return "Low";
        default:                      return "Medium";
    }
}

PriorityLevel stringToPriority(const std::string& str) {
    if (str == "Critical" || str == "CRITICAL" || str == "4") return PriorityLevel::CRITICAL;
    if (str == "High" || str == "HIGH" || str == "3") return PriorityLevel::HIGH;
    if (str == "Medium" || str == "MEDIUM" || str == "2") return PriorityLevel::MEDIUM;
    if (str == "Low" || str == "LOW" || str == "1") return PriorityLevel::LOW;
    return PriorityLevel::MEDIUM;
}

std::string statusToString(ComplaintStatus status) {
    switch (status) {
        case ComplaintStatus::PENDING:            return "Pending";
        case ComplaintStatus::ASSIGNED:           return "Assigned";
        case ComplaintStatus::IN_PROGRESS:        return "In Progress";
        case ComplaintStatus::COMPLETED_BY_STAFF: return "Completed by Staff";
        case ComplaintStatus::RESOLVED:           return "Resolved";
        case ComplaintStatus::CLOSED:             return "Closed";
        default:                                  return "Pending";
    }
}

ComplaintStatus stringToStatus(const std::string& str) {
    if (str == "Pending" || str == "PENDING") return ComplaintStatus::PENDING;
    if (str == "Assigned" || str == "ASSIGNED") return ComplaintStatus::ASSIGNED;
    if (str == "In Progress" || str == "IN_PROGRESS" || str == "In-Progress") return ComplaintStatus::IN_PROGRESS;
    if (str == "Completed by Staff" || str == "COMPLETED_BY_STAFF" || str == "Completed") return ComplaintStatus::COMPLETED_BY_STAFF;
    if (str == "Resolved" || str == "RESOLVED") return ComplaintStatus::RESOLVED;
    if (str == "Closed" || str == "CLOSED") return ComplaintStatus::CLOSED;
    return ComplaintStatus::PENDING;
}

Complaint::Complaint()
    : id(""), studentId(""), studentName(""), roomNumber(""), category("Other"),
      title(""), description(""), priority(PriorityLevel::MEDIUM), status(ComplaintStatus::PENDING),
      createdAt(""), updatedAt(""), resolvedAt(""), assignedStaffId(""), assignedStaffName(""),
      remarks(""), isEscalated(false), feedback() {}

Complaint::Complaint(std::string id, std::string studentId, std::string studentName,
                     std::string roomNumber, std::string category, std::string title,
                     std::string description, PriorityLevel priority, ComplaintStatus status,
                     std::string createdAt, std::string updatedAt, std::string resolvedAt,
                     std::string assignedStaffId, std::string assignedStaffName,
                     std::string remarks, bool isEscalated)
    : id(id), studentId(studentId), studentName(studentName), roomNumber(roomNumber),
      category(category), title(title), description(description), priority(priority),
      status(status), createdAt(createdAt), updatedAt(updatedAt), resolvedAt(resolvedAt),
      assignedStaffId(assignedStaffId), assignedStaffName(assignedStaffName),
      remarks(remarks), isEscalated(isEscalated), feedback() {}

} // namespace Models
} // namespace HostelCMS
