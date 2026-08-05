import React, { useState } from 'react';
import { Star } from 'lucide-react';

export const FeedbackModal = ({ complaint, onClose, onSubmitFeedback }) => {
  const [rating, setRating] = useState(5);
  const [comments, setComments] = useState('');
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState('');

  const handleSubmit = async (e) => {
    e.preventDefault();
    setLoading(true);
    setError('');

    try {
      await onSubmitFeedback(complaint.id, rating, comments);
      onClose();
    } catch (err) {
      setError(err.message || 'Failed to submit feedback');
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
            <h5 className="modal-title fw-bold text-white">Rate Complaint Resolution #{complaint.id}</h5>
            <button type="button" className="btn-close btn-close-white" onClick={onClose}></button>
          </div>

          <form onSubmit={handleSubmit}>
            <div className="modal-body text-white">
              {error && <div className="alert alert-danger py-2">{error}</div>}

              <div className="mb-4 text-center">
                <label className="form-label text-secondary small d-block">Overall Satisfaction</label>
                <div className="d-flex justify-content-center gap-2 my-2">
                  {[1, 2, 3, 4, 5].map((star) => (
                    <button
                      key={star}
                      type="button"
                      className="btn p-1 text-warning border-0"
                      onClick={() => setRating(star)}
                    >
                      <Star size={32} fill={star <= rating ? '#eab308' : 'none'} />
                    </button>
                  ))}
                </div>
                <span className="fw-semibold text-warning">{rating} out of 5 Stars</span>
              </div>

              <div className="mb-3">
                <label className="form-label text-secondary small">Feedback & Remarks</label>
                <textarea
                  className="form-control"
                  rows="3"
                  placeholder="How well was your complaint handled? Any additional feedback?"
                  value={comments}
                  onChange={(e) => setComments(e.target.value)}
                  required
                ></textarea>
              </div>
            </div>

            <div className="modal-footer border-top-0">
              <button type="button" className="btn btn-secondary rounded-3" onClick={onClose}>Cancel</button>
              <button type="submit" className="btn btn-success rounded-3" disabled={loading}>
                {loading ? 'Submitting...' : 'Submit Feedback & Close Ticket'}
              </button>
            </div>
          </form>
        </div>
      </div>
    </div>
  );
};
