import React from 'react';

export const ReportTable = ({ staffPerformance, reportsData }) => {
  return (
    <div className="d-flex flex-column gap-4">
      {/* Staff Performance Report */}
      <div className="glass-card p-4">
        <h5 className="fw-bold text-white mb-3">Maintenance Staff Performance Report</h5>
        <div className="table-responsive">
          <table className="table custom-table">
            <thead>
              <tr>
                <th>Staff ID</th>
                <th>Name</th>
                <th>Department</th>
                <th>Assigned</th>
                <th>Resolved</th>
                <th>Resolution Rate</th>
                <th>Avg Resolution Time</th>
                <th>Avg Rating</th>
              </tr>
            </thead>
            <tbody>
              {(!staffPerformance || staffPerformance.length === 0) ? (
                <tr>
                  <td colSpan="8" className="text-center py-3 text-secondary">No staff performance data available.</td>
                </tr>
              ) : (
                staffPerformance.map((staff) => (
                  <tr key={staff.staffId}>
                    <td className="fw-mono text-indigo-400">{staff.staffId}</td>
                    <td className="fw-semibold text-white">{staff.staffName}</td>
                    <td className="text-secondary">{staff.department}</td>
                    <td>{staff.assignedCount}</td>
                    <td>{staff.resolvedCount}</td>
                    <td>
                      <div className="d-flex align-items-center gap-2">
                        <div className="progress flex-grow-1" style={{ height: '6px' }}>
                          <div
                            className="progress-bar bg-success"
                            style={{ width: `${staff.resolutionRatePercentage}%` }}
                          ></div>
                        </div>
                        <span className="small text-white">{staff.resolutionRatePercentage.toFixed(1)}%</span>
                      </div>
                    </td>
                    <td className="text-secondary">{staff.avgResolutionHours.toFixed(1)} hrs</td>
                    <td className="text-warning fw-semibold">{staff.avgRating.toFixed(1)} / 5</td>
                  </tr>
                ))
              )}
            </tbody>
          </table>
        </div>
      </div>

      {/* Generated Report Summaries */}
      {reportsData && (
        <div className="row g-4">
          <div className="col-md-6">
            <div className="glass-card p-4 h-100">
              <h6 className="fw-bold text-indigo-400 mb-3">Category-Wise Breakdown Report</h6>
              <pre className="text-white-50 bg-dark bg-opacity-50 p-3 rounded-3 fs-7" style={{ whiteSpace: 'pre-wrap' }}>
                {reportsData.categoryReport || 'No report data'}
              </pre>
            </div>
          </div>
          <div className="col-md-6">
            <div className="glass-card p-4 h-100">
              <h6 className="fw-bold text-cyan-400 mb-3">Room Hotspot Summary Report</h6>
              <pre className="text-white-50 bg-dark bg-opacity-50 p-3 rounded-3 fs-7" style={{ whiteSpace: 'pre-wrap' }}>
                {reportsData.roomReport || 'No report data'}
              </pre>
            </div>
          </div>
        </div>
      )}
    </div>
  );
};
