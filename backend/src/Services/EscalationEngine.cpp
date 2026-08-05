#include "Services/EscalationEngine.hpp"
#include "Utils/DateTimeUtils.hpp"
#include "Database/DatabaseManager.hpp"
#include <algorithm>

namespace HostelCMS {
namespace Services {

EscalationEngine::EscalationEngine(double criticalDays, double highDays, 
                                 double mediumDays, double lowDays)
    : criticalThresholdDays(criticalDays), highThresholdDays(highDays),
      mediumThresholdDays(mediumDays), lowThresholdDays(lowDays) {}

bool EscalationEngine::shouldEscalate(const Models::Complaint& complaint, double& outDaysPending) const {
    if (complaint.getStatus() == Models::ComplaintStatus::RESOLVED ||
        complaint.getStatus() == Models::ComplaintStatus::CLOSED) {
        return false;
    }

    std::int64_t createdTs = Utils::DateTimeUtils::parseDateTimeString(complaint.getCreatedAt());
    std::int64_t nowTs = Utils::DateTimeUtils::getCurrentTimestamp();

    if (createdTs <= 0) return false;

    outDaysPending = Utils::DateTimeUtils::getDifferenceInDays(createdTs, nowTs);

    switch (complaint.getPriority()) {
        case Models::PriorityLevel::LOW:
            return outDaysPending >= lowThresholdDays;
        case Models::PriorityLevel::MEDIUM:
            return outDaysPending >= mediumThresholdDays;
        case Models::PriorityLevel::HIGH:
            return outDaysPending >= highThresholdDays;
        case Models::PriorityLevel::CRITICAL:
            return outDaysPending >= criticalThresholdDays && !complaint.getIsEscalated();
    }

    return false;
}

Models::PriorityLevel EscalationEngine::getNextPriority(Models::PriorityLevel currentPrio) const {
    switch (currentPrio) {
        case Models::PriorityLevel::LOW:      return Models::PriorityLevel::MEDIUM;
        case Models::PriorityLevel::MEDIUM:   return Models::PriorityLevel::HIGH;
        case Models::PriorityLevel::HIGH:     return Models::PriorityLevel::CRITICAL;
        case Models::PriorityLevel::CRITICAL: return Models::PriorityLevel::CRITICAL;
    }
    return Models::PriorityLevel::CRITICAL;
}

std::vector<EscalationRecord> EscalationEngine::checkAndEscalateComplaints(std::vector<Models::Complaint>& complaints) {
    std::vector<EscalationRecord> report;
    auto db = Database::DatabaseManager::getInstance();

    for (auto& c : complaints) {
        double daysPending = 0.0;
        if (shouldEscalate(c, daysPending)) {
            Models::PriorityLevel oldPrio = c.getPriority();
            Models::PriorityLevel newPrio = getNextPriority(oldPrio);

            c.setPriority(newPrio);
            c.setIsEscalated(true);
            c.setUpdatedAt(Utils::DateTimeUtils::getCurrentDateTimeString());

            db->updateComplaint(c);

            EscalationRecord rec;
            rec.complaintId = c.getId();
            rec.roomNumber = c.getRoomNumber();
            rec.category = c.getCategory();
            rec.oldPriority = oldPrio;
            rec.newPriority = newPrio;
            rec.daysPending = daysPending;
            rec.escalationDate = Utils::DateTimeUtils::getCurrentDateTimeString();

            report.push_back(rec);
        }
    }

    return report;
}

} // namespace Services
} // namespace HostelCMS
