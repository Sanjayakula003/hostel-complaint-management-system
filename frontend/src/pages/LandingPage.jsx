import React from 'react';
import { Link } from 'react-router-dom';
import { Building2, GraduationCap, Shield, Wrench, ArrowRight, Zap, CheckCircle2, ShieldCheck, Clock, BarChart2 } from 'lucide-react';

export const LandingPage = () => {
  return (
    <div className="py-5">
      {/* Hero Section */}
      <div className="container py-5">
        <div className="row align-items-center g-5">
          <div className="col-lg-7">
            <div className="badge bg-indigo-500 bg-opacity-20 text-indigo-300 border border-indigo-500 border-opacity-30 rounded-pill px-3 py-2 mb-4 d-inline-flex align-items-center gap-2">
              <Zap size={16} /> Next-Gen Smart Hostel Governance Platform
            </div>
            <h1 className="display-4 fw-extrabold text-white mb-3 lh-sm">
              Streamlined Hostel <span className="gradient-text">Complaint & Issue</span> Management
            </h1>
            <p className="lead text-secondary mb-4">
              Empowering students, wardens, and maintenance staff with an automated C++ powered backend, real-time ticket tracking, duplicate issue detection, and intelligent analytics.
            </p>

            <div className="d-flex flex-wrap gap-3 mb-5">
              <Link to="/student-login" className="btn btn-primary-gradient btn-lg rounded-3 px-4 py-3 d-flex align-items-center gap-2">
                <GraduationCap size={20} /> Student Portal <ArrowRight size={18} />
              </Link>
              <Link to="/warden-login" className="btn btn-outline-light btn-lg rounded-3 px-4 py-3 d-flex align-items-center gap-2">
                <Shield size={20} /> Warden Dashboard
              </Link>
              <Link to="/staff-login" className="btn btn-outline-info btn-lg rounded-3 px-4 py-3 d-flex align-items-center gap-2">
                <Wrench size={20} /> Staff Portal
              </Link>
            </div>

            <div className="row g-3 pt-3 border-top border-secondary border-opacity-25">
              <div className="col-sm-4">
                <div className="d-flex align-items-center gap-2 text-white">
                  <CheckCircle2 size={18} className="text-success" />
                  <span className="fw-medium">24/7 Issue Raising</span>
                </div>
              </div>
              <div className="col-sm-4">
                <div className="d-flex align-items-center gap-2 text-white">
                  <ShieldCheck size={18} className="text-indigo-400" />
                  <span className="fw-medium">Auto-Duplicate Check</span>
                </div>
              </div>
              <div className="col-sm-4">
                <div className="d-flex align-items-center gap-2 text-white">
                  <BarChart2 size={18} className="text-info" />
                  <span className="fw-medium">Analytical Reports</span>
                </div>
              </div>
            </div>
          </div>

          <div className="col-lg-5">
            <div className="glass-card p-4 text-center">
              <div className="mb-4">
                <div className="bg-primary bg-opacity-20 text-primary p-3 rounded-circle d-inline-flex mb-3">
                  <Building2 size={48} />
                </div>
                <h4 className="fw-bold text-white mb-1">Quick Access Portals</h4>
                <p className="text-secondary small">Choose your user role to log into your portal</p>
              </div>

              <div className="d-flex flex-column gap-3">
                <Link to="/student-login" className="glass-card p-3 d-flex align-items-center justify-content-between text-decoration-none text-white hover-border-primary">
                  <div className="d-flex align-items-center gap-3">
                    <div className="p-2 rounded-3 bg-primary bg-opacity-20 text-primary">
                      <GraduationCap size={24} />
                    </div>
                    <div className="text-start">
                      <div className="fw-bold">Student Portal</div>
                      <div className="text-secondary fs-7">Raise & Track Room Complaints</div>
                    </div>
                  </div>
                  <ArrowRight size={18} className="text-secondary" />
                </Link>

                <Link to="/warden-login" className="glass-card p-3 d-flex align-items-center justify-content-between text-decoration-none text-white hover-border-warning">
                  <div className="d-flex align-items-center gap-3">
                    <div className="p-2 rounded-3 bg-warning bg-opacity-20 text-warning">
                      <Shield size={24} />
                    </div>
                    <div className="text-start">
                      <div className="fw-bold">Warden Portal</div>
                      <div className="text-secondary fs-7">Manage Tickets, Assign & Analytics</div>
                    </div>
                  </div>
                  <ArrowRight size={18} className="text-secondary" />
                </Link>

                <Link to="/staff-login" className="glass-card p-3 d-flex align-items-center justify-content-between text-decoration-none text-white hover-border-info">
                  <div className="d-flex align-items-center gap-3">
                    <div className="p-2 rounded-3 bg-info bg-opacity-20 text-info">
                      <Wrench size={24} />
                    </div>
                    <div className="text-start">
                      <div className="fw-bold">Maintenance Staff</div>
                      <div className="text-secondary fs-7">View Assigned Jobs & Update Status</div>
                    </div>
                  </div>
                  <ArrowRight size={18} className="text-secondary" />
                </Link>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  );
};
