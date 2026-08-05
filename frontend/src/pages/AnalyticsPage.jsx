import React, { useState, useEffect } from 'react';
import { dashboardApi } from '../api/client';
import { AnalyticsCards } from '../components/AnalyticsCards';
import { BarChart3, TrendingUp, AlertTriangle } from 'lucide-react';

export const AnalyticsPage = () => {
  const [analytics, setAnalytics] = useState(null);
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    loadAnalytics();
  }, []);

  const loadAnalytics = async () => {
    setLoading(true);
    try {
      const res = await dashboardApi.getAnalytics();
      if (res.success) setAnalytics(res.analytics);
    } catch (e) {
      console.error(e);
    } finally {
      setLoading(false);
    }
  };

  return (
    <div className="container py-4">
      <div className="d-flex align-items-center gap-3 mb-4">
        <div className="bg-primary bg-opacity-20 text-primary p-3 rounded-circle">
          <BarChart3 size={32} />
        </div>
        <div>
          <h3 className="fw-bold text-white mb-1">Hostel Analytics Dashboard</h3>
          <p className="text-secondary small mb-0">Visual data breakdown generated from C++ STL analytics engine</p>
        </div>
      </div>

      <AnalyticsCards analytics={analytics} />

      {/* Room Hotspot List */}
      {analytics && analytics.topHotspotRooms && (
        <div className="glass-card p-4 mt-4">
          <h5 className="fw-bold text-white mb-3 d-flex align-items-center gap-2">
            <AlertTriangle className="text-warning" size={20} /> High Complaint Frequency Room Hotspots
          </h5>
          <div className="row g-3">
            {analytics.topHotspotRooms.map((h, i) => (
              <div key={i} className="col-12 col-sm-6 col-md-4 col-lg-3">
                <div className="p-3 rounded-3 bg-dark bg-opacity-50 border border-secondary border-opacity-25 d-flex justify-content-between align-items-center">
                  <div>
                    <span className="text-secondary small d-block">Room Number</span>
                    <strong className="fs-5 text-white">{h.roomNumber}</strong>
                  </div>
                  <span className="badge bg-danger bg-opacity-20 text-danger border border-danger border-opacity-30 rounded-pill px-3 py-2">
                    {h.complaintCount} Tickets
                  </span>
                </div>
              </div>
            ))}
          </div>
        </div>
      )}
    </div>
  );
};
