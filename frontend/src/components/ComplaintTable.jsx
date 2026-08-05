import React, { useState } from 'react';
import { Eye, UserCheck, Wrench, CheckCircle, Trash2, MessageSquare, AlertTriangle } from 'lucide-react';

export const ComplaintTable = ({
  complaints,
  userRole,
  onAssignStaff,
  onUpdateStatus,
  onMarkCompleted,
  onApprove,
  onSendBack,
  onDelete,
  onViewDetails,
  onFeedback
}) => {
  const [searchTerm, setSearchTerm] = useState('');
  const [statusFilter, setStatusFilter] = useState('');
  const [categoryFilter, setCategoryFilter] = useState('');

  const filtered = complaints.filter(c => {
    const matchesSearch = c.title.toLowerCase().includes(searchTerm.toLowerCase()) ||
                          c.id.toLowerCase().includes(searchTerm.toLowerCase()) ||
                          c.roomNumber.toLowerCase().includes(searchTerm.toLowerCase()) ||
                          c.studentName.toLowerCase().includes(searchTerm.toLowerCase());
    const matchesStatus = !statusFilter || c.status.toString() === statusFilter || c.statusStr.toUpperCase() === statusFilter.toUpperCase();
    const matchesCategory = !categoryFilter || c.category === categoryFilter;
    return matchesSearch && matchesStatus && matchesCategory;
  });

  const getPriorityBadge = (p, pStr) => {
    switch (pStr.toUpperCase()) {
      case 'CRITICAL': return <span className="badge badge-priority-critical px-2 py-1">CRITICAL</span>;
      case 'HIGH': return <span className="badge badge-priority-high px-2 py-1">HIGH</span>;
      case 'MEDIUM': return <span className="badge badge-priority-medium px-2 py-1">MEDIUM</span>;
      default: return <span className="badge badge-priority-low px-2 py-1">LOW</span>;
    }
  };

  const getStatusBadge = (sStr) => {
    switch (sStr.toUpperCase()) {
      case 'PENDING': return <span className="badge badge-status-pending px-2.5 py-1">Pending</span>;
      case 'ASSIGNED': return <span className="badge badge-status-assigned px-2.5 py-1">Assigned</span>;
      case 'IN_PROGRESS': return <span className="badge badge-status-inprogress px-2.5 py-1">In Progress</span>;
      case 'COMPLETED_BY_STAFF':
      case 'COMPLETED BY STAFF':
      case 'COMPLETED': return <span className="badge badge-status-completed px-2.5 py-1">Completed by Staff</span>;
      case 'RESOLVED': return <span className="badge badge-status-resolved px-2.5 py-1">Resolved</span>;
      case 'CLOSED': return <span className="badge badge-status-closed px-2.5 py-1">Closed</span>;
      default: return <span className="badge bg-secondary px-2.5 py-1">{sStr}</span>;
    }
  };

  return (
    <div className="glass-card p-4">
      <div className="d-flex flex-column flex-md-row gap-3 justify-content-between align-items-md-center mb-4">
        <div>
          <h5 className="fw-bold text-white mb-1">Complaint Tickets</h5>
          <span className="text-secondary small">Total tickets listed: {filtered.length}</span>
        </div>
        <div className="d-flex flex-wrap gap-2">
          <input
            type="text"
            className="form-control form-control-sm rounded-3 px-3"
            placeholder="Search tickets..."
            style={{ width: '200px' }}
            value={searchTerm}
            onChange={(e) => setSearchTerm(e.target.value)}
          />
          <select className="form-select form-select-sm rounded-3" style={{ width: '140px' }} value={statusFilter} onChange={(e) => setStatusFilter(e.target.value)}>
            <option value="">All Statuses</option>
            <option value="PENDING">Pending</option>
            <option value="ASSIGNED">Assigned</option>
            <option value="IN_PROGRESS">In Progress</option>
            <option value="RESOLVED">Resolved</option>
            <option value="CLOSED">Closed</option>
          </select>
          <select className="form-select form-select-sm rounded-3" style={{ width: '140px' }} value={categoryFilter} onChange={(e) => setCategoryFilter(e.target.value)}>
            <option value="">All Categories</option>
            <option value="Electrical">Electrical</option>
            <option value="Plumbing">Plumbing</option>
            <option value="Carpentry">Carpentry</option>
            <option value="Internet/Wi-Fi">Internet/Wi-Fi</option>
            <option value="Cleaning/Hygiene">Cleaning</option>
          </select>
        </div>
      </div>

      <div className="table-responsive">
        <table className="table custom-table align-middle">
          <thead>
            <tr>
              <th>ID</th>
              <th>Student / Room</th>
              <th>Title & Category</th>
              <th>Priority</th>
              <th>Status</th>
              <th>Assigned Staff</th>
              <th>Date</th>
              <th className="text-end">Actions</th>
            </tr>
          </thead>
          <tbody>
            {filtered.length === 0 ? (
              <tr>
                <td colSpan="8" className="text-center py-4 text-secondary">
                  No complaint tickets found matching criteria.
                </td>
              </tr>
            ) : (
              filtered.map((c) => (
                <tr key={c.id}>
                  <td className="fw-mono text-indigo-400 small">{c.id}</td>
                  <td>
                    <div className="fw-semibold text-white">{c.studentName}</div>
                    <div className="text-secondary fs-7">Room {c.roomNumber}</div>
                  </td>
                  <td>
                    <div className="fw-medium text-white">{c.title}</div>
                    <span className="badge bg-secondary bg-opacity-25 text-white-50 fs-8">{c.category}</span>
                  </td>
                  <td>{getPriorityBadge(c.priority, c.priorityStr)}</td>
                  <td>{getStatusBadge(c.statusStr)}</td>
                  <td>
                    {c.assignedStaffName ? (
                      <span className="text-info-subtle fw-medium">{c.assignedStaffName}</span>
                    ) : (
                      <span className="text-muted italic small">Unassigned</span>
                    )}
                  </td>
                  <td className="text-secondary small">{c.createdAt ? c.createdAt.substring(0, 16) : ''}</td>
                  <td className="text-end">
                    <div className="d-flex gap-1 justify-content-end">
                      <button onClick={() => onViewDetails(c)} className="btn btn-sm btn-outline-light p-1 rounded-2" title="View Details">
                        <Eye size={16} />
                      </button>

                      {/* Warden Actions */}
                      {userRole === 1 && c.statusStr === 'PENDING' && onAssignStaff && (
                        <button onClick={() => onAssignStaff(c)} className="btn btn-sm btn-outline-info p-1 rounded-2" title="Assign Staff">
                          <UserCheck size={16} />
                        </button>
                      )}

                      {userRole === 1 && (c.statusStr === 'COMPLETED_BY_STAFF' || c.statusStr === 'Completed by Staff') && onApprove && (
                        <>
                          <button onClick={() => onApprove(c)} className="btn btn-sm btn-success px-2 py-1 fs-8 rounded-2 d-flex align-items-center gap-1" title="Approve Ticket">
                            <CheckCircle size={14} /> Approve
                          </button>
                          <button onClick={() => onSendBack(c)} className="btn btn-sm btn-outline-warning px-2 py-1 fs-8 rounded-2 d-flex align-items-center gap-1" title="Send Back to Staff">
                            Send Back
                          </button>
                        </>
                      )}

                      {/* Staff Actions */}
                      {userRole === 2 && (c.statusStr === 'ASSIGNED' || c.statusStr === 'Assigned') && onUpdateStatus && (
                        <button onClick={() => onUpdateStatus(c)} className="btn btn-sm btn-outline-warning p-1 rounded-2" title="Update Status to In Progress">
                          <Wrench size={16} />
                        </button>
                      )}

                      {userRole === 2 && (c.statusStr === 'ASSIGNED' || c.statusStr === 'IN_PROGRESS' || c.statusStr === 'In Progress' || c.statusStr === 'Assigned') && onMarkCompleted && (
                        <button onClick={() => onMarkCompleted(c)} className="btn btn-sm btn-cyan px-2 py-1 fs-8 rounded-2 text-info border-info d-flex align-items-center gap-1" title="Mark as Completed">
                          <CheckCircle size={14} /> Mark Completed
                        </button>
                      )}

                      {/* Student Action: Submit Feedback / Close */}
                      {userRole === 0 && c.statusStr === 'RESOLVED' && (
                        <button onClick={() => onFeedback(c)} className="btn btn-sm btn-outline-success p-1 rounded-2" title="Feedback & Close">
                          <MessageSquare size={16} />
                        </button>
                      )}

                      {/* Delete */}
                      {(userRole === 1 || userRole === 0) && (
                        <button onClick={() => onDelete(c.id)} className="btn btn-sm btn-outline-danger p-1 rounded-2" title="Delete Ticket">
                          <Trash2 size={16} />
                        </button>
                      )}
                    </div>
                  </td>
                </tr>
              ))
            )}
          </tbody>
        </table>
      </div>
    </div>
  );
};
