import React from 'react';
import { Link, useLocation } from 'react-router-dom';
import { useAuth } from '../context/AuthContext';
import { LayoutDashboard, AlertCircle, ListFilter, BarChart3, FileText, User, Wrench } from 'lucide-react';

export const Sidebar = () => {
  const { user } = useAuth();
  const location = useLocation();

  if (!user) return null;

  const isActive = (path) => location.pathname === path ? 'bg-primary text-white' : 'text-white-50 hover-bg-secondary';

  return (
    <div className="glass-card p-3 d-flex flex-column gap-2" style={{ minWidth: '240px' }}>
      <div className="px-3 py-2 text-uppercase text-secondary fw-bold fs-7">
        Navigation
      </div>
      <Link to="/dashboard" className={`d-flex align-items-center gap-3 px-3 py-2.5 rounded-3 text-decoration-none transition-all ${isActive('/dashboard')}`}>
        <LayoutDashboard size={18} />
        <span className="fw-medium">Dashboard</span>
      </Link>

      {user.role === 0 && (
        <>
          <Link to="/raise-complaint" className={`d-flex align-items-center gap-3 px-3 py-2.5 rounded-3 text-decoration-none transition-all ${isActive('/raise-complaint')}`}>
            <AlertCircle size={18} />
            <span className="fw-medium">Raise Complaint</span>
          </Link>
          <Link to="/track-complaints" className={`d-flex align-items-center gap-3 px-3 py-2.5 rounded-3 text-decoration-none transition-all ${isActive('/track-complaints')}`}>
            <ListFilter size={18} />
            <span className="fw-medium">Track Complaints</span>
          </Link>
        </>
      )}

      {user.role === 1 && (
        <>
          <Link to="/view-complaints" className={`d-flex align-items-center gap-3 px-3 py-2.5 rounded-3 text-decoration-none transition-all ${isActive('/view-complaints')}`}>
            <ListFilter size={18} />
            <span className="fw-medium">Manage Complaints</span>
          </Link>
          <Link to="/analytics" className={`d-flex align-items-center gap-3 px-3 py-2.5 rounded-3 text-decoration-none transition-all ${isActive('/analytics')}`}>
            <BarChart3 size={18} />
            <span className="fw-medium">Analytics</span>
          </Link>
          <Link to="/reports" className={`d-flex align-items-center gap-3 px-3 py-2.5 rounded-3 text-decoration-none transition-all ${isActive('/reports')}`}>
            <FileText size={18} />
            <span className="fw-medium">Reports</span>
          </Link>
        </>
      )}

      {user.role === 2 && (
        <Link to="/dashboard" className={`d-flex align-items-center gap-3 px-3 py-2.5 rounded-3 text-decoration-none transition-all ${isActive('/dashboard')}`}>
          <Wrench size={18} />
          <span className="fw-medium">Assigned Tickets</span>
        </Link>
      )}

      <hr className="border-secondary border-opacity-25 my-2" />

      <Link to="/profile" className={`d-flex align-items-center gap-3 px-3 py-2.5 rounded-3 text-decoration-none transition-all ${isActive('/profile')}`}>
        <User size={18} />
        <span className="fw-medium">My Profile</span>
      </Link>
    </div>
  );
};
