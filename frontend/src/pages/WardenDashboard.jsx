import React, { useState, useEffect } from 'react';
import { complaintApi, dashboardApi } from '../api/client';
import { useAuth } from '../context/AuthContext';
import { DashboardCards } from '../components/DashboardCards';
import { ComplaintTable } from '../components/ComplaintTable';
import { ComplaintModal } from '../components/ComplaintModal';
import { AssignStaffModal } from '../components/AssignStaffModal';
import { AnalyticsCards } from '../components/AnalyticsCards';
import { Shield, CheckCircle, RotateCcw, AlertCircle, Clock } from 'lucide-react';

export const WardenDashboard = () => {
  const { user } = useAuth();
  const [complaints, setComplaints] = useState([]);
  const [metrics, setMetrics] = useState(null);
  const [selectedComplaint, setSelectedComplaint] = useState(null);
  const [assigningComplaint, setAssigningComplaint] = useState(null);
  const [sendBackTicket, setSendBackTicket] = useState(null);
  const [rejectionRemarks, setRejectionRemarks] = useState('');
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    loadData();
  }, []);

  const loadData = async () => {
    setLoading(true);
    try {
      const [cRes, dRes] = await Promise.all([
        complaintApi.getComplaints(),
        dashboardApi.getDashboard(),
      ]);

      if (cRes.success) setComplaints(cRes.complaints || []);
      if (dRes.success) setMetrics(dRes.metrics);
    } catch (e) {
      console.error('Error loading warden data:', e);
    } finally {
      setLoading(false);
    }
  };

  const handleAssignStaff = async (complaintId, staffId, staffName) => {
    try {
      await complaintApi.updateComplaint(complaintId, {
        action: 'assign_staff',
        staffId,
        staffName,
      });
      loadData();
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
      loadData();
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
      loadData();
    } catch (err) {
      alert(err.message || 'Failed to send back complaint');
    }
  };

  const handleDelete = async (id) => {
    if (!window.confirm('Are you sure you want to delete this complaint ticket?')) return;
    try {
      await complaintApi.deleteComplaint(id);
      loadData();
    } catch (e) {
      alert(e.message || 'Failed to delete complaint');
    }
  };

  const pendingVerificationList = complaints.filter(
    (c) => c.statusStr === 'COMPLETED_BY_STAFF' || c.statusStr === 'Completed by Staff'
  );

  return (
    <div className="container py-4">
      <div className="d-flex flex-column flex-md-row justify-content-between align-items-md-center gap-3 mb-4">
        <div>
          <h3 className="fw-bold text-white mb-1">Warden Governance Dashboard</h3>
          <p className="text-secondary small mb-0">Hostel Administration & Ticket Lifecycle Control</p>
        </div>
      </div>

      <DashboardCards metrics={metrics} />

      {/* PENDING VERIFICATION SECTION */}
      <div className="glass-card p-4 mb-5 border-info border-opacity-30">
        <div className="d-flex align-items-center justify-content-between mb-3">
          <div className="d-flex align-items-center gap-2">
            <div className="p-2 rounded-3 bg-cyan bg-opacity-20 text-info">
              <Clock size={22} />
            </div>
            <div>
              <h5 className="fw-bold text-white mb-0">Pending Verification</h5>
              <span className="text-secondary small">Tickets completed by staff awaiting Warden approval</span>
            </div>
          </div>
          <span className="badge bg-info bg-opacity-20 text-info border border-info border-opacity-30 px-3 py-2 fs-7 rounded-pill">
            {pendingVerificationList.length} Pending Approval
          </span>
        </div>

        {pendingVerificationList.length === 0 ? (
          <div className="p-4 text-center text-secondary bg-dark bg-opacity-40 rounded-3 border border-secondary border-opacity-10">
            No complaints currently pending verification.
          </div>
        ) : (
          <div className="row g-3">
            {pendingVerificationList.map((c) => (
              <div key={c.id} className="col-12 col-lg-6">
                <div className="p-3 rounded-3 bg-dark bg-opacity-60 border border-secondary border-opacity-25 h-100 d-flex flex-column justify-content-between">
                  <div>
                    <div className="d-flex justify-content-between align-items-start mb-2">
                      <span className="fw-mono text-indigo-400 fw-bold">{c.id}</span>
                      <span className="badge badge-status-completed px-2.5 py-1">Completed by Staff</span>
                    </div>
                    <h6 className="fw-bold text-white mb-1">{c.title}</h6>
                    <div className="text-secondary fs-7 mb-2">
                      Student: <strong className="text-white">{c.studentName}</strong> | Room: <strong className="text-white">{c.roomNumber}</strong> ({c.category})
                    </div>
                    <div className="text-secondary fs-7 mb-2">
                      Assigned Staff: <span className="text-info-subtle fw-semibold">{c.assignedStaffName || 'Staff'}</span>
                    </div>

                    <div className="p-2.5 rounded-3 bg-secondary bg-opacity-10 border border-secondary border-opacity-25 mb-3">
                      <span className="text-secondary fs-8 d-block fw-semibold mb-1">Staff Maintenance Remarks:</span>
                      <div className="text-white fs-7">{c.resolutionRemarks || c.remarks || 'No remarks provided'}</div>
                    </div>

                    <div className="fs-8 text-secondary mb-3">
                      Completion Date: {c.updatedAt ? c.updatedAt.substring(0, 16) : 'N/A'}
                    </div>
                  </div>

                  <div className="d-flex gap-2 justify-content-end pt-2 border-top border-secondary border-opacity-10">
                    <button
                      onClick={() => setSelectedComplaint(c)}
                      className="btn btn-sm btn-outline-light rounded-3 px-3"
                    >
                      View Details
                    </button>
                    <button
                      onClick={() => setSendBackTicket(c)}
                      className="btn btn-sm btn-outline-warning rounded-3 px-3 d-flex align-items-center gap-1"
                    >
                      <RotateCcw size={14} /> Send Back
                    </button>
                    <button
                      onClick={() => handleApprove(c.id)}
                      className="btn btn-sm btn-success rounded-3 px-3 fw-bold d-flex align-items-center gap-1"
                    >
                      <CheckCircle size={14} /> Approve Ticket
                    </button>
                  </div>
                </div>
              </div>
            ))}
          </div>
        )}
      </div>

      <div className="mb-5">
        <ComplaintTable
          complaints={complaints}
          userRole={1}
          onAssignStaff={(c) => setAssigningComplaint(c)}
          onApprove={(c) => handleApprove(c.id)}
          onSendBack={(c) => setSendBackTicket(c)}
          onViewDetails={(c) => setSelectedComplaint(c)}
          onDelete={handleDelete}
        />
      </div>

      <div className="mb-4">
        <h4 className="fw-bold text-white mb-3">Live System Analytics</h4>
        <AnalyticsCards analytics={metrics} />
      </div>

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

      {/* Send Back Modal */}
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
                      placeholder="Reason for sending back (e.g. Issue not fully resolved, physical inspection needed)..."
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
