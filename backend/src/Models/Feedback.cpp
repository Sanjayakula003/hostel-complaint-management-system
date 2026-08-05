#include "Models/Feedback.hpp"

namespace HostelCMS {
namespace Models {

Feedback::Feedback()
    : id(""), complaintId(""), studentId(""), rating(5), comments(""), createdAt("") {}

Feedback::Feedback(std::string id, std::string complaintId, std::string studentId,
                   int rating, std::string comments, std::string createdAt)
    : id(id), complaintId(complaintId), studentId(studentId),
      rating(rating), comments(comments), createdAt(createdAt) {}

std::string Feedback::getStarsString() const {
    std::string stars = "";
    for (int i = 0; i < rating; ++i) {
        stars += "*";
    }
    return stars + " (" + std::to_string(rating) + "/5)";
}

} // namespace Models
} // namespace HostelCMS
