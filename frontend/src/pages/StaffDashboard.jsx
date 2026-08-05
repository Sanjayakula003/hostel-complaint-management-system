import React, { useState, useEffect } from 'react';
import { complaintApi } from '../api/client';
import { useAuth } from '../context/AuthContext';
import { ComplaintTable } from '../components/ComplaintTable';
import { ComplaintModal } from '../components/ComplaintModal';
import { CheckCircle, AlertCircle } from 'lucide-react';

export const StaffDashboard = () => {
  const { user } = useAuth();
  const [complaints, setComplaints] = useState([]);
  const [selectedComplaint, setSelectedComplaint] = useState(null);
  const [completingComplaint, setCompletingComplaint] = useState(null);
  const [remarks, setRemarks] = useState('');
  const [validationError, setValidationError] = useState('');
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    loadData();
  }, []);

  const loadData = async () => {
    setLoading(true);
    try {
      const res = await complaintApi.getComplaints({ 'X-Staff-Id': user?.id });
      if (res.success) setComplaints(res.complaints || []);
    } catch (e) {
      console.error(e);
    } finally {
      setLoading(false);
    }
  };

  const handleMarkInProgress = async (complaint) => {
    try {
      await complaintApi.updateComplaint(complaint.id, {
        action: 'mark_in_progress',
        staffId: user.id,
      });
      loadData();
    } catch (err) {
      alert(err.message || 'Failed to update status to In Progress');
    }
  };

  const handleMarkCompletedSubmit = async (e) => {
    e.preventDefault();
    if (!completingComplaint) return;

    if (!remarks.trim()) {
      setValidationError('Maintenance remarks are required before marking a complaint as completed.');
      return;
    }

    setValidationError('');
    try {
      await complaintApi.updateComplaint(completingComplaint.id, {
        action: 'mark_completed_by_staff',
        staffId: user.id,
        remarks: remarks.trim(),
      });
      setCompletingComplaint(null);
      setRemarks('');
      loadData();
    } catch (err) {
      setValidationError(err.message || 'Failed to mark complaint as completed');
    }
  };

  return (
    <div className="container py-4">
      <div className="d-flex flex-column flex-md-row justify-content-between align-items-md-center gap-3 mb-4">
        <div>
          <h3 className="fw-bold text-white mb-1">Maintenance Staff Workbench</h3>
          <p className="text-secondary small mb-0">Assigned Job Tickets | {user?.name} ({user?.department || 'Maintenance'})</p>
        </div>
      </div>

      <ComplaintTable
        complaints={complaints}
        userRole={2}
        onUpdateStatus={handleMarkInProgress}
        onMarkCompleted={(c) => {
          setCompletingComplaint(c);
          setRemarks(c.resolutionRemarks || c.remarks || '');
          setValidationError('');
        }}
        onViewDetails={(c) => setSelectedComplaint(c)}
      />

      {selectedComplaint && (
        <ComplaintModal complaint={selectedComplaint} onClose={() => setSelectedComplaint(null)} />
      )}

      {/* Completion Modal */}
      {completingComplaint && (
        <div className="modal show d-block" tabIndex="-1" style={{ backgroundColor: 'rgba(0,0,0,0.7)' }}>
          <div className="modal-dialog modal-dialog-centered">
            <div className="modal-content glass-card p-3">
              <div className="modal-header border-bottom border-secondary border-opacity-25">
                <h5 className="modal-title fw-bold text-white d-flex align-items-center gap-2">
                  <CheckCircle className="text-info" size={22} /> Mark Completed - Ticket #{completingComplaint.id}
                </h5>
                <button type="button" className="btn-close btn-close-white" onClick={() => setCompletingComplaint(null)}></button>
              </div>

              <form onSubmit={handleMarkCompletedSubmit}>
                <div className="modal-body text-white">
                  {validationError && (
                    <div className="alert alert-danger py-2 small d-flex align-items-center gap-2">
                      <AlertCircle size={16} /> {validationError}
                    </div>
                  )}

                  <div className="mb-3">
                    <label className="form-label text-secondary small">Ticket Summary</label>
                    <div className="p-3 rounded-3 bg-dark bg-opacity-50 border border-secondary border-opacity-25">
                      <div className="fw-semibold text-white">{completingComplaint.title}</div>
                      <div className="text-secondary fs-7">Room {completingComplaint.roomNumber} ({completingComplaint.category})</div>
                    </div>
                  </div>

                  <div className="mb-3">
                    <label className="form-label text-secondary small">Maintenance Remarks <span className="text-danger">*</span></label>
                    <textarea
                      className="form-control"
                      rows="4"
                      placeholder="Describe the maintenance work carried out (e.g. Replaced faulty wiring, sealed pipe leak, tested operation)..."
                      value={remarks}
                      onChange={(e) => {
                        setRemarks(e.target.value);
                        if (e.target.value.trim()) setValidationError('');
                      }}
                      required
                    ></textarea>
                    <span className="fs-8 text-secondary mt-1 d-block">
                      Note: This complaint will be sent to the Warden for approval before final student resolution.
                    </span>
                  </div>
                </div>

                <div className="modal-footer border-top-0">
                  <button type="button" className="btn btn-secondary rounded-3" onClick={() => setCompletingComplaint(null)}>Cancel</button>
                  <button type="submit" className="btn btn-info text-dark font-weight-bold rounded-3 px-4">
                    Submit as Completed by Staff
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
