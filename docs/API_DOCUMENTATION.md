# Hostel Complaint Management System - REST API Documentation

Base URL: `http://localhost:8080/api`

---

## 1. Authentication Endpoints

### `POST /api/login`
Authenticates students, wardens, or maintenance staff.

**Request Body:**
```json
{
  "username": "student1",
  "password": "pass123",
  "role": "STUDENT" // "STUDENT" | "WARDEN" | "STAFF"
}
```

**Response (200 OK):**
```json
{
  "success": true,
  "message": "Login successful",
  "user": {
    "id": "U1001",
    "username": "student1",
    "name": "Rahul Sharma",
    "email": "rahul@hostel.edu",
    "phone": "9876543210",
    "role": 0,
    "roleStr": "STUDENT",
    "rollNumber": "CS2026-042",
    "roomNumber": "A-101",
    "hostelBlock": "Block-A"
  }
}
```

---

### `POST /api/register`
Registers a new student account.

**Request Body:**
```json
{
  "username": "newstudent",
  "password": "securepass",
  "name": "Alex Johnson",
  "email": "alex@hostel.edu",
  "phone": "9876543211",
  "rollNumber": "CS2026-099",
  "roomNumber": "B-204",
  "hostelBlock": "Block-B"
}
```

---

## 2. Complaint Endpoints

### `GET /api/complaints`
Retrieves complaint tickets. Pass `X-Student-Id` or `X-Staff-Id` headers to filter per user.

---

### `POST /api/complaints/check-duplicate`
Checks for recent active duplicate complaints in the same room.

**Request Body:**
```json
{
  "roomNumber": "A-101",
  "category": "Electrical"
}
```

---

### `POST /api/complaints`
Raises a new complaint.

**Request Body:**
```json
{
  "studentId": "U1001",
  "studentName": "Rahul Sharma",
  "roomNumber": "A-101",
  "category": "Electrical",
  "title": "Ceiling fan not working",
  "description": "The fan in room A-101 makes noise and stopped rotating."
}
```

---

### `PUT /api/complaints/{id}`
Updates a complaint status or assigns staff.

**Actions:**
- `assign_staff`: `{ "action": "assign_staff", "staffId": "U3001", "staffName": "Ramesh Kumar" }`
- `mark_in_progress`: `{ "action": "mark_in_progress", "staffId": "U3001" }`
- `mark_resolved`: `{ "action": "mark_resolved", "staffId": "U3001", "remarks": "Capacitor replaced" }`
- `close`: `{ "action": "close", "studentId": "U1001", "rating": 5, "comments": "Quick repair!" }`

---

### `DELETE /api/complaints/{id}`
Deletes a complaint ticket.

---

## 3. Analytics & Dashboard

### `GET /api/dashboard`
Returns live system metrics and summary counts.

### `GET /api/analytics`
Returns full metric object including category breakdowns, room hotspots, and monthly trends.

### `GET /api/reports`
Generates time-based, category-wise, room-wise, and staff performance metrics.

### `GET /api/staff`
Returns list of all registered maintenance staff users.
