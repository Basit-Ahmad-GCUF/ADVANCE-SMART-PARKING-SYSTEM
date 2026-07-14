# Advanced Smart Parking Management System

## 1. Project Overview

The Advanced Smart Parking Management System is a robust C++ console application designed to automate modern parking facility operations. Built with strict adherence to Object-Oriented Programming (OOP) principles, the system handles dynamic fee calculations, tiered memberships, vehicle categorization, VIP slot allocation, and administrative analytics. It ensures data persistence across sessions using standard file I/O operations.

## 2. Core Features

### Role-Based Access Control:
A secure dual-login portal separates Standard Users (who park and checkout vehicles) from Administrators (who require a masked password for sensitive financial data).

### Dynamic Vehicle Parsing:
Supports Cars, Bikes, and Trucks with unique base rates.

### Smart Slot Allocation:
Automatically finds available spaces, prioritizing VIP slots for premium members.

### Tiered Memberships:
Users register with specific validity periods. The system applies dynamic discounts based on remaining validity (e.g., 20% off for >60 days).

### Automated Fee Calculation:
Computes totals using base rates, hours spent, VIP surcharges, late-night fees, and extra-time penalties.

### Administrative Dashboard:
Tracks revenue, counts parked vehicle types, and includes a secure database reset.

### Data Persistence:
Saves active parking states, member records, and stats to local text files (parked.txt, members.txt, stats.txt).

## 3. Object-Oriented Architecture

The system architecture leverages OOP pillars to create modular, scalable code.

 - Abstraction: The Vehicle acts as an abstract base class with pure virtual functions, hiding calculation logic from the main application flow.

 - Inheritance: Car, Bike, and Truck inherit shared attributes from the Vehicle base class, preventing duplication while allowing unique pricing.

 - Polymorphism: Through method overriding, the program dynamically determines at runtime whether to apply Car, Bike, or Truck rate logic during checkout.

 - Encapsulation: Sensitive data, like membership validity or a slot's VIP status, are private and accessed only via secure public methods.

## 4. Class Breakdown

 - UI: A static utility class handling console aesthetics and robust input validation.

 - Membership: Represents a registered user, holding contact info, VIP status, and discount logic.

 - Vehicle: Represents the physical entity occupying a slot, calculating its own checkout fee.

 - ParkingSlot: Represents a space. It uses Composition to safely manage a Vehicle's memory lifecycle.

 - ParkingSystem: The central engine orchestrating login routing, file I/O, and overall business logic.

## 5. Future Scope

This system establishes a highly structured backend. The architecture is decoupled—separating UI from business logic—to facilitate a future transition. The next development phase involves porting this backend into Qt Creator, replacing the console with a modern Graphical User Interface (GUI) featuring interactive buttons, dynamic data tables, and visual slot maps.

## 6. Credentials For Login

The Login Credential For Admin are :
```
admin123
```
