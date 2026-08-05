import React from 'react';
import {
  Chart as ChartJS,
  CategoryScale,
  LinearScale,
  BarElement,
  PointElement,
  LineElement,
  ArcElement,
  Title,
  Tooltip,
  Legend,
} from 'chart.js';
import { Bar, Pie, Line } from 'react-chartjs-2';

ChartJS.register(
  CategoryScale,
  LinearScale,
  BarElement,
  PointElement,
  LineElement,
  ArcElement,
  Title,
  Tooltip,
  Legend
);

export const AnalyticsCards = ({ analytics }) => {
  if (!analytics) return null;

  // 1. Category Distribution Bar Chart
  const categoryLabels = analytics.categoryBreakdown ? analytics.categoryBreakdown.map(c => c.category) : [];
  const categoryCounts = analytics.categoryBreakdown ? analytics.categoryBreakdown.map(c => c.count) : [];

  const barData = {
    labels: categoryLabels.length > 0 ? categoryLabels : ['Electrical', 'Plumbing', 'Carpentry', 'Internet/Wi-Fi', 'Cleaning'],
    datasets: [
      {
        label: 'Number of Complaints',
        data: categoryCounts.length > 0 ? categoryCounts : [12, 8, 5, 15, 6],
        backgroundColor: [
          'rgba(99, 102, 241, 0.7)',
          'rgba(6, 182, 212, 0.7)',
          'rgba(245, 158, 11, 0.7)',
          'rgba(168, 85, 247, 0.7)',
          'rgba(16, 185, 129, 0.7)',
        ],
        borderColor: [
          '#6366f1',
          '#06b6d4',
          '#f59e0b',
          '#a855f7',
          '#10b981',
        ],
        borderWidth: 1,
        borderRadius: 8,
      },
    ],
  };

  // 2. Status Distribution Pie Chart
  const pieData = {
    labels: ['Pending', 'Assigned', 'In Progress', 'Resolved', 'Closed'],
    datasets: [
      {
        data: [
          analytics.pendingCount || 0,
          analytics.assignedCount || 0,
          analytics.inProgressCount || 0,
          analytics.resolvedCount || 0,
          analytics.closedCount || 0,
        ],
        backgroundColor: [
          'rgba(245, 158, 11, 0.8)',
          'rgba(59, 130, 246, 0.8)',
          'rgba(168, 85, 247, 0.8)',
          'rgba(16, 185, 129, 0.8)',
          'rgba(148, 163, 184, 0.8)',
        ],
        borderWidth: 1,
      },
    ],
  };

  // 3. Monthly Trends Line Chart
  const monthlyLabels = analytics.monthlyStats ? analytics.monthlyStats.map(m => m.yearMonth) : ['2026-05', '2026-06', '2026-07', '2026-08'];
  const monthlyTotals = analytics.monthlyStats ? analytics.monthlyStats.map(m => m.totalComplaints) : [15, 22, 18, 30];
  const monthlyResolved = analytics.monthlyStats ? analytics.monthlyStats.map(m => m.resolvedComplaints) : [12, 19, 15, 24];

  const lineData = {
    labels: monthlyLabels,
    datasets: [
      {
        label: 'Total Registered',
        data: monthlyTotals,
        borderColor: '#818cf8',
        backgroundColor: 'rgba(129, 140, 248, 0.2)',
        tension: 0.3,
        fill: true,
      },
      {
        label: 'Resolved',
        data: monthlyResolved,
        borderColor: '#34d399',
        backgroundColor: 'rgba(52, 211, 153, 0.2)',
        tension: 0.3,
        fill: true,
      },
    ],
  };

  const chartOptions = {
    responsive: true,
    plugins: {
      legend: {
        labels: { color: '#94a3b8' },
      },
    },
    scales: {
      x: { ticks: { color: '#94a3b8' }, grid: { color: 'rgba(255, 255, 255, 0.05)' } },
      y: { ticks: { color: '#94a3b8' }, grid: { color: 'rgba(255, 255, 255, 0.05)' } },
    },
  };

  return (
    <div className="row g-4">
      <div className="col-12 col-lg-6">
        <div className="glass-card p-4 h-100">
          <h6 className="fw-bold text-white mb-3">Category Breakdown (Bar Chart)</h6>
          <Bar data={barData} options={chartOptions} />
        </div>
      </div>

      <div className="col-12 col-lg-6">
        <div className="glass-card p-4 h-100">
          <h6 className="fw-bold text-white mb-3">Complaint Status Distribution (Pie Chart)</h6>
          <div style={{ maxHeight: '300px' }} className="d-flex justify-content-center">
            <Pie data={pieData} options={{ responsive: true, plugins: { legend: { labels: { color: '#94a3b8' } } } }} />
          </div>
        </div>
      </div>

      <div className="col-12">
        <div className="glass-card p-4">
          <h6 className="fw-bold text-white mb-3">Monthly Complaint Trend (Line Chart)</h6>
          <Line data={lineData} options={chartOptions} />
        </div>
      </div>
    </div>
  );
};
