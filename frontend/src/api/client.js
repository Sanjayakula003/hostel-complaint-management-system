const API_BASE = '/api';

export async function apiRequest(endpoint, options = {}) {
  const defaultHeaders = {
    'Content-Type': 'application/json',
  };

  const userStr = localStorage.getItem('hostel_user');
  if (userStr) {
    try {
      const user = JSON.parse(userStr);
      if (user.role === 0 && user.id) defaultHeaders['X-Student-Id'] = user.id;
      if (user.role === 2 && user.id) defaultHeaders['X-Staff-Id'] = user.id;
    } catch (e) {
      console.error('Failed to parse cached user token', e);
    }
  }

  const config = {
    ...options,
    headers: {
      ...defaultHeaders,
      ...options.headers,
    },
  };

  if (config.body && typeof config.body === 'object') {
    config.body = JSON.stringify(config.body);
  }

  const response = await fetch(`${API_BASE}${endpoint}`, config);
  const data = await response.json().catch(() => ({}));

  if (!response.ok) {
    throw new Error(data.error || `HTTP Error ${response.status}`);
  }
  return data;
}

export const authApi = {
  login: (credentials) => apiRequest('/login', { method: 'POST', body: credentials }),
  register: (studentData) => apiRequest('/register', { method: 'POST', body: studentData }),
};

export const complaintApi = {
  getComplaints: (headers = {}) => apiRequest('/complaints', { method: 'GET', headers }),
  getComplaintById: (id) => apiRequest(`/complaints/${id}`, { method: 'GET' }),
  checkDuplicate: (roomNumber, category) => apiRequest('/complaints/check-duplicate', { method: 'POST', body: { roomNumber, category } }),
  createComplaint: (data) => apiRequest('/complaints', { method: 'POST', body: data }),
  updateComplaint: (id, data) => apiRequest(`/complaints/${id}`, { method: 'PUT', body: data }),
  deleteComplaint: (id) => apiRequest(`/complaints/${id}`, { method: 'DELETE' }),
};

export const dashboardApi = {
  getDashboard: () => apiRequest('/dashboard', { method: 'GET' }),
  getAnalytics: () => apiRequest('/analytics', { method: 'GET' }),
  getReports: () => apiRequest('/reports', { method: 'GET' }),
  getStaffList: () => apiRequest('/staff', { method: 'GET' }),
  submitFeedback: (feedbackData) => apiRequest('/feedback', { method: 'POST', body: feedbackData }),
};
