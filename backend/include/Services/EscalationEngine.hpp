#ifndef HOSTEL_CMS_ESCALATION_ENGINE_HPP
#define HOSTEL_CMS_ESCALATION_ENGINE_HPP

#include <vector>
#include <string>
#include "Models/Complaint.hpp"

namespace HostelCMS {
namespace Services {

struct EscalationRecord {
    std::string complaintId;
    std::string roomNumber;
    std::string category;
    Models::PriorityLevel oldPriority;
    Models::PriorityLevel newPriority;
    double daysPending;
    std::string escalationDate;
};

class EscalationEngine {
private:
    double criticalThresholdDays;
    double highThresholdDays;
    double mediumThresholdDays;
    double lowThresholdDays;

public:
    EscalationEngine(double criticalDays = 1.0, double highDays = 2.0, 
                     double mediumDays = 3.0, double lowDays = 5.0);

    std::vector<EscalationRecord> checkAndEscalateComplaints(std::vector<Models::Complaint>& complaints);
    bool shouldEscalate(const Models::Complaint& complaint, double& outDaysPending) const;
    Models::PriorityLevel getNextPriority(Models::PriorityLevel currentPrio) const;
};

} // namespace Services
} // namespace HostelCMS

#endif // HOSTEL_CMS_ESCALATION_ENGINE_HPP
