import React, { useState } from 'react';
import { useNavigate } from 'react-router-dom';
import { complaintApi } from '../api/client';
import { useAuth } from '../context/AuthContext';
import { AlertCircle, AlertTriangle, CheckCircle, ArrowRight } from 'lucide-react';

export const RaiseComplaintPage = () => {
  const { user } = useAuth();
  const navigate = useNavigate();

  const [formData, setFormData] = useState({
    title: '',
    category: 'Electrical',
    description: '',
    roomNumber: user?.roomNumber || 'A-101',
  });

  const [duplicateWarning, setDuplicateWarning] = useState(null);
  const [loading, setLoading] = useState(false);
  const [checkingDup, setCheckingDup] = useState(false);
  const [error, setError] = useState('');

  const handleChange = (e) => {
    const { name, value } = e.target;
    setFormData((prev) => ({ ...prev, [name]: value }));

    if (name === 'category' || name === 'roomNumber') {
      checkDuplicate(name === 'roomNumber' ? value : formData.roomNumber, name === 'category' ? value : formData.category);
    }
  };

  const checkDuplicate = async (room, cat) => {
    if (!room || !cat) return;
    setCheckingDup(true);
    try {
      const res = await complaintApi.checkDuplicate(room, cat);
      if (res.success && res.duplicate && res.duplicate.isDuplicateFound) {
        setDuplicateWarning(res.duplicate);
      } else {
        setDuplicateWarning(null);
      }
    } catch (e) {
      console.error(e);
    } finally {
      setCheckingDup(false);
    }
  };

  const handleSubmit = async (e) => {
    e.preventDefault();
    setLoading(true);
    setError('');

    try {
      const payload = {
        studentId: user.id,
        studentName: user.name || user.username,
        roomNumber: formData.roomNumber,
        category: formData.category,
        title: formData.title,
        description: formData.description,
      };

      const res = await complaintApi.createComplaint(payload);
      if (res.success) {
        navigate('/track-complaints');
      } else {
        setError(res.error || 'Failed to raise complaint');
      }
    } catch (err) {
      setError(err.message || 'Server connection error');
    } finally {
      setLoading(false);
    }
  };

  return (
    <div className="container py-5">
      <div className="row justify-content-center">
        <div className="col-12 col-lg-8">
          <div className="glass-card p-4 p-md-5">
            <div className="d-flex align-items-center gap-3 mb-4 pb-3 border-bottom border-secondary border-opacity-25">
              <div className="bg-primary bg-opacity-20 text-primary p-3 rounded-circle">
                <AlertCircle size={32} />
              </div>
              <div>
                <h3 className="fw-bold text-white mb-1">Raise Hostel Complaint</h3>
                <p className="text-secondary small mb-0">Submit maintenance ticket directly to hostel warden</p>
              </div>
            </div>

            {error && <div className="alert alert-danger py-2">{error}</div>}

            {/* Duplicate Complaint Alert Banner */}
            {duplicateWarning && (
              <div className="alert alert-warning border-warning border-opacity-50 bg-warning bg-opacity-10 text-warning p-3 rounded-3 mb-4">
                <div className="d-flex align-items-start gap-2">
                  <AlertTriangle size={24} className="flex-shrink-0 mt-1" />
                  <div>
                    <h6 className="fw-bold mb-1">Duplicate Complaint Notice</h6>
                    <p className="small mb-1">{duplicateWarning.message}</p>
                    <div className="fs-7 text-warning-subtle">
                      Existing Ticket ID: <strong>{duplicateWarning.existingComplaintId}</strong> | Status: <strong>{duplicateWarning.existingStatus}</strong>
                    </div>
                  </div>
                </div>
              </div>
            )}

            <form onSubmit={handleSubmit}>
              <div className="row g-4">
                <div className="col-md-6">
                  <label className="form-label text-secondary small">Hostel Room Number</label>
                  <input
                    type="text"
                    className="form-control"
                    name="roomNumber"
                    value={formData.roomNumber}
                    onChange={handleChange}
                    required
                  />
                </div>

                <div className="col-md-6">
                  <label className="form-label text-secondary small">Complaint Category</label>
                  <select
                    className="form-select"
                    name="category"
                    value={formData.category}
                    onChange={handleChange}
                  >
                    <option value="Electrical">Electrical (Lights, Fans, Sockets)</option>
                    <option value="Plumbing">Plumbing (Tap, Basin, Flush, Leakage)</option>
                    <option value="Carpentry">Carpentry (Door, Bed, Chair, Almirah)</option>
                    <option value="Internet/Wi-Fi">Internet / Wi-Fi Connectivity</option>
                    <option value="Cleaning/Hygiene">Room Cleaning & Sanitation</option>
                    <option value="Other">Other Issues</option>
                  </select>
                </div>

                <div className="col-12">
                  <label className="form-label text-secondary small">Complaint Title / Summary</label>
                  <input
                    type="text"
                    className="form-control"
                    name="title"
                    placeholder="e.g., Ceiling fan not spinning in Room A-101"
                    value={formData.title}
                    onChange={handleChange}
                    required
                  />
                </div>

                <div className="col-12">
                  <label className="form-label text-secondary small">Detailed Issue Description</label>
                  <textarea
                    className="form-control"
                    rows="4"
                    name="description"
                    placeholder="Provide specific details about the issue to help staff resolve it quickly..."
                    value={formData.description}
                    onChange={handleChange}
                    required
                  ></textarea>
                </div>
              </div>

              <div className="d-flex justify-content-end gap-3 mt-4 pt-3 border-top border-secondary border-opacity-25">
                <button type="button" className="btn btn-outline-light rounded-3 px-4" onClick={() => navigate(-1)}>
                  Cancel
                </button>
                <button type="submit" className="btn btn-primary-gradient rounded-3 px-4 d-flex align-items-center gap-2" disabled={loading}>
                  {loading ? 'Submitting Ticket...' : <>Submit Complaint Ticket <ArrowRight size={18} /></>}
                </button>
              </div>
            </form>
          </div>
        </div>
      </div>
    </div>
  );
};
