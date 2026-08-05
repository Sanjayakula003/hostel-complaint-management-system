import React, { useState } from 'react';
import { Link, useNavigate } from 'react-router-dom';
import { authApi } from '../api/client';
import { useAuth } from '../context/AuthContext';
import { GraduationCap, Lock, User, ArrowRight } from 'lucide-react';

export const StudentLogin = () => {
  const [username, setUsername] = useState('student_aarav');
  const [password, setPassword] = useState('Pass@123');
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState('');
  const { loginUser } = useAuth();
  const navigate = useNavigate();

  const handleSubmit = async (e) => {
    e.preventDefault();
    setLoading(true);
    setError('');

    try {
      const res = await authApi.login({ username, password, role: 'STUDENT' });
      if (res.success && res.user) {
        loginUser(res.user);
        navigate('/dashboard');
      } else {
        setError(res.error || 'Invalid student credentials');
      }
    } catch (err) {
      setError(err.message || 'Login failed. Please check backend connection.');
    } finally {
      setLoading(false);
    }
  };

  return (
    <div className="container py-5">
      <div className="row justify-content-center">
        <div className="col-12 col-md-6 col-lg-5">
          <div className="glass-card p-4 p-md-5">
            <div className="text-center mb-4">
              <div className="bg-primary bg-opacity-20 text-primary p-3 rounded-circle d-inline-flex mb-3">
                <GraduationCap size={36} />
              </div>
              <h3 className="fw-bold text-white mb-1">Student Login</h3>
              <p className="text-secondary small">Access your hostel complaint portal</p>
            </div>

            {error && <div className="alert alert-danger py-2">{error}</div>}

            <form onSubmit={handleSubmit}>
              <div className="mb-3">
                <label className="form-label text-secondary small">Username / Roll No</label>
                <div className="input-group">
                  <span className="input-group-text bg-dark border-secondary border-opacity-25 text-secondary">
                    <User size={18} />
                  </span>
                  <input
                    type="text"
                    className="form-control"
                    placeholder="Enter username"
                    value={username}
                    onChange={(e) => setUsername(e.target.value)}
                    required
                  />
                </div>
              </div>

              <div className="mb-4">
                <label className="form-label text-secondary small">Password</label>
                <div className="input-group">
                  <span className="input-group-text bg-dark border-secondary border-opacity-25 text-secondary">
                    <Lock size={18} />
                  </span>
                  <input
                    type="password"
                    className="form-control"
                    placeholder="Enter password"
                    value={password}
                    onChange={(e) => setPassword(e.target.value)}
                    required
                  />
                </div>
              </div>

              <button type="submit" className="btn btn-primary-gradient w-100 py-2.5 rounded-3 mb-3 d-flex align-items-center justify-content-center gap-2" disabled={loading}>
                {loading ? 'Authenticating...' : <>Login to Portal <ArrowRight size={18} /></>}
              </button>

              <div className="text-center">
                <span className="text-secondary small">Don't have a student account? </span>
                <Link to="/student-register" className="text-indigo-400 fw-semibold text-decoration-none small">Register Here</Link>
              </div>
            </form>
          </div>
        </div>
      </div>
    </div>
  );
};
