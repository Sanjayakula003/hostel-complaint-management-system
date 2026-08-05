import React from 'react';
import { Link, useNavigate } from 'react-router-dom';
import { useAuth } from '../context/AuthContext';
import { Building2, LogOut, User, Shield, Wrench, GraduationCap } from 'lucide-react';

export const Navbar = () => {
  const { user, logoutUser } = useAuth();
  const navigate = useNavigate();

  const handleLogout = () => {
    logoutUser();
    navigate('/');
  };

  const getRoleBadge = (role) => {
    switch (role) {
      case 0:
        return <span className="badge bg-primary d-flex align-items-center gap-1"><GraduationCap size={14}/> Student</span>;
      case 1:
        return <span className="badge bg-warning text-dark d-flex align-items-center gap-1"><Shield size={14}/> Warden</span>;
      case 2:
        return <span className="badge bg-info text-dark d-flex align-items-center gap-1"><Wrench size={14}/> Staff</span>;
      default:
        return <span className="badge bg-secondary">User</span>;
    }
  };

  return (
    <nav className="navbar navbar-expand-lg navbar-dark bg-dark bg-opacity-75 border-bottom border-secondary border-opacity-25 backdrop-blur sticky-top py-3">
      <div className="container">
        <Link className="navbar-brand d-flex align-items-center gap-2 fw-bold text-white fs-4" to="/">
          <div className="bg-primary p-2 rounded-3 text-white d-flex align-items-center justify-content-center">
            <Building2 size={24} />
          </div>
          <span>Hostel<span className="gradient-text">CMS</span></span>
        </Link>

        <button className="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarContent">
          <span className="navbar-toggler-icon"></span>
        </button>

        <div className="collapse navbar-collapse" id="navbarContent">
          <ul className="navbar-nav me-auto mb-2 mb-lg-0 ms-lg-4 gap-2">
            <li className="nav-item">
              <Link className="nav-link text-white-50 hover-text-white fw-medium" to="/">Home</Link>
            </li>
            {user && (
              <>
                <li className="nav-item">
                  <Link className="nav-link text-white-50 hover-text-white fw-medium" to="/dashboard">Dashboard</Link>
                </li>
                {user.role === 0 && (
                  <>
                    <li className="nav-item">
                      <Link className="nav-link text-white-50 hover-text-white fw-medium" to="/raise-complaint">Raise Complaint</Link>
                    </li>
                    <li className="nav-item">
                      <Link className="nav-link text-white-50 hover-text-white fw-medium" to="/track-complaints">Track Complaints</Link>
                    </li>
                  </>
                )}
                {user.role === 1 && (
                  <>
                    <li className="nav-item">
                      <Link className="nav-link text-white-50 hover-text-white fw-medium" to="/view-complaints">Complaints</Link>
                    </li>
                    <li className="nav-item">
                      <Link className="nav-link text-white-50 hover-text-white fw-medium" to="/analytics">Analytics</Link>
                    </li>
                    <li className="nav-item">
                      <Link className="nav-link text-white-50 hover-text-white fw-medium" to="/reports">Reports</Link>
                    </li>
                  </>
                )}
              </>
            )}
          </ul>

          <div className="d-flex align-items-center gap-3">
            {user ? (
              <>
                <div className="d-flex align-items-center gap-2 text-white">
                  <div className="bg-secondary bg-opacity-25 p-2 rounded-circle">
                    <User size={18} />
                  </div>
                  <div className="d-flex flex-column">
                    <span className="fw-semibold small">{user.name || user.username}</span>
                    {getRoleBadge(user.role)}
                  </div>
                </div>
                <Link to="/profile" className="btn btn-outline-light btn-sm rounded-3">Profile</Link>
                <button onClick={handleLogout} className="btn btn-outline-danger btn-sm rounded-3 d-flex align-items-center gap-1">
                  <LogOut size={16} /> Logout
                </button>
              </>
            ) : (
              <div className="d-flex gap-2">
                <Link to="/student-login" className="btn btn-outline-light rounded-3 px-3">Student Portal</Link>
                <Link to="/warden-login" className="btn btn-primary-gradient rounded-3 px-3">Warden Portal</Link>
              </div>
            )}
          </div>
        </div>
      </div>
    </nav>
  );
};
