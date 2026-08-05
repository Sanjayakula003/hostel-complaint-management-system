import React from 'react';
import { BrowserRouter as Router, Routes, Route, Navigate } from 'react-router-dom';
import { AuthProvider, useAuth } from './context/AuthContext';
import { Navbar } from './components/Navbar';

import { LandingPage } from './pages/LandingPage';
import { StudentLogin } from './pages/StudentLogin';
import { WardenLogin } from './pages/WardenLogin';
import { StaffLogin } from './pages/StaffLogin';
import { StudentRegister } from './pages/StudentRegister';

import { StudentDashboard } from './pages/StudentDashboard';
import { WardenDashboard } from './pages/WardenDashboard';
import { StaffDashboard } from './pages/StaffDashboard';

import { RaiseComplaintPage } from './pages/RaiseComplaintPage';
import { TrackComplaintsPage } from './pages/TrackComplaintsPage';
import { ViewComplaintsPage } from './pages/ViewComplaintsPage';
import { AnalyticsPage } from './pages/AnalyticsPage';
import { ReportsPage } from './pages/ReportsPage';
import { ProfilePage } from './pages/ProfilePage';

const ProtectedRoute = ({ children, allowedRoles }) => {
  const { user } = useAuth();
  if (!user) return <Navigate to="/student-login" replace />;
  if (allowedRoles && !allowedRoles.includes(user.role)) {
    return <Navigate to="/dashboard" replace />;
  }
  return children;
};

const DashboardRouter = () => {
  const { user } = useAuth();
  if (!user) return <Navigate to="/student-login" replace />;
  if (user.role === 0) return <StudentDashboard />;
  if (user.role === 1) return <WardenDashboard />;
  if (user.role === 2) return <StaffDashboard />;
  return <Navigate to="/" replace />;
};

export const App = () => {
  return (
    <AuthProvider>
      <Router>
        <div className="min-vh-100 d-flex flex-column bg-dark text-light">
          <Navbar />
          <div className="flex-grow-1">
            <Routes>
              <Route path="/" element={<LandingPage />} />
              <Route path="/student-login" element={<StudentLogin />} />
              <Route path="/warden-login" element={<WardenLogin />} />
              <Route path="/staff-login" element={<StaffLogin />} />
              <Route path="/student-register" element={<StudentRegister />} />

              <Route path="/dashboard" element={<DashboardRouter />} />

              {/* Student Routes */}
              <Route
                path="/raise-complaint"
                element={
                  <ProtectedRoute allowedRoles={[0]}>
                    <RaiseComplaintPage />
                  </ProtectedRoute>
                }
              />
              <Route
                path="/track-complaints"
                element={
                  <ProtectedRoute allowedRoles={[0]}>
                    <TrackComplaintsPage />
                  </ProtectedRoute>
                }
              />

              {/* Warden Routes */}
              <Route
                path="/view-complaints"
                element={
                  <ProtectedRoute allowedRoles={[1]}>
                    <ViewComplaintsPage />
                  </ProtectedRoute>
                }
              />
              <Route
                path="/analytics"
                element={
                  <ProtectedRoute allowedRoles={[1]}>
                    <AnalyticsPage />
                  </ProtectedRoute>
                }
              />
              <Route
                path="/reports"
                element={
                  <ProtectedRoute allowedRoles={[1]}>
                    <ReportsPage />
                  </ProtectedRoute>
                }
              />

              {/* Shared Routes */}
              <Route
                path="/profile"
                element={
                  <ProtectedRoute>
                    <ProfilePage />
                  </ProtectedRoute>
                }
              />

              <Route path="*" element={<Navigate to="/" replace />} />
            </Routes>
          </div>
        </div>
      </Router>
    </AuthProvider>
  );
};

export default App;
