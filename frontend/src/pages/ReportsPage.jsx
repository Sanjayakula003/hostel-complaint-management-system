import React, { useState, useEffect } from 'react';
import { dashboardApi } from '../api/client';
import { ReportTable } from '../components/ReportTable';
import { FileText, Download } from 'lucide-react';

export const ReportsPage = () => {
  const [reportsData, setReportsData] = useState(null);
  const [staffPerformance, setStaffPerformance] = useState([]);
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    loadReports();
  }, []);

  const loadReports = async () => {
    setLoading(true);
    try {
      const res = await dashboardApi.getReports();
      if (res.success) {
        setReportsData(res);
        setStaffPerformance(res.staffPerformance || []);
      }
    } catch (e) {
      console.error(e);
    } finally {
      setLoading(false);
    }
  };

  return (
    <div className="container py-4">
      <div className="d-flex flex-column flex-md-row justify-content-between align-items-md-center gap-3 mb-4">
        <div>
          <h3 className="fw-bold text-white mb-1">Analytical Reports</h3>
          <p className="text-secondary small mb-0">Staff resolution metrics, category breakdowns & time analysis</p>
        </div>
      </div>

      <ReportTable staffPerformance={staffPerformance} reportsData={reportsData} />
    </div>
  );
};
