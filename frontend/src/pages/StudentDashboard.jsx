import React, { useState, useEffect } from 'react';
import { Link } from 'react-router-dom';
import { complaintApi, dashboardApi } from '../api/client';
import { useAuth } from '../context/AuthContext';
import { DashboardCards } from '../components/DashboardCards';
import { ComplaintTable } from '../components/ComplaintTable';
import { ComplaintModal } from '../components/ComplaintModal';
import { FeedbackModal } from '../components/FeedbackModal';
import { PlusCircle, ListFilter, AlertCircle } from 'lucide-react';

export const StudentDashboard = () => {
  const { user } = useAuth();
  const [complaints, setComplaints] = useState([]);
  const [metrics, setMetrics] = useState(null);
  const [selectedComplaint, setSelectedComplaint] = useState(null);
  const [feedbackComplaint, setFeedbackComplaint] = useState(null);
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    loadData();
  }, []);

  const loadData = async () => {
    setLoading(true);
    try {
      const [cRes, dRes] = await Promise.all([
        complaintApi.getComplaints({ 'X-Student-Id': user?.id }),
        dashboardApi.getDashboard(),
      ]);

      if (cRes.success) setComplaints(cRes.complaints || []);
      if (dRes.success) setMetrics(dRes.metrics);
    } catch (e) {
      console.error('Error fetching student dashboard data:', e);
    } finally {
      setLoading(false);
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

  const handleSubmitFeedback = async (complaintId, rating, comments) => {
    try {
      await dashboardApi.submitFeedback({ complaintId, studentId: user.id, rating, comments });
      loadData();
    } catch (e) {
      throw e;
    }
  };

  return (
    <div className="container py-4">
      <div className="d-flex flex-column flex-md-row justify-content-between align-items-md-center gap-3 mb-4">
        <div>
          <h3 className="fw-bold text-white mb-1">Welcome back, {user?.name || user?.username}!</h3>
          <p className="text-secondary small mb-0">Student Hostel Dashboard | Room {user?.roomNumber || 'N/A'}</p>
        </div>
        <div className="d-flex gap-2">
          <Link to="/raise-complaint" className="btn btn-primary-gradient rounded-3 px-3 py-2 d-flex align-items-center gap-2">
            <PlusCircle size={18} /> Raise Complaint
          </Link>
        </div>
      </div>

      <DashboardCards metrics={metrics} />

      <ComplaintTable
        complaints={complaints}
        userRole={0}
        onViewDetails={(c) => setSelectedComplaint(c)}
        onFeedback={(c) => setFeedbackComplaint(c)}
        onDelete={handleDelete}
      />

      {selectedComplaint && (
        <ComplaintModal complaint={selectedComplaint} onClose={() => setSelectedComplaint(null)} />
      )}

      {feedbackComplaint && (
        <FeedbackModal
          complaint={feedbackComplaint}
          onClose={() => setFeedbackComplaint(null)}
          onSubmitFeedback={handleSubmitFeedback}
        />
      )}
    </div>
  );
};
