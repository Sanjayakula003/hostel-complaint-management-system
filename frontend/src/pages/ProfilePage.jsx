import React from 'react';
import { useAuth } from '../context/AuthContext';
import { User, Shield, GraduationCap, Wrench, Mail, Phone, Home, Building } from 'lucide-react';

export const ProfilePage = () => {
  const { user } = useAuth();

  if (!user) return null;

  return (
    <div className="container py-5">
      <div className="row justify-content-center">
        <div className="col-12 col-md-8 col-lg-6">
          <div className="glass-card p-4 p-md-5">
            <div className="text-center mb-4">
              <div className="bg-primary bg-opacity-20 text-primary p-3 rounded-circle d-inline-flex mb-3">
                <User size={48} />
              </div>
              <h3 className="fw-bold text-white mb-1">{user.name || user.username}</h3>
              <div className="d-flex justify-content-center gap-2 mt-2">
                {user.role === 0 && <span className="badge bg-primary px-3 py-1.5 fs-7">Student</span>}
                {user.role === 1 && <span className="badge bg-warning text-dark px-3 py-1.5 fs-7">Warden</span>}
                {user.role === 2 && <span className="badge bg-info text-dark px-3 py-1.5 fs-7">Maintenance Staff</span>}
              </div>
            </div>

            <div className="d-flex flex-column gap-3 py-3 border-top border-bottom border-secondary border-opacity-25 my-4">
              <div className="d-flex align-items-center gap-3 text-white">
                <div className="p-2 rounded-3 bg-secondary bg-opacity-20 text-secondary">
                  <User size={20} />
                </div>
                <div>
                  <span className="text-secondary small d-block">Username / User ID</span>
                  <span className="fw-semibold">{user.username} (ID: {user.id})</span>
                </div>
              </div>

              <div className="d-flex align-items-center gap-3 text-white">
                <div className="p-2 rounded-3 bg-secondary bg-opacity-20 text-secondary">
                  <Mail size={20} />
                </div>
                <div>
                  <span className="text-secondary small d-block">Email Address</span>
                  <span className="fw-semibold">{user.email || 'N/A'}</span>
                </div>
              </div>

              <div className="d-flex align-items-center gap-3 text-white">
                <div className="p-2 rounded-3 bg-secondary bg-opacity-20 text-secondary">
                  <Phone size={20} />
                </div>
                <div>
                  <span className="text-secondary small d-block">Phone Number</span>
                  <span className="fw-semibold">{user.phone || 'N/A'}</span>
                </div>
              </div>

              {user.role === 0 && (
                <>
                  <div className="d-flex align-items-center gap-3 text-white">
                    <div className="p-2 rounded-3 bg-secondary bg-opacity-20 text-secondary">
                      <GraduationCap size={20} />
                    </div>
                    <div>
                      <span className="text-secondary small d-block">Roll Number</span>
                      <span className="fw-semibold">{user.rollNumber || 'N/A'}</span>
                    </div>
                  </div>

                  <div className="d-flex align-items-center gap-3 text-white">
                    <div className="p-2 rounded-3 bg-secondary bg-opacity-20 text-secondary">
                      <Home size={20} />
                    </div>
                    <div>
                      <span className="text-secondary small d-block">Hostel Room & Block</span>
                      <span className="fw-semibold">Room {user.roomNumber} ({user.hostelBlock})</span>
                    </div>
                  </div>
                </>
              )}

              {user.role === 1 && (
                <div className="d-flex align-items-center gap-3 text-white">
                  <div className="p-2 rounded-3 bg-secondary bg-opacity-20 text-secondary">
                    <Building size={20} />
                  </div>
                  <div>
                    <span className="text-secondary small d-block">Assigned Hostel Block</span>
                    <span className="fw-semibold">{user.assignedBlock || 'All Blocks'}</span>
                  </div>
                </div>
              )}

              {user.role === 2 && (
                <div className="d-flex align-items-center gap-3 text-white">
                  <div className="p-2 rounded-3 bg-secondary bg-opacity-20 text-secondary">
                    <Wrench size={20} />
                  </div>
                  <div>
                    <span className="text-secondary small d-block">Maintenance Department</span>
                    <span className="fw-semibold">{user.department || 'General Maintenance'}</span>
                  </div>
                </div>
              )}
            </div>

            <div className="text-center text-secondary fs-7">
              Hostel Complaint Management System v2.0
            </div>
          </div>
        </div>
      </div>
    </div>
  );
};
