#ifndef HOSTEL_CMS_PRIORITY_ENGINE_HPP
#define HOSTEL_CMS_PRIORITY_ENGINE_HPP

#include <string>
#include "Models/Complaint.hpp"

namespace HostelCMS {
namespace Services {

class PriorityEngine {
public:
    static Models::PriorityLevel calculatePriority(const std::string& category,
                                                    const std::string& title,
                                                    const std::string& description);
    static std::string getPriorityReasoning(const std::string& category,
                                             const std::string& title,
                                             const std::string& description);
};

} // namespace Services
} // namespace HostelCMS

#endif // HOSTEL_CMS_PRIORITY_ENGINE_HPP
