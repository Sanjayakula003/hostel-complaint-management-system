import React from 'react';
import { AlertCircle, Clock, CheckCircle2, ShieldAlert, Zap, Star } from 'lucide-react';

export const DashboardCards = ({ metrics }) => {
  if (!metrics) return null;

  const cards = [
    {
      title: 'Total Complaints',
      value: metrics.totalComplaints || 0,
      icon: <AlertCircle className="text-indigo-400" size={24} />,
      bgColor: 'rgba(99, 102, 241, 0.1)',
      borderColor: 'rgba(99, 102, 241, 0.2)',
    },
    {
      title: 'Pending Action',
      value: metrics.pendingCount || 0,
      icon: <Clock className="text-amber-400" size={24} />,
      bgColor: 'rgba(245, 158, 11, 0.1)',
      borderColor: 'rgba(245, 158, 11, 0.2)',
    },
    {
      title: 'Resolved',
      value: (metrics.resolvedCount || 0) + (metrics.closedCount || 0),
      icon: <CheckCircle2 className="text-emerald-400" size={24} />,
      bgColor: 'rgba(16, 185, 129, 0.1)',
      borderColor: 'rgba(16, 185, 129, 0.2)',
    },
    {
      title: 'Critical Priority',
      value: metrics.criticalCount || 0,
      icon: <ShieldAlert className="text-rose-400" size={24} />,
      bgColor: 'rgba(239, 68, 68, 0.1)',
      borderColor: 'rgba(239, 68, 68, 0.2)',
    },
    {
      title: 'Avg Resolution Time',
      value: `${(metrics.averageResolutionTimeHours || 0).toFixed(1)} hrs`,
      icon: <Zap className="text-cyan-400" size={24} />,
      bgColor: 'rgba(6, 182, 212, 0.1)',
      borderColor: 'rgba(6, 182, 212, 0.2)',
    },
    {
      title: 'Satisfaction Rating',
      value: metrics.averageRating ? `${metrics.averageRating.toFixed(1)} / 5` : 'N/A',
      icon: <Star className="text-yellow-400" size={24} />,
      bgColor: 'rgba(234, 179, 8, 0.1)',
      borderColor: 'rgba(234, 179, 8, 0.2)',
    },
  ];

  return (
    <div className="row g-3 mb-4">
      {cards.map((card, idx) => (
        <div key={idx} className="col-12 col-sm-6 col-lg-4">
          <div className="glass-card p-3 d-flex align-items-center justify-content-between h-100">
            <div>
              <span className="text-secondary small fw-semibold text-uppercase d-block mb-1">{card.title}</span>
              <h3 className="fw-bold mb-0 text-white">{card.value}</h3>
            </div>
            <div className="p-3 rounded-4 d-flex align-items-center justify-content-center" style={{ backgroundColor: card.bgColor, border: `1px solid ${card.borderColor}` }}>
              {card.icon}
            </div>
          </div>
        </div>
      ))}
    </div>
  );
};
