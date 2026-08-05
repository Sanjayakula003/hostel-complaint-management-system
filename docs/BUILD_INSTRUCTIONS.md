# Hostel Complaint Management System - Build & Run Instructions

## Prerequisites
- C++ Compiler (GCC 7+, Clang, or MSVC 2019+) supporting C++17
- CMake 3.14+
- Node.js 16+ & npm

---

## 1. C++ Backend Setup & Build

Navigate to the `backend/` directory:

```bash
cd backend
cmake -B build
cmake --build build --config Debug
```

### Running the C++ REST Backend Server
```bash
# On Windows
.\build\Debug\HostelCMSBackend.exe

# On Linux / macOS
./build/HostelCMSBackend
```
The backend server will start on `http://localhost:8080`.

---

## 2. React Frontend Setup & Run

Navigate to the `frontend/` directory:

```bash
cd frontend
npm install
npm run dev
```

The frontend web application will start on `http://localhost:3000`.

---

## Default User Accounts for Testing

| Role | Username | Password | Notes |
|---|---|---|---|
| Student | `student1` | `pass123` | Room A-101 (Rahul Sharma) |
| Student | `student2` | `pass123` | Room B-202 (Priya Singh) |
| Warden | `warden1` | `admin123` | Hostel Warden Admin |
| Staff | `staff1` | `staff123` | Electrical Dept Staff |
| Staff | `staff2` | `staff123` | Plumbing Dept Staff |
