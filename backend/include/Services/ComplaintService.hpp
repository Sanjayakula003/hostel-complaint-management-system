#ifndef HOSTEL_CMS_COMPLAINT_SERVICE_HPP
#define HOSTEL_CMS_COMPLAINT_SERVICE_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <memory>
#include "Models/Complaint.hpp"
#include "Models/User.hpp"
#include "Services/PriorityEngine.hpp"

namespace HostelCMS {
namespace Services {

struct DuplicateWarning {
    bool isDuplicateFound;
    std::string existingComplaintId;
    std::string existingStatus;
    std::string existingCreatedAt;
    std::string message;
};

class ComplaintService {
private:
    // STL Data Structures
    std::vector<Models::Complaint> complaintVector;
    std::unordered_map<std::string, Models::Complaint> complaintMap;
    std::queue<Models::Complaint> standardProcessingQueue;
    std::priority_queue<Models::Complaint, std::vector<Models::Complaint>, Models::Complaint::PriorityComparator> criticalPriorityQueue;

    void refreshDSAContainers();

public:
    ComplaintService();

    void loadFromDatabase();

    // Duplicate Check
    DuplicateWarning checkForDuplicate(const std::string& roomNumber, 
                                        const std::string& category, 
                                        double timeWindowHours = 48.0);

    // Complaint Management
    bool registerComplaint(const std::string& studentId, const std::string& studentName,
                           const std::string& roomNumber, const std::string& category,
                           const std::string& title, const std::string& description,
                           std::string& outComplaintId, std::string& outErrorMessage);

    bool updateComplaintDetails(const std::string& complaintId, const std::string& studentId,
                               const std::string& newTitle, const std::string& newDescription,
                               std::string& outErrorMessage);

    bool assignStaff(const std::string& complaintId, const std::string& staffId,
                     const std::string& staffName, std::string& outErrorMessage);

    bool markInProgress(const std::string& complaintId, const std::string& staffId,
                        std::string& outErrorMessage);

    bool markCompletedByStaff(const std::string& complaintId, const std::string& staffId,
                              const std::string& remarks, std::string& outErrorMessage);

    bool approveByWarden(const std::string& complaintId, const std::string& wardenId,
                         std::string& outErrorMessage);

    bool sendBackByWarden(const std::string& complaintId, const std::string& wardenId,
                          const std::string& rejectionRemarks, std::string& outErrorMessage);

    bool markResolved(const std::string& complaintId, const std::string& staffId,
                      const std::string& remarks, std::string& outErrorMessage);

    bool verifyAndCloseComplaint(const std::string& complaintId, const std::string& studentId,
                                int rating, const std::string& comments,
                                std::string& outErrorMessage);

    bool deleteComplaint(const std::string& complaintId, std::string& outErrorMessage);

    // Query Methods using DSA (O(1) Map Lookup, Sorting, Binary Search)
    Models::Complaint getComplaintById(const std::string& complaintId);
    std::vector<Models::Complaint> getComplaintsByStudent(const std::string& studentId);
    std::vector<Models::Complaint> getComplaintsByStaff(const std::string& staffId);
    std::vector<Models::Complaint> getAllComplaints();
    
    // Search & Filter
    std::vector<Models::Complaint> searchComplaints(const std::string& keyword,
                                                   const std::string& category = "",
                                                   const std::string& priorityStr = "",
                                                   const std::string& statusStr = "");

    // DSA Queue Retrievals
    std::queue<Models::Complaint> getStandardQueue();
    std::priority_queue<Models::Complaint, std::vector<Models::Complaint>, Models::Complaint::PriorityComparator> getCriticalPriorityQueue();
};

} // namespace Services
} // namespace HostelCMS

#endif // HOSTEL_CMS_COMPLAINT_SERVICE_HPP
