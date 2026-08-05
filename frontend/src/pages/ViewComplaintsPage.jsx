import React, { useState, useEffect } from 'react';
import { complaintApi } from '../api/client';
import { useAuth } from '../context/AuthContext';
import { ComplaintTable } from '../components/ComplaintTable';
import { ComplaintModal } from '../components/ComplaintModal';
import { AssignStaffModal } from '../components/AssignStaffModal';

export const ViewComplaintsPage = () => {
  const { user } = useAuth();
  const [complaints, setComplaints] = useState([]);
  const [selectedComplaint, setSelectedComplaint] = useState(null);
  const [assigningComplaint, setAssigningComplaint] = useState(null);
  const [sendBackTicket, setSendBackTicket] = useState(null);
  const [rejectionRemarks, setRejectionRemarks] = useState('');

  useEffect(() => {
    loadComplaints();
  }, []);

  const loadComplaints = async () => {
    try {
      const res = await complaintApi.getComplaints();
      if (res.success) setComplaints(res.complaints || []);
    } catch (e) {
      console.error(e);
    }
  };

  const handleAssignStaff = async (complaintId, staffId, staffName) => {
    try {
      await complaintApi.updateComplaint(complaintId, {
        action: 'assign_staff',
        staffId,
        staffName,
      });
      loadComplaints();
    } catch (e) {
      throw e;
    }
  };

  const handleApprove = async (complaintId) => {
    try {
      await complaintApi.updateComplaint(complaintId, {
        action: 'approve_warden',
        wardenId: user.id,
      });
      loadComplaints();
    } catch (e) {
      alert(e.message || 'Failed to approve complaint');
    }
  };

  const handleSendBackSubmit = async (e) => {
    e.preventDefault();
    if (!sendBackTicket) return;

    try {
      await complaintApi.updateComplaint(sendBackTicket.id, {
        action: 'send_back_warden',
        wardenId: user.id,
        rejectionRemarks: rejectionRemarks.trim(),
      });
      setSendBackTicket(null);
      setRejectionRemarks('');
      loadComplaints();
    } catch (err) {
      alert(err.message || 'Failed to send back complaint');
    }
  };

  const handleDelete = async (id) => {
    if (!window.confirm('Delete this ticket?')) return;
    try {
      await complaintApi.deleteComplaint(id);
      loadComplaints();
    } catch (e) {
      alert(e.message || 'Failed to delete complaint');
    }
  };

  return (
    <div className="container py-4">
      <div className="mb-4">
        <h3 className="fw-bold text-white mb-1">Manage All Hostel Complaints</h3>
        <p className="text-secondary small">Warden view: Assign maintenance staff, verify completion & monitor resolution status</p>
      </div>

      <ComplaintTable
        complaints={complaints}
        userRole={1}
        onAssignStaff={(c) => setAssigningComplaint(c)}
        onApprove={(c) => handleApprove(c.id)}
        onSendBack={(c) => setSendBackTicket(c)}
        onViewDetails={(c) => setSelectedComplaint(c)}
        onDelete={handleDelete}
      />

      {selectedComplaint && (
        <ComplaintModal complaint={selectedComplaint} onClose={() => setSelectedComplaint(null)} />
      )}

      {assigningComplaint && (
        <AssignStaffModal
          complaint={assigningComplaint}
          onClose={() => setAssigningComplaint(null)}
          onAssigned={handleAssignStaff}
        />
      )}

      {sendBackTicket && (
        <div className="modal show d-block" tabIndex="-1" style={{ backgroundColor: 'rgba(0,0,0,0.7)' }}>
          <div className="modal-dialog modal-dialog-centered">
            <div className="modal-content glass-card p-3">
              <div className="modal-header border-bottom border-secondary border-opacity-25">
                <h5 className="modal-title fw-bold text-white">Send Back Ticket #{sendBackTicket.id} to Staff</h5>
                <button type="button" className="btn-close btn-close-white" onClick={() => setSendBackTicket(null)}></button>
              </div>

              <form onSubmit={handleSendBackSubmit}>
                <div className="modal-body text-white">
                  <p className="text-secondary small mb-3">
                    This complaint will be sent back to <strong className="text-white">{sendBackTicket.assignedStaffName || 'Assigned Staff'}</strong> with status <strong>Assigned</strong>.
                  </p>

                  <div className="mb-3">
                    <label className="form-label text-secondary small">Rejection / Revision Remarks (Optional)</label>
                    <textarea
                      className="form-control"
                      rows="3"
                      placeholder="Reason for sending back..."
                      value={rejectionRemarks}
                      onChange={(e) => setRejectionRemarks(e.target.value)}
                    ></textarea>
                  </div>
                </div>

                <div className="modal-footer border-top-0">
                  <button type="button" className="btn btn-secondary rounded-3" onClick={() => setSendBackTicket(null)}>Cancel</button>
                  <button type="submit" className="btn btn-warning font-weight-bold rounded-3 px-3">
                    Confirm Send Back
                  </button>
                </div>
              </form>
            </div>
          </div>
        </div>
      )}
    </div>
  );
};
