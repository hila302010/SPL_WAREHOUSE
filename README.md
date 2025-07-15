# 🏪 Warehouse Management System

A comprehensive C++ warehouse management system that simulates order processing with customers, volunteers, and logistics operations.

## 📋 Overview

This project implements a warehouse management system that handles:

- **Customer Management**: Different types of customers with varying order limits
- **Volunteer Coordination**: Collectors and drivers with different capabilities
- **Order Processing**: Complete order lifecycle from placement to delivery
- **Action System**: Command-based operations for system interaction

## 🏗️ Project Structure

```
SPL_WAREHOUSE/
├── src/                    # Source files
│   ├── main.cpp           # Main program entry point
│   ├── WareHouse.cpp      # Core warehouse management
│   ├── Customer.cpp       # Customer implementations
│   ├── Volunteer.cpp      # Volunteer implementations
│   ├── Order.cpp          # Order management
│   └── Action.cpp         # Action command implementations
├── include/               # Header files
│   ├── WareHouse.h        # Warehouse class definitions
│   ├── Customer.h         # Customer class hierarchy
│   ├── Volunteer.h        # Volunteer class hierarchy
│   ├── Order.h            # Order management
│   └── Action.h           # Action command system
├── bin/                   # Compiled binaries and objects
├── .vscode/              # VS Code configuration
├── configFileExample.txt # Sample configuration
└── makefile              # Build configuration
```

## 🚀 Features

### Customer Types

- **👨‍🎖️ Soldier Customers**: Military personnel with specific ordering patterns
- **👤 Civilian Customers**: Regular customers with standard ordering limits

### Volunteer Roles

- **📦 Collectors**: Responsible for gathering orders from warehouse
  - Regular collectors (unlimited orders)
  - Limited collectors (with max order limits)
- **🚚 Drivers**: Handle delivery to customers
  - Regular drivers (unlimited deliveries)
  - Limited drivers (with max delivery limits)

### Order Management

- Order placement and tracking
- Status updates throughout the process
- Distance-based delivery calculations
- Volunteer assignment optimization

### Action System

- Command-based interface for system operations
- Support for various warehouse actions
- Action history and logging

## 🛠️ Building and Running

### Prerequisites

- **C++ Compiler**: g++ with C++11 support
- **Make**: For build automation
- **Git**: For version control

### Build Instructions

1. **Clone the repository**:

   ```bash
   git clone https://github.com/hila302010/SPL_WAREHOUSE.git
   cd SPL_WAREHOUSE
   ```

2. **Compile the project**:

   ```bash
   make
   ```

3. **Run the program**:
   ```bash
   ./bin/run configFileExample.txt
   ```

### Build Targets

- `make all` - Clean and build the project
- `make run` - Build the executable
- `make clean` - Remove compiled files

## ⚙️ Configuration

The system uses a configuration file to set up initial customers and volunteers. See `configFileExample.txt` for the format:

### Customer Configuration

```
customer <name> <type> <distance> <max_orders>
```

- **name**: Customer identifier
- **type**: `soldier` or `civilian`
- **distance**: Distance from warehouse
- **max_orders**: Maximum orders this customer can place

Example:

```
customer Moshe soldier 3 2
customer Ron civilian 2 1
```

### Volunteer Configuration

```
volunteer <name> <role> <cooldown/max_distance> [distance_per_step] [max_orders]
```

**Collector Types**:

```
volunteer Tamar collector 2              # coolDown=2, unlimited orders
volunteer Ron limited_collector 3 2      # coolDown=3, maxOrders=2
```

**Driver Types**:

```
volunteer Tal driver 7 4                 # maxDistance=7, speed=4, unlimited
volunteer Din limited_driver 3 2 3       # maxDistance=3, speed=2, maxOrders=3
```

## 🎮 Usage

Once running, the system accepts various commands:

- **Order Management**: Place and track orders
- **Customer Operations**: Add and manage customers
- **Volunteer Management**: Coordinate volunteer activities
- **System Operations**: Backup, restore, and status queries

## 🏛️ Architecture

### Design Patterns Used

- **Command Pattern**: Action system for operations
- **Factory Pattern**: Customer and volunteer creation
- **Observer Pattern**: Order status updates
- **Singleton**: Warehouse instance management

### Key Classes

- **WareHouse**: Central management system
- **Customer**: Abstract base for customer types
- **Volunteer**: Abstract base for volunteer types
- **Order**: Order lifecycle management
- **BaseAction**: Command interface for operations

## 🔧 Development

### Code Style

- C++11 standard compliance
- Wall and Weffc++ compiler flags for strict checking
- Proper memory management with RAII principles
- Clear separation of concerns

### VS Code Integration

The project includes VS Code configuration for:

- IntelliSense support
- Debugging configuration
- Build tasks
- Code formatting

## 📊 System Flow

1. **Initialization**: Load configuration and set up initial state
2. **Order Placement**: Customers place orders based on their limits
3. **Collection Phase**: Collector volunteers gather orders
4. **Delivery Phase**: Driver volunteers deliver to customers
5. **Status Updates**: Track progress and update order status
6. **Cleanup**: Handle volunteer availability and order completion

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## 📝 License

This project is part of a Systems Programming Laboratory (SPL) course assignment.

## 👥 Authors

- **hila302010** - Project maintainer

---

**Note**: This is an educational project designed to demonstrate object-oriented programming concepts, design patterns, and C++ development practices in the context of a warehouse management system.
