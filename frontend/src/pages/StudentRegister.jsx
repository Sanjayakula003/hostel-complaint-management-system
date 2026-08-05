import React, { useState } from 'react';
import { Link, useNavigate } from 'react-router-dom';
import { authApi } from '../api/client';
import { GraduationCap, User, Lock, Mail, Phone, Hash, Home, Building } from 'lucide-react';

export const StudentRegister = () => {
  const [formData, setFormData] = useState({
    username: '',
    password: '',
    name: '',
    email: '',
    phone: '',
    rollNumber: '',
    roomNumber: '',
    hostelBlock: 'Block-A',
  });
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState('');
  const [successMsg, setSuccessMsg] = useState('');
  const navigate = useNavigate();

  const handleChange = (e) => {
    setFormData({ ...formData, [e.target.name]: e.target.value });
  };

  const handleSubmit = async (e) => {
    e.preventDefault();
    setLoading(true);
    setError('');
    setSuccessMsg('');

    try {
      const res = await authApi.register(formData);
      if (res.success) {
        setSuccessMsg('Registration successful! Redirecting to login...');
        setTimeout(() => navigate('/student-login'), 1500);
      } else {
        setError(res.error || 'Registration failed');
      }
    } catch (err) {
      setError(err.message || 'Registration error');
    } finally {
      setLoading(false);
    }
  };

  return (
    <div className="container py-5">
      <div className="row justify-content-center">
        <div className="col-12 col-md-8 col-lg-6">
          <div className="glass-card p-4 p-md-5">
            <div className="text-center mb-4">
              <div className="bg-primary bg-opacity-20 text-primary p-3 rounded-circle d-inline-flex mb-3">
                <GraduationCap size={36} />
              </div>
              <h3 className="fw-bold text-white mb-1">Student Registration</h3>
              <p className="text-secondary small">Create your student profile in Hostel CMS</p>
            </div>

            {error && <div className="alert alert-danger py-2">{error}</div>}
            {successMsg && <div className="alert alert-success py-2">{successMsg}</div>}

            <form onSubmit={handleSubmit}>
              <div className="row g-3">
                <div className="col-md-6">
                  <label className="form-label text-secondary small">Username</label>
                  <input type="text" className="form-control" name="username" value={formData.username} onChange={handleChange} required />
                </div>
                <div className="col-md-6">
                  <label className="form-label text-secondary small">Password</label>
                  <input type="password" className="form-control" name="password" placeholder="e.g., Pass@123" value={formData.password} onChange={handleChange} required />
                  <span className="fs-8 text-secondary">Min 8 chars, 1 upper, 1 lower, 1 digit, 1 special</span>
                </div>
                <div className="col-md-12">
                  <label className="form-label text-secondary small">Full Name</label>
                  <input type="text" className="form-control" name="name" value={formData.name} onChange={handleChange} required />
                </div>
                <div className="col-md-6">
                  <label className="form-label text-secondary small">Email Address</label>
                  <input type="email" className="form-control" name="email" value={formData.email} onChange={handleChange} required />
                </div>
                <div className="col-md-6">
                  <label className="form-label text-secondary small">Phone Number</label>
                  <input type="tel" className="form-control" name="phone" value={formData.phone} onChange={handleChange} required />
                </div>
                <div className="col-md-4">
                  <label className="form-label text-secondary small">Roll Number</label>
                  <input type="text" className="form-control" name="rollNumber" value={formData.rollNumber} onChange={handleChange} required />
                </div>
                <div className="col-md-4">
                  <label className="form-label text-secondary small">Room Number</label>
                  <input type="text" className="form-control" name="roomNumber" value={formData.roomNumber} onChange={handleChange} required />
                </div>
                <div className="col-md-4">
                  <label className="form-label text-secondary small">Hostel Block</label>
                  <select className="form-select" name="hostelBlock" value={formData.hostelBlock} onChange={handleChange}>
                    <option value="Block-A">Block-A</option>
                    <option value="Block-B">Block-B</option>
                    <option value="Block-C">Block-C</option>
                    <option value="Girls-Block">Girls-Block</option>
                  </select>
                </div>
              </div>

              <button type="submit" className="btn btn-primary-gradient w-100 py-2.5 rounded-3 mt-4 mb-3" disabled={loading}>
                {loading ? 'Creating Profile...' : 'Complete Registration'}
              </button>

              <div className="text-center">
                <span className="text-secondary small">Already registered? </span>
                <Link to="/student-login" className="text-indigo-400 fw-semibold text-decoration-none small">Login Here</Link>
              </div>
            </form>
          </div>
        </div>
      </div>
    </div>
  );
};
