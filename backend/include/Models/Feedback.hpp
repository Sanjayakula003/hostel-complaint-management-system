#ifndef HOSTEL_CMS_FEEDBACK_HPP
#define HOSTEL_CMS_FEEDBACK_HPP

#include <string>

namespace HostelCMS {
namespace Models {

class Feedback {
private:
    std::string id;
    std::string complaintId;
    std::string studentId;
    int rating; // 1 to 5
    std::string comments;
    std::string createdAt;

public:
    Feedback();
    Feedback(std::string id, std::string complaintId, std::string studentId,
             int rating, std::string comments, std::string createdAt = "");

    // Getters & Setters
    std::string getId() const { return id; }
    std::string getComplaintId() const { return complaintId; }
    std::string getStudentId() const { return studentId; }
    int getRating() const { return rating; }
    std::string getComments() const { return comments; }
    std::string getCreatedAt() const { return createdAt; }

    void setId(const std::string& val) { id = val; }
    void setComplaintId(const std::string& val) { complaintId = val; }
    void setStudentId(const std::string& val) { studentId = val; }
    void setRating(int val) { rating = val; }
    void setComments(const std::string& val) { comments = val; }
    void setCreatedAt(const std::string& val) { createdAt = val; }

    std::string getStarsString() const;
};

} // namespace Models
} // namespace HostelCMS

#endif // HOSTEL_CMS_FEEDBACK_HPP
