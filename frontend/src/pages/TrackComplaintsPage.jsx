import React, { useState, useEffect } from 'react';
import { complaintApi, dashboardApi } from '../api/client';
import { useAuth } from '../context/AuthContext';
import { ComplaintTable } from '../components/ComplaintTable';
import { ComplaintModal } from '../components/ComplaintModal';
import { FeedbackModal } from '../components/FeedbackModal';

export const TrackComplaintsPage = () => {
  const { user } = useAuth();
  const [complaints, setComplaints] = useState([]);
  const [selectedComplaint, setSelectedComplaint] = useState(null);
  const [feedbackComplaint, setFeedbackComplaint] = useState(null);
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    loadComplaints();
  }, []);

  const loadComplaints = async () => {
    setLoading(true);
    try {
      const res = await complaintApi.getComplaints({ 'X-Student-Id': user?.id });
      if (res.success) setComplaints(res.complaints || []);
    } catch (e) {
      console.error(e);
    } finally {
      setLoading(false);
    }
  };

  const handleDelete = async (id) => {
    if (!window.confirm('Delete this complaint ticket?')) return;
    try {
      await complaintApi.deleteComplaint(id);
      loadComplaints();
    } catch (e) {
      alert(e.message || 'Failed to delete complaint');
    }
  };

  const handleSubmitFeedback = async (complaintId, rating, comments) => {
    try {
      await dashboardApi.submitFeedback({ complaintId, studentId: user.id, rating, comments });
      loadComplaints();
    } catch (e) {
      throw e;
    }
  };

  return (
    <div className="container py-4">
      <div className="mb-4">
        <h3 className="fw-bold text-white mb-1">Track Complaint Status</h3>
        <p className="text-secondary small">Live lifecycle timeline & status tracking for your room tickets</p>
      </div>

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
