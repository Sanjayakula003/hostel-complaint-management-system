# Project Abstract - Smart Hostel Complaint Management System

## Abstract

The **Smart Hostel Complaint Management System** is a production-quality C++17 console application backed by MySQL 8.0 designed to modernize and digitize hostel maintenance workflows. In traditional educational hostel setups, complaint submission relies on paper registers or manual oral reporting, leading to delayed maintenance, untracked service requests, lack of accountability, and zero analytical visibility into recurring maintenance issues.

This system replaces manual practices with an intelligent multi-tier digital platform supporting three primary user roles: **Students**, **Wardens**, and **Maintenance Staff**. 

Key technical innovations include:
1. **Rule-Based Priority Classification Engine**: Automatically evaluates complaint title and category keywords to assign priority tiers (`Critical`, `High`, `Medium`, `Low`).
2. **Automated SLA Escalation Engine**: Detects delayed active complaints beyond threshold time windows and escalates priority automatically while generating warden warning alerts.
3. **Duplicate Complaint Suppression**: Prevents duplicate spam submissions for the same room and facility category within configurable time windows.
4. **Data Structure Optimizations**: Leverages `std::vector`, `std::unordered_map` (O(1) lookup), `std::queue` (FIFO processing buffer), `std::priority_queue` (Max-heap for critical complaints), `std::sort`, and `std::binary_search`.
5. **Robust Database Layer**: Supports MySQL 8.0 prepared statements alongside a dual-mode embedded file storage fallback.

The solution provides executive analytics, hotspot room detection, category breakdowns, staff efficiency ranking, and tabular reporting (Daily, Weekly, Monthly, Yearly).
