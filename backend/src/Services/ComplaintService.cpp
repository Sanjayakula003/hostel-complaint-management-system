#include "Services/ComplaintService.hpp"
#include "Database/DatabaseManager.hpp"
#include "Utils/DateTimeUtils.hpp"
#include "Utils/ValidationUtils.hpp"
#include "Utils/Logger.hpp"
#include <algorithm>
#include <cctype>

namespace HostelCMS {
namespace Services {

namespace {
    std::string toLowerStr(const std::string& str) {
        std::string res = str;
        std::transform(res.begin(), res.end(), res.begin(), [](unsigned char c){ return std::tolower(c); });
        return res;
    }
}

ComplaintService::ComplaintService() {
    loadFromDatabase();
}

void ComplaintService::loadFromDatabase() {
    auto db = Database::DatabaseManager::getInstance();
    complaintVector = db->getAllComplaints();
    refreshDSAContainers();
}

void ComplaintService::refreshDSAContainers() {
    complaintMap.clear();
    while (!standardProcessingQueue.empty()) standardProcessingQueue.pop();
    while (!criticalPriorityQueue.empty()) criticalPriorityQueue.pop();

    // Sort complaintVector by ID for binary_search support
    std::sort(complaintVector.begin(), complaintVector.end(),
        [](const Models::Complaint& a, const Models::Complaint& b) {
            return a.getId() < b.getId();
        });

    for (const auto& c : complaintVector) {
        // Fast O(1) Map Lookup index
        complaintMap[c.getId()] = c;

        // Data Structure Queues segregation
        if (c.getStatus() == Models::ComplaintStatus::PENDING || c.getStatus() == Models::ComplaintStatus::ASSIGNED) {
            if (c.getPriority() == Models::PriorityLevel::CRITICAL || c.getPriority() == Models::PriorityLevel::HIGH) {
                criticalPriorityQueue.push(c); // Priority Queue Heap
            } else {
                standardProcessingQueue.push(c); // Standard FIFO Queue
            }
        }
    }
}

DuplicateWarning ComplaintService::checkForDuplicate(const std::string& roomNumber, 
                                                    const std::string& category, 
                                                    double timeWindowHours) {
    DuplicateWarning dw;
    dw.isDuplicateFound = false;
    std::int64_t nowTs = Utils::DateTimeUtils::getCurrentTimestamp();

    for (const auto& c : complaintVector) {
        if (c.getRoomNumber() == roomNumber && c.getCategory() == category) {
            if (c.getStatus() != Models::ComplaintStatus::CLOSED) {
                std::int64_t createdTs = Utils::DateTimeUtils::parseDateTimeString(c.getCreatedAt());
                double hoursDiff = Utils::DateTimeUtils::getDifferenceInHours(createdTs, nowTs);

                if (hoursDiff <= timeWindowHours) {
                    dw.isDuplicateFound = true;
                    dw.existingComplaintId = c.getId();
                    dw.existingStatus = Models::statusToString(c.getStatus());
                    dw.existingCreatedAt = c.getCreatedAt();
                    dw.message = "A complaint of category '" + category + "' for Room " + roomNumber +
                                 " already exists (" + c.getId() + ", Status: " + dw.existingStatus +
                                 ", Registered on: " + c.getCreatedAt() + ").";
                    return dw;
                }
            }
        }
    }
    return dw;
}

bool ComplaintService::registerComplaint(const std::string& studentId, const std::string& studentName,
                                        const std::string& roomNumber, const std::string& category,
                                        const std::string& title, const std::string& description,
                                        std::string& outComplaintId, std::string& outErrorMessage) {
    using namespace Utils;

    if (!ValidationUtils::isNotEmpty(title) || !ValidationUtils::isNotEmpty(description)) {
        outErrorMessage = "Title and Description cannot be empty.";
        return false;
    }

    if (!ValidationUtils::isValidRoomNumber(roomNumber)) {
        outErrorMessage = "Invalid Room Number format.";
        return false;
    }

    auto db = Database::DatabaseManager::getInstance();
    std::string newId = db->generateNextComplaintId();
    Models::PriorityLevel calcPriority = PriorityEngine::calculatePriority(category, title, description);
    std::string nowStr = DateTimeUtils::getCurrentDateTimeString();

    Models::Complaint newComplaint(newId, studentId, studentName, roomNumber, category, title, description,
                                  calcPriority, Models::ComplaintStatus::PENDING, nowStr);

    if (db->saveComplaint(newComplaint)) {
        outComplaintId = newId;
        loadFromDatabase();
        return true;
    } else {
        outErrorMessage = "Failed to save complaint to database.";
        return false;
    }
}

bool ComplaintService::updateComplaintDetails(const std::string& complaintId, const std::string& studentId,
                                            const std::string& newTitle, const std::string& newDescription,
                                            std::string& outErrorMessage) {
    auto c = getComplaintById(complaintId);
    if (c.getId().empty()) {
        outErrorMessage = "Complaint not found with ID: " + complaintId;
        return false;
    }

    if (c.getStudentId() != studentId) {
        outErrorMessage = "Unauthorized! You can only modify your own complaints.";
        return false;
    }

    if (c.getStatus() != Models::ComplaintStatus::PENDING) {
        outErrorMessage = "Cannot edit complaint after processing has started (Current Status: " + Models::statusToString(c.getStatus()) + ").";
        return false;
    }

    c.setTitle(newTitle);
    c.setDescription(newDescription);
    c.setPriority(PriorityEngine::calculatePriority(c.getCategory(), newTitle, newDescription));
    c.setUpdatedAt(Utils::DateTimeUtils::getCurrentDateTimeString());

    auto db = Database::DatabaseManager::getInstance();
    if (db->updateComplaint(c)) {
        loadFromDatabase();
        return true;
    } else {
        outErrorMessage = "Failed to update complaint in database.";
        return false;
    }
}

bool ComplaintService::assignStaff(const std::string& complaintId, const std::string& staffId,
                                  const std::string& staffName, std::string& outErrorMessage) {
    auto c = getComplaintById(complaintId);
    if (c.getId().empty()) {
        outErrorMessage = "Complaint ID not found.";
        return false;
    }

    c.setAssignedStaffId(staffId);
    c.setAssignedStaffName(staffName);
    c.setStatus(Models::ComplaintStatus::ASSIGNED);
    c.setUpdatedAt(Utils::DateTimeUtils::getCurrentDateTimeString());

    auto db = Database::DatabaseManager::getInstance();
    if (db->updateComplaint(c)) {
        loadFromDatabase();
        return true;
    } else {
        outErrorMessage = "Failed to assign staff in database.";
        return false;
    }
}

bool ComplaintService::markInProgress(const std::string& complaintId, const std::string& staffId,
                                     std::string& outErrorMessage) {
    auto c = getComplaintById(complaintId);
    if (c.getId().empty()) {
        outErrorMessage = "Complaint ID not found.";
        return false;
    }

    c.setStatus(Models::ComplaintStatus::IN_PROGRESS);
    c.setUpdatedAt(Utils::DateTimeUtils::getCurrentDateTimeString());

    auto db = Database::DatabaseManager::getInstance();
    if (db->updateComplaint(c)) {
        loadFromDatabase();
        return true;
    } else {
        outErrorMessage = "Failed to update status to In Progress.";
        return false;
    }
}

bool ComplaintService::markCompletedByStaff(const std::string& complaintId, const std::string& staffId,
                                          const std::string& remarks, std::string& outErrorMessage) {
    (void)staffId;
    auto c = getComplaintById(complaintId);
    if (c.getId().empty()) {
        outErrorMessage = "Complaint ID not found.";
        return false;
    }

    if (!Utils::ValidationUtils::isNotEmpty(remarks)) {
        outErrorMessage = "Maintenance remarks are required before marking a complaint as completed.";
        return false;
    }

    if (c.getStatus() == Models::ComplaintStatus::CLOSED) {
        outErrorMessage = "Cannot modify a closed complaint.";
        return false;
    }

    c.setStatus(Models::ComplaintStatus::COMPLETED_BY_STAFF);
    c.setRemarks(remarks);
    c.setUpdatedAt(Utils::DateTimeUtils::getCurrentDateTimeString());

    auto db = Database::DatabaseManager::getInstance();
    if (db->updateComplaint(c)) {
        loadFromDatabase();
        return true;
    } else {
        outErrorMessage = "Failed to update complaint status to Completed by Staff.";
        return false;
    }
}

bool ComplaintService::approveByWarden(const std::string& complaintId, const std::string& wardenId,
                                      std::string& outErrorMessage) {
    (void)wardenId;
    auto c = getComplaintById(complaintId);
    if (c.getId().empty()) {
        outErrorMessage = "Complaint ID not found.";
        return false;
    }

    if (c.getStatus() == Models::ComplaintStatus::CLOSED) {
        outErrorMessage = "Cannot approve an already closed complaint.";
        return false;
    }

    if (c.getStatus() != Models::ComplaintStatus::COMPLETED_BY_STAFF) {
        outErrorMessage = "Only complaints with status 'Completed by Staff' can be approved by the Warden.";
        return false;
    }

    c.setStatus(Models::ComplaintStatus::RESOLVED);
    c.setResolvedAt(Utils::DateTimeUtils::getCurrentDateTimeString());
    c.setUpdatedAt(Utils::DateTimeUtils::getCurrentDateTimeString());

    auto db = Database::DatabaseManager::getInstance();
    if (db->updateComplaint(c)) {
        loadFromDatabase();
        return true;
    } else {
        outErrorMessage = "Failed to approve complaint in database.";
        return false;
    }
}

bool ComplaintService::sendBackByWarden(const std::string& complaintId, const std::string& wardenId,
                                       const std::string& rejectionRemarks, std::string& outErrorMessage) {
    (void)wardenId;
    auto c = getComplaintById(complaintId);
    if (c.getId().empty()) {
        outErrorMessage = "Complaint ID not found.";
        return false;
    }

    if (c.getStatus() != Models::ComplaintStatus::COMPLETED_BY_STAFF) {
        outErrorMessage = "Only complaints with status 'Completed by Staff' can be sent back.";
        return false;
    }

    c.setStatus(Models::ComplaintStatus::ASSIGNED);
    std::string updatedRemarks = c.getRemarks();
    if (Utils::ValidationUtils::isNotEmpty(rejectionRemarks)) {
        updatedRemarks = "Sent Back: " + rejectionRemarks + " | Previous: " + updatedRemarks;
    }
    c.setRemarks(updatedRemarks);
    c.setUpdatedAt(Utils::DateTimeUtils::getCurrentDateTimeString());

    auto db = Database::DatabaseManager::getInstance();
    if (db->updateComplaint(c)) {
        loadFromDatabase();
        return true;
    } else {
        outErrorMessage = "Failed to send back complaint in database.";
        return false;
    }
}

bool ComplaintService::markResolved(const std::string& complaintId, const std::string& staffId,
                                   const std::string& remarks, std::string& outErrorMessage) {
    auto c = getComplaintById(complaintId);
    if (c.getId().empty()) {
        outErrorMessage = "Complaint ID not found.";
        return false;
    }

    c.setStatus(Models::ComplaintStatus::RESOLVED);
    c.setRemarks(remarks);
    c.setResolvedAt(Utils::DateTimeUtils::getCurrentDateTimeString());
    c.setUpdatedAt(Utils::DateTimeUtils::getCurrentDateTimeString());

    auto db = Database::DatabaseManager::getInstance();
    if (db->updateComplaint(c)) {
        loadFromDatabase();
        return true;
    } else {
        outErrorMessage = "Failed to mark complaint resolved in database.";
        return false;
    }
}

bool ComplaintService::verifyAndCloseComplaint(const std::string& complaintId, const std::string& studentId,
                                             int rating, const std::string& comments,
                                             std::string& outErrorMessage) {
    auto c = getComplaintById(complaintId);
    if (c.getId().empty()) {
        outErrorMessage = "Complaint ID not found.";
        return false;
    }

    if (c.getStudentId() != studentId) {
        outErrorMessage = "Unauthorized! Only the student who submitted this complaint can submit feedback.";
        return false;
    }

    if (c.getStatus() == Models::ComplaintStatus::CLOSED) {
        outErrorMessage = "Feedback already submitted. This complaint is already closed.";
        return false;
    }

    if (c.getStatus() != Models::ComplaintStatus::RESOLVED) {
        outErrorMessage = "Complaint must be marked RESOLVED by Warden before submitting feedback.";
        return false;
    }

    auto db = Database::DatabaseManager::getInstance();
    std::string fbId = db->generateNextFeedbackId();
    std::string nowStr = Utils::DateTimeUtils::getCurrentDateTimeString();
    Models::Feedback fb(fbId, complaintId, studentId, rating, comments, nowStr);

    db->saveFeedback(fb);

    c.setStatus(Models::ComplaintStatus::CLOSED);
    c.setFeedback(fb);
    c.setUpdatedAt(nowStr);

    if (db->updateComplaint(c)) {
        loadFromDatabase();
        return true;
    } else {
        outErrorMessage = "Failed to close complaint in database.";
        return false;
    }
}

bool ComplaintService::deleteComplaint(const std::string& complaintId, std::string& outErrorMessage) {
    auto db = Database::DatabaseManager::getInstance();
    if (db->deleteComplaint(complaintId)) {
        loadFromDatabase();
        return true;
    } else {
        outErrorMessage = "Complaint ID not found or could not be deleted.";
        return false;
    }
}

Models::Complaint ComplaintService::getComplaintById(const std::string& complaintId) {
    // 1. Fast O(1) unordered_map Lookup
    auto it = complaintMap.find(complaintId);
    if (it != complaintMap.end()) {
        return it->second;
    }

    // 2. Binary Search on sorted complaintVector (STL Binary Search demonstration)
    Models::Complaint dummy;
    dummy.setId(complaintId);
    auto lb = std::lower_bound(complaintVector.begin(), complaintVector.end(), dummy,
        [](const Models::Complaint& a, const Models::Complaint& b) {
            return a.getId() < b.getId();
        });

    if (lb != complaintVector.end() && lb->getId() == complaintId) {
        return *lb;
    }

    return Models::Complaint();
}

std::vector<Models::Complaint> ComplaintService::getComplaintsByStudent(const std::string& studentId) {
    std::vector<Models::Complaint> res;
    for (const auto& c : complaintVector) {
        if (c.getStudentId() == studentId) {
            res.push_back(c);
        }
    }
    return res;
}

std::vector<Models::Complaint> ComplaintService::getComplaintsByStaff(const std::string& staffId) {
    std::vector<Models::Complaint> res;
    for (const auto& c : complaintVector) {
        if (c.getAssignedStaffId() == staffId) {
            res.push_back(c);
        }
    }
    return res;
}

std::vector<Models::Complaint> ComplaintService::getAllComplaints() {
    return complaintVector;
}

std::vector<Models::Complaint> ComplaintService::searchComplaints(const std::string& keyword,
                                                                 const std::string& category,
                                                                 const std::string& priorityStr,
                                                                 const std::string& statusStr) {
    std::vector<Models::Complaint> filtered;
    std::string kwLower = toLowerStr(keyword);
    std::string catLower = toLowerStr(category);
    std::string prioLower = toLowerStr(priorityStr);
    std::string statLower = toLowerStr(statusStr);

    for (const auto& c : complaintVector) {
        bool match = true;

        if (!kwLower.empty()) {
            std::string combined = toLowerStr(c.getId() + " " + c.getStudentId() + " " + c.getStudentName() +
                                              " " + c.getRoomNumber() + " " + c.getTitle() + " " + c.getDescription());
            if (combined.find(kwLower) == std::string::npos) match = false;
        }

        if (match && !catLower.empty() && catLower != "all") {
            if (toLowerStr(c.getCategory()) != catLower) match = false;
        }

        if (match && !prioLower.empty() && prioLower != "all") {
            if (toLowerStr(Models::priorityToString(c.getPriority())) != prioLower) match = false;
        }

        if (match && !statLower.empty() && statLower != "all") {
            if (toLowerStr(Models::statusToString(c.getStatus())) != statLower) match = false;
        }

        if (match) {
            filtered.push_back(c);
        }
    }

    return filtered;
}

std::queue<Models::Complaint> ComplaintService::getStandardQueue() {
    return standardProcessingQueue;
}

std::priority_queue<Models::Complaint, std::vector<Models::Complaint>, Models::Complaint::PriorityComparator> ComplaintService::getCriticalPriorityQueue() {
    return criticalPriorityQueue;
}

} // namespace Services
} // namespace HostelCMS
