import React from 'react';
import { X } from 'lucide-react';

export const ComplaintModal = ({ complaint, onClose }) => {
  if (!complaint) return null;

  return (
    <div
      className="modal show d-block"
      tabIndex="-1"
      style={{ backgroundColor: 'rgba(0,0,0,0.7)' }}
    >
      <div className="modal-dialog modal-dialog-centered modal-lg">
        <div className="modal-content glass-card p-4">
          <div className="d-flex justify-content-between align-items-center mb-3">
            <h5 className="modal-title fw-bold text-white">
              Complaint Ticket #{complaint.id}
            </h5>

            <button
              type="button"
              className="btn-close btn-close-white"
              onClick={onClose}
            ></button>
          </div>

          <div className="modal-body text-white">
            <div className="row g-3 mb-4">
              <div className="col-md-6">
                <div className="p-3 rounded-3 bg-dark bg-opacity-50 border border-secondary border-opacity-25">
                  <span className="text-secondary small d-block">
                    Student Info
                  </span>

                  <strong className="fs-5">
                    {complaint.studentName}
                  </strong>

                  <div className="text-white-50 mt-1">
                    Room: {complaint.roomNumber}
                  </div>
                </div>
              </div>

              <div className="col-md-6">
                <div className="p-3 rounded-3 bg-dark bg-opacity-50 border border-secondary border-opacity-25">
                  <span className="text-secondary small d-block">
                    Category & Priority
                  </span>

                  <div className="fw-semibold text-info">
                    {complaint.category}
                  </div>

                  <div className="mt-1">
                    Priority: <strong>{complaint.priorityStr}</strong>
                    {" | "}
                    Status: <strong>{complaint.statusStr}</strong>
                  </div>
                </div>
              </div>
            </div>

            <div className="mb-4">
              <h6 className="fw-bold text-info">Title</h6>
              <p className="fs-5">{complaint.title}</p>
            </div>

            <div className="mb-4">
              <h6 className="fw-bold text-info">Description</h6>

              <div className="p-3 rounded-3 bg-dark bg-opacity-50 border border-secondary border-opacity-25">
                {complaint.description}
              </div>
            </div>

            {complaint.assignedStaffName && (
              <div className="mb-4">
                <h6 className="fw-bold text-info">
                  Assigned Maintenance Staff
                </h6>

                <div className="p-3 rounded-3 bg-dark bg-opacity-50 border border-secondary border-opacity-25">
                  <strong>{complaint.assignedStaffName}</strong>
                </div>
              </div>
            )}

            {complaint.resolutionRemarks && (
              <div className="mb-4">
                <h6 className="fw-bold text-success">
                  Resolution Remarks
                </h6>

                <div className="p-3 rounded-3 bg-success bg-opacity-10 border border-success border-opacity-25">
                  {complaint.resolutionRemarks}
                </div>
              </div>
            )}

            {complaint.rating > 0 && (
              <div className="mb-4">
                <h6 className="fw-bold text-warning">
                  Student Feedback & Rating
                </h6>

                <div className="p-3 rounded-3 bg-warning bg-opacity-10 border border-warning border-opacity-25">
                  <strong>
                    Rating: {complaint.rating} / 5 Stars
                  </strong>

                  <div className="mt-2">
                    {complaint.feedbackComments}
                  </div>
                </div>
              </div>
            )}

            <div className="d-flex flex-wrap gap-3 text-secondary small pt-3 border-top border-secondary border-opacity-25">
              <span>Created: {complaint.createdAt}</span>

              {complaint.resolvedAt && (
                <span>Resolved: {complaint.resolvedAt}</span>
              )}

              {complaint.closedAt && (
                <span>Closed: {complaint.closedAt}</span>
              )}
            </div>
          </div>

          <div className="modal-footer border-top-0">
            <button
              className="btn btn-secondary"
              onClick={onClose}
            >
              Close
            </button>
          </div>
        </div>
      </div>
    </div>
  );
};