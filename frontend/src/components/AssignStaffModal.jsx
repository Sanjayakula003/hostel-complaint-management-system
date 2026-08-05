import React, { useState, useEffect } from 'react';
import { dashboardApi } from '../api/client';

export const AssignStaffModal = ({ complaint, onClose, onAssigned }) => {
  const [staffList, setStaffList] = useState([]);
  const [selectedStaffId, setSelectedStaffId] = useState('');
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState('');

  useEffect(() => {
    fetchStaff();
  }, []);

  const fetchStaff = async () => {
    try {
      const res = await dashboardApi.getStaffList();
      if (res.success && res.staffList) {
        setStaffList(res.staffList);
        if (res.staffList.length > 0) setSelectedStaffId(res.staffList[0].id);
      }
    } catch (e) {
      console.error(e);
    }
  };

  const handleAssign = async (e) => {
    e.preventDefault();
    if (!selectedStaffId) return;

    const staffObj = staffList.find(s => s.id === selectedStaffId);
    const staffName = staffObj ? staffObj.name : 'Maintenance Staff';

    setLoading(true);
    setError('');

    try {
      await onAssigned(complaint.id, selectedStaffId, staffName);
      onClose();
    } catch (err) {
      setError(err.message || 'Failed to assign staff member');
    } finally {
      setLoading(false);
    }
  };

  if (!complaint) return null;

  return (
    <div className="modal show d-block" tabIndex="-1" style={{ backgroundColor: 'rgba(0,0,0,0.7)' }}>
      <div className="modal-dialog modal-dialog-centered">
        <div className="modal-content glass-card p-3">
          <div className="modal-header border-bottom border-secondary border-opacity-25">
            <h5 className="modal-title fw-bold text-white">Assign Staff to Ticket #{complaint.id}</h5>
            <button type="button" className="btn-close btn-close-white" onClick={onClose}></button>
          </div>

          <form onSubmit={handleAssign}>
            <div className="modal-body text-white">
              {error && <div className="alert alert-danger py-2">{error}</div>}

              <div className="mb-3">
                <label className="form-label text-secondary small">Ticket Details</label>
                <div className="p-3 rounded-3 bg-dark bg-opacity-50">
                  <div className="fw-semibold text-white">{complaint.title}</div>
                  <div className="text-secondary fs-7">{complaint.category} | Room {complaint.roomNumber}</div>
                </div>
              </div>

              <div className="mb-3">
                <label className="form-label text-secondary small">Select Maintenance Staff</label>
                <select
                  className="form-select"
                  value={selectedStaffId}
                  onChange={(e) => setSelectedStaffId(e.target.value)}
                  required
                >
                  {staffList.map((staff) => (
                    <option key={staff.id} value={staff.id}>
                      {staff.name} ({staff.department || 'Maintenance'}) - ID: {staff.id}
                    </option>
                  ))}
                </select>
              </div>
            </div>

            <div className="modal-footer border-top-0">
              <button type="button" className="btn btn-secondary rounded-3" onClick={onClose}>Cancel</button>
              <button type="submit" className="btn btn-primary-gradient rounded-3" disabled={loading}>
                {loading ? 'Assigning...' : 'Assign Staff Member'}
              </button>
            </div>
          </form>
        </div>
      </div>
    </div>
  );
};
