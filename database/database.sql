-- ==================================================================
-- SMART HOSTEL COMPLAINT MANAGEMENT SYSTEM
-- MySQL 8.0 Relational Database Schema & Sample Data Script
-- ==================================================================

CREATE DATABASE IF NOT EXISTS hostel_cms;
USE hostel_cms;

-- Drop existing tables to ensure clean setup
DROP TABLE IF EXISTS Feedback;
DROP TABLE IF EXISTS ComplaintHistory;
DROP TABLE IF EXISTS Complaints;
DROP TABLE IF EXISTS Staff;
DROP TABLE IF EXISTS Wardens;
DROP TABLE IF EXISTS Students;
DROP TABLE IF EXISTS Users;

-- 1. Base Users Table
CREATE TABLE Users (
    id VARCHAR(20) PRIMARY KEY,
    username VARCHAR(50) NOT NULL UNIQUE,
    password_hash VARCHAR(128) NOT NULL,
    name VARCHAR(100) NOT NULL,
    email VARCHAR(100) NOT NULL,
    phone VARCHAR(20) NOT NULL,
    role ENUM('Student', 'Warden', 'Staff') NOT NULL,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    INDEX idx_username (username),
    INDEX idx_role (role)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 2. Students Table
CREATE TABLE Students (
    user_id VARCHAR(20) PRIMARY KEY,
    roll_number VARCHAR(30) NOT NULL UNIQUE,
    room_number VARCHAR(20) NOT NULL,
    hostel_block VARCHAR(10) NOT NULL,
    FOREIGN KEY (user_id) REFERENCES Users(id) ON DELETE CASCADE ON UPDATE CASCADE,
    INDEX idx_room (room_number),
    INDEX idx_block (hostel_block)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 3. Wardens Table
CREATE TABLE Wardens (
    user_id VARCHAR(20) PRIMARY KEY,
    hostel_block_assigned VARCHAR(10) NOT NULL,
    office_room VARCHAR(30) NOT NULL,
    FOREIGN KEY (user_id) REFERENCES Users(id) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 4. Staff Table
CREATE TABLE Staff (
    user_id VARCHAR(20) PRIMARY KEY,
    department VARCHAR(50) NOT NULL,
    status VARCHAR(20) DEFAULT 'Available',
    FOREIGN KEY (user_id) REFERENCES Users(id) ON DELETE CASCADE ON UPDATE CASCADE,
    INDEX idx_department (department)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 5. Complaints Table
CREATE TABLE Complaints (
    id VARCHAR(20) PRIMARY KEY,
    student_id VARCHAR(20) NOT NULL,
    room_number VARCHAR(20) NOT NULL,
    category ENUM('Electrical', 'Plumbing', 'WiFi', 'Furniture', 'Cleaning', 'Water Supply', 'Mess', 'Security', 'Other') NOT NULL,
    title VARCHAR(150) NOT NULL,
    description TEXT NOT NULL,
    priority ENUM('Low', 'Medium', 'High', 'Critical') NOT NULL DEFAULT 'Medium',
    status ENUM('Pending', 'Assigned', 'In Progress', 'Resolved', 'Closed') NOT NULL DEFAULT 'Pending',
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    resolved_at DATETIME NULL,
    assigned_staff_id VARCHAR(20) NULL,
    remarks TEXT NULL,
    is_escalated BOOLEAN DEFAULT FALSE,
    FOREIGN KEY (student_id) REFERENCES Users(id) ON DELETE CASCADE,
    FOREIGN KEY (assigned_staff_id) REFERENCES Users(id) ON DELETE SET NULL,
    INDEX idx_complaint_status (status),
    INDEX idx_complaint_priority (priority),
    INDEX idx_complaint_category (category),
    INDEX idx_complaint_student (student_id),
    INDEX idx_complaint_room (room_number),
    INDEX idx_created_at (created_at)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 6. Complaint History Audit Table
CREATE TABLE ComplaintHistory (
    id INT AUTO_INCREMENT PRIMARY KEY,
    complaint_id VARCHAR(20) NOT NULL,
    previous_status VARCHAR(30) NOT NULL,
    new_status VARCHAR(30) NOT NULL,
    updated_by VARCHAR(20) NOT NULL,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    remarks TEXT NULL,
    FOREIGN KEY (complaint_id) REFERENCES Complaints(id) ON DELETE CASCADE,
    FOREIGN KEY (updated_by) REFERENCES Users(id) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 7. Feedback Table
CREATE TABLE Feedback (
    id VARCHAR(20) PRIMARY KEY,
    complaint_id VARCHAR(20) NOT NULL UNIQUE,
    student_id VARCHAR(20) NOT NULL,
    rating INT NOT NULL CHECK (rating >= 1 AND rating <= 5),
    comments TEXT NULL,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (complaint_id) REFERENCES Complaints(id) ON DELETE CASCADE,
    FOREIGN KEY (student_id) REFERENCES Users(id) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- ==================================================================
-- SAMPLE SEED DATA
-- Default Hashed Password for all demo users: Pass@123 -> Salted SHA256 Hash
-- ==================================================================

-- Insert Users
INSERT INTO Users (id, username, password_hash, name, email, phone, role, created_at) VALUES
('W101', 'warden_rahul', '16f001ef07a97491cf0eb96bcfa82d7734ea0c2ed1bd55b0a3c20042f4c6e9a6', 'Dr. Rahul Sharma', 'rahul.warden@hostel.edu', '+919876543210', 'Warden', '2026-08-01 08:00:00'),
('W102', 'warden_priya', '16f001ef07a97491cf0eb96bcfa82d7734ea0c2ed1bd55b0a3c20042f4c6e9a6', 'Dr. Priya Verma', 'priya.warden@hostel.edu', '+919876543211', 'Warden', '2026-08-01 08:00:00'),
('ST101', 'staff_ramesh', '16f001ef07a97491cf0eb96bcfa82d7734ea0c2ed1bd55b0a3c20042f4c6e9a6', 'Ramesh Kumar', 'ramesh.electric@hostel.edu', '+919123456780', 'Staff', '2026-08-01 08:00:00'),
('ST102', 'staff_suresh', '16f001ef07a97491cf0eb96bcfa82d7734ea0c2ed1bd55b0a3c20042f4c6e9a6', 'Suresh Patel', 'suresh.plumb@hostel.edu', '+919123456781', 'Staff', '2026-08-01 08:00:00'),
('ST103', 'staff_anita', '16f001ef07a97491cf0eb96bcfa82d7734ea0c2ed1bd55b0a3c20042f4c6e9a6', 'Anita Roy', 'anita.wifi@hostel.edu', '+919123456782', 'Staff', '2026-08-01 08:00:00'),
('ST104', 'staff_vikram', '16f001ef07a97491cf0eb96bcfa82d7734ea0c2ed1bd55b0a3c20042f4c6e9a6', 'Vikram Singh', 'vikram.carpenter@hostel.edu', '+919123456783', 'Staff', '2026-08-01 08:00:00'),
('S101', 'student_aarav', '16f001ef07a97491cf0eb96bcfa82d7734ea0c2ed1bd55b0a3c20042f4c6e9a6', 'Aarav Gupta', 'aarav@student.edu', '+919988776655', 'Student', '2026-08-01 08:00:00'),
('S102', 'student_diya', '16f001ef07a97491cf0eb96bcfa82d7734ea0c2ed1bd55b0a3c20042f4c6e9a6', 'Diya Sharma', 'diya@student.edu', '+919988776656', 'Student', '2026-08-01 08:00:00'),
('S103', 'student_karan', '16f001ef07a97491cf0eb96bcfa82d7734ea0c2ed1bd55b0a3c20042f4c6e9a6', 'Karan Mehta', 'karan@student.edu', '+919988776657', 'Student', '2026-08-01 08:00:00');

-- Insert Wardens
INSERT INTO Wardens (user_id, hostel_block_assigned, office_room) VALUES
('W101', 'A', 'Warden Office A-101'),
('W102', 'B', 'Warden Office B-102');

-- Insert Staff
INSERT INTO Staff (user_id, department, status) VALUES
('ST101', 'Electrical', 'Available'),
('ST102', 'Plumbing', 'Available'),
('ST103', 'WiFi', 'Available'),
('ST104', 'Furniture', 'Available');

-- Insert Students
INSERT INTO Students (user_id, roll_number, room_number, hostel_block) VALUES
('S101', 'CS2026-01', 'A-204', 'A'),
('S102', 'EC2026-15', 'B-305', 'B'),
('S103', 'ME2026-42', 'A-108', 'A');

-- Insert Complaints
INSERT INTO Complaints (id, student_id, room_number, category, title, description, priority, status, created_at, updated_at, resolved_at, assigned_staff_id, remarks, is_escalated) VALUES
('CMP-1001', 'S101', 'A-204', 'Electrical', 'Short Circuit in Switchboard', 'Sparks coming out of main switchboard in room.', 'Critical', 'Assigned', '2026-08-01 10:00:00', '2026-08-01 11:00:00', NULL, 'ST101', 'Inspecting wiring.', FALSE),
('CMP-1002', 'S102', 'B-305', 'Plumbing', 'Severe Water Leakage', 'Bathroom pipe leaking continuously onto floor.', 'High', 'Pending', '2026-08-02 09:30:00', '2026-08-02 09:30:00', NULL, NULL, NULL, FALSE),
('CMP-1003', 'S103', 'A-108', 'WiFi', 'No Internet Connection', 'WiFi access point not broadcasting SSID in block A.', 'Medium', 'In Progress', '2026-08-02 14:20:00', '2026-08-03 10:00:00', NULL, 'ST103', 'Resetting router configuration.', FALSE),
('CMP-1004', 'S101', 'A-204', 'Furniture', 'Broken Chair Leg', 'Study chair leg cracked and unsafe to sit.', 'Low', 'Resolved', '2026-07-28 16:00:00', '2026-07-29 12:00:00', '2026-07-29 12:00:00', 'ST104', 'Replaced with new study chair.', FALSE),
('CMP-1005', 'S102', 'B-305', 'Cleaning', 'Room Cleaning Request', 'Request deep cleaning of corridor and room dustbin.', 'Low', 'Closed', '2026-07-25 11:00:00', '2026-07-26 15:00:00', '2026-07-26 15:00:00', 'ST102', 'Cleaning completed.', FALSE);

-- Insert Feedback
INSERT INTO Feedback (id, complaint_id, student_id, rating, comments, created_at) VALUES
('FB-1001', 'CMP-1004', 'S101', 5, 'Fast resolution and great service by Vikram!', '2026-07-29 14:00:00'),
('FB-1002', 'CMP-1005', 'S102', 4, 'Room is clean now. Thanks.', '2026-07-26 16:30:00');
