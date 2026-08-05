#include "Services/PriorityEngine.hpp"
#include <algorithm>
#include <cctype>

namespace HostelCMS {
namespace Services {

namespace {
    std::string toLower(const std::string& str) {
        std::string lower = str;
        std::transform(lower.begin(), lower.end(), lower.begin(),
                       [](unsigned char c){ return std::tolower(c); });
        return lower;
    }

    bool containsAny(const std::string& text, const std::initializer_list<std::string>& keywords) {
        for (const auto& kw : keywords) {
            if (text.find(kw) != std::string::npos) {
                return true;
            }
        }
        return false;
    }
}

Models::PriorityLevel PriorityEngine::calculatePriority(const std::string& category,
                                                         const std::string& title,
                                                         const std::string& description) {
    std::string text = toLower(title + " " + description);
    std::string cat = toLower(category);

    // 1. Critical Priority Rule Checks
    if (containsAny(text, {"short circuit", "fire", "spark", "smoke", "gas leak", "emergency", "theft", "flooding", "water burst", "intruder", "lockout"})) {
        return Models::PriorityLevel::CRITICAL;
    }
    if (cat == "security" && containsAny(text, {"broken lock", "unauthorized", "stolen", "missing"})) {
        return Models::PriorityLevel::CRITICAL;
    }

    // 2. High Priority Rule Checks
    if (cat == "electrical" || cat == "plumbing" || cat == "water supply" || cat == "security") {
        if (containsAny(text, {"leak", "outage", "no water", "no power", "drain block", "shock", "overflow"})) {
            return Models::PriorityLevel::HIGH;
        }
    }
    if (containsAny(text, {"leakage", "water leak", "total wifi outage", "no electricity", "main door"})) {
        return Models::PriorityLevel::HIGH;
    }

    // 3. Medium Priority Rule Checks
    if (cat == "wifi" || cat == "furniture" || cat == "mess") {
        return Models::PriorityLevel::MEDIUM;
    }
    if (containsAny(text, {"fan", "light", "bulb", "socket", "slow internet", "bed", "chair", "table", "window", "food quality"})) {
        return Models::PriorityLevel::MEDIUM;
    }

    // 4. Low Priority Default
    if (cat == "cleaning") {
        return Models::PriorityLevel::LOW;
    }

    return Models::PriorityLevel::LOW;
}

std::string PriorityEngine::getPriorityReasoning(const std::string& category,
                                                  const std::string& title,
                                                  const std::string& description) {
    Models::PriorityLevel prio = calculatePriority(category, title, description);
    switch (prio) {
        case Models::PriorityLevel::CRITICAL:
            return "Assigned CRITICAL priority due to potential hazard or severe impact keyword detection.";
        case Models::PriorityLevel::HIGH:
            return "Assigned HIGH priority due to essential facility disruption (Electrical / Plumbing / Security).";
        case Models::PriorityLevel::MEDIUM:
            return "Assigned MEDIUM priority for standard fixture or amenity repair request.";
        case Models::PriorityLevel::LOW:
            return "Assigned LOW priority for routine housekeeping or non-urgent request.";
    }
    return "Default priority assignment.";
}

} // namespace Services
} // namespace HostelCMS
