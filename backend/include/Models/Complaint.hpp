#ifndef HOSTEL_CMS_COMPLAINT_HPP
#define HOSTEL_CMS_COMPLAINT_HPP

#include <string>
#include <vector>
#include "Models/Feedback.hpp"

namespace HostelCMS {
namespace Models {

enum class PriorityLevel {
    CRITICAL = 4,
    HIGH = 3,
    MEDIUM = 2,
    LOW = 1
};

enum class ComplaintStatus {
    PENDING,
    ASSIGNED,
    IN_PROGRESS,
    COMPLETED_BY_STAFF,
    RESOLVED,
    CLOSED
};

std::string priorityToString(PriorityLevel priority);
PriorityLevel stringToPriority(const std::string& str);

std::string statusToString(ComplaintStatus status);
ComplaintStatus stringToStatus(const std::string& str);

class Complaint {
private:
    std::string id;
    std::string studentId;
    std::string studentName;
    std::string roomNumber;
    std::string category;
    std::string title;
    std::string description;
    PriorityLevel priority;
    ComplaintStatus status;
    std::string createdAt;
    std::string updatedAt;
    std::string resolvedAt;
    std::string assignedStaffId;
    std::string assignedStaffName;
    std::string remarks;
    bool isEscalated;
    Feedback feedback;

public:
    Complaint();
    Complaint(std::string id, std::string studentId, std::string studentName,
              std::string roomNumber, std::string category, std::string title,
              std::string description, PriorityLevel priority, ComplaintStatus status,
              std::string createdAt, std::string updatedAt = "", std::string resolvedAt = "",
              std::string assignedStaffId = "", std::string assignedStaffName = "",
              std::string remarks = "", bool isEscalated = false);

    // Getters & Setters
    std::string getId() const { return id; }
    std::string getStudentId() const { return studentId; }
    std::string getStudentName() const { return studentName; }
    std::string getRoomNumber() const { return roomNumber; }
    std::string getCategory() const { return category; }
    std::string getTitle() const { return title; }
    std::string getDescription() const { return description; }
    PriorityLevel getPriority() const { return priority; }
    ComplaintStatus getStatus() const { return status; }
    std::string getCreatedAt() const { return createdAt; }
    std::string getUpdatedAt() const { return updatedAt; }
    std::string getResolvedAt() const { return resolvedAt; }
    std::string getAssignedStaffId() const { return assignedStaffId; }
    std::string getAssignedStaffName() const { return assignedStaffName; }
    std::string getRemarks() const { return remarks; }
    bool getIsEscalated() const { return isEscalated; }
    Feedback getFeedback() const { return feedback; }

    void setId(const std::string& val) { id = val; }
    void setStudentId(const std::string& val) { studentId = val; }
    void setStudentName(const std::string& val) { studentName = val; }
    void setRoomNumber(const std::string& val) { roomNumber = val; }
    void setCategory(const std::string& val) { category = val; }
    void setTitle(const std::string& val) { title = val; }
    void setDescription(const std::string& val) { description = val; }
    void setPriority(PriorityLevel val) { priority = val; }
    void setStatus(ComplaintStatus val) { status = val; }
    void setCreatedAt(const std::string& val) { createdAt = val; }
    void setUpdatedAt(const std::string& val) { updatedAt = val; }
    void setResolvedAt(const std::string& val) { resolvedAt = val; }
    void setAssignedStaffId(const std::string& val) { assignedStaffId = val; }
    void setAssignedStaffName(const std::string& val) { assignedStaffName = val; }
    void setRemarks(const std::string& val) { remarks = val; }
    void setIsEscalated(bool val) { isEscalated = val; }
    void setFeedback(const Feedback& val) { feedback = val; }

    // Comparator for std::priority_queue (Max-Heap by Priority value, then oldest creation time)
    struct PriorityComparator {
        bool operator()(const Complaint& c1, const Complaint& c2) const {
            if (static_cast<int>(c1.getPriority()) != static_cast<int>(c2.getPriority())) {
                return static_cast<int>(c1.getPriority()) < static_cast<int>(c2.getPriority());
            }
            return c1.getCreatedAt() > c2.getCreatedAt();
        }
    };
};

} // namespace Models
} // namespace HostelCMS

#endif // HOSTEL_CMS_COMPLAINT_HPP
