#!/bin/bash

# Comprehensive Warehouse Management System Test Suite
# This script thoroughly tests all functionality and edge cases

echo "=== Comprehensive Warehouse Management System Test Suite ==="
echo ""

# Ensure we're in the correct directory
cd "$(dirname "$0")"

# Check if binary exists
if [ ! -f "./bin/run" ]; then
    echo "ERROR: Binary ./bin/run not found. Please compile first with 'make'"
    exit 1
fi

# Check if config file exists
if [ ! -f "configFileExample.txt" ]; then
    echo "ERROR: Config file configFileExample.txt not found"
    exit 1
fi

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo "Starting comprehensive tests..."
echo ""

# Function to run a test
run_test() {
    local test_name="$1"
    local expected="$2"
    local commands="$3"
    
    echo -e "${BLUE}Test: $test_name${NC}"
    echo -e "${YELLOW}Expected: $expected${NC}"
    echo "Commands: $commands"
    echo ""
    echo "$commands" | ./bin/run configFileExample.txt
    echo ""
    echo "----------------------------------------"
    echo ""
}

# ===============================
# BASIC FUNCTIONALITY TESTS
# ===============================

echo -e "${GREEN}=== BASIC FUNCTIONALITY TESTS ===${NC}"

# Test 1: Customer existence and status
run_test "Customer 0 (Moshe) Status" \
    "Customer exists with ID=0, 2 orders left" \
    "customerStatus 0
close"

# Test 2: Customer 1 (Ron) Status  
run_test "Customer 1 (Ron) Status" \
    "Customer exists with ID=1, 1 order left" \
    "customerStatus 1
close"

# Test 3: Non-existent customer
run_test "Non-existent Customer Status" \
    "Error: Customer doesn't exist" \
    "customerStatus 99
close"

# Test 4: Volunteer status checks
run_test "Volunteer 0 (Tamar - Collector) Status" \
    "Shows collector volunteer status, not busy, unlimited orders" \
    "volunteerStatus 0
close"

run_test "Volunteer 1 (Ron - Limited Collector) Status" \
    "Shows limited collector, not busy, 2 orders max" \
    "volunteerStatus 1
close"

run_test "Volunteer 2 (Tal - Driver) Status" \
    "Shows driver volunteer, not busy, unlimited orders" \
    "volunteerStatus 2
close"

run_test "Volunteer 3 (Din - Limited Driver) Status" \
    "Shows limited driver, not busy, 3 orders max" \
    "volunteerStatus 3
close"

# ===============================
# ORDER CREATION TESTS  
# ===============================

echo -e "${GREEN}=== ORDER CREATION TESTS ===${NC}"

# Test 5: Valid order creation
run_test "Create Order for Customer 0" \
    "Order created successfully, shows as PENDING" \
    "order 0
orderStatus 0
close"

# Test 6: Invalid customer order
run_test "Order for Non-existent Customer" \
    "Error: Cannot place this order" \
    "order 99
close"

# Test 7: Multiple orders for same customer
run_test "Multiple Orders for Customer 0" \
    "Both orders created, customer has 0 orders left" \
    "order 0
order 0
customerStatus 0
close"

# Test 8: Exceed customer order limit
run_test "Exceed Customer Order Limit" \
    "Third order should fail" \
    "order 0
order 0
order 0
close"

# Test 9: Order for customer 1 (only 1 order allowed)
run_test "Customer 1 Single Order" \
    "Order created, then second order fails" \
    "order 1
order 1
close"

# ===============================
# SIMULATION STEP TESTS
# ===============================

echo -e "${GREEN}=== SIMULATION STEP TESTS ===${NC}"

# Test 10: Step command validation
run_test "Step Command Validation" \
    "Usage message when no number provided" \
    "step
close"

# Test 11: Single step simulation
run_test "Single Step Simulation" \
    "Order progresses from PENDING to COLLECTING" \
    "order 0
step 1
orderStatus 0
close"

# Test 12: Multi-step order progression
run_test "Complete Order Lifecycle" \
    "Order goes PENDING -> COLLECTING -> DELIVERING -> COMPLETED" \
    "order 0
step 1
orderStatus 0
step 1
orderStatus 0
step 1
orderStatus 0
step 1
orderStatus 0
close"

# Test 13: Multiple orders with steps
run_test "Multiple Orders Simulation" \
    "Multiple orders progress independently" \
    "order 0
order 1
step 2
orderStatus 0
orderStatus 1
step 3
orderStatus 0
orderStatus 1
close"

# ===============================
# VOLUNTEER MANAGEMENT TESTS
# ===============================

echo -e "${GREEN}=== VOLUNTEER MANAGEMENT TESTS ===${NC}"

# Test 14: Volunteer assignment to orders
run_test "Volunteer Assignment" \
    "Orders get assigned to appropriate volunteers" \
    "order 0
step 1
volunteerStatus 0
orderStatus 0
close"

# Test 15: Limited volunteer exhaustion
run_test "Limited Volunteer Exhaustion" \
    "Limited volunteers should be removed when maxOrders reached" \
    "order 0
order 1
step 1
volunteerStatus 1
step 5
volunteerStatus 1
close"

# Test 16: Driver assignment after collection
run_test "Driver Assignment After Collection" \
    "Order moves from collector to driver" \
    "order 0
step 3
volunteerStatus 2
orderStatus 0
close"

# ===============================
# COMMAND SYNTAX TESTS
# ===============================

echo -e "${GREEN}=== COMMAND SYNTAX TESTS ===${NC}"

# Test 17: Invalid commands
run_test "Invalid Command Handling" \
    "Proper error messages for invalid commands" \
    "invalidcommand
order
customerStatus
volunteerStatus
orderStatus
step
close"

# Test 18: Command with wrong parameters
run_test "Wrong Parameter Count" \
    "Usage messages for incomplete commands" \
    "order
customerStatus
volunteerStatus
orderStatus
step
customer John
close"

# ===============================
# ACTION LOG TESTS
# ===============================

echo -e "${GREEN}=== ACTION LOG TESTS ===${NC}"

# Test 19: Action logging
run_test "Action Log Functionality" \
    "All actions logged with completion status" \
    "order 0
customerStatus 0
orderStatus 0
log
close"

# Test 20: Log with failed actions
run_test "Log with Failed Actions" \
    "Failed actions also logged" \
    "order 99
customerStatus 99
log
close"

# ===============================
# BACKUP/RESTORE TESTS
# ===============================

echo -e "${GREEN}=== BACKUP/RESTORE TESTS ===${NC}"

# Test 21: Backup and restore
run_test "Backup and Restore Functionality" \
    "State preserved after backup/restore" \
    "order 0
backup
step 1
orderStatus 0
restore
orderStatus 0
close"

# ===============================
# DYNAMIC CUSTOMER ADDITION TESTS
# ===============================

echo -e "${GREEN}=== DYNAMIC CUSTOMER ADDITION TESTS ===${NC}"

# Test 22: Add new customer during runtime
run_test "Add New Customer" \
    "New customer added successfully" \
    "customer Alice soldier 5 3
customerStatus 2
order 2
close"

# Test 23: Invalid customer type
run_test "Invalid Customer Type" \
    "Error for invalid customer type" \
    "customer Bob invalid 2 1
close"

# ===============================
# EDGE CASE TESTS
# ===============================

echo -e "${GREEN}=== EDGE CASE TESTS ===${NC}"

# Test 24: Empty input handling
run_test "Empty Input Handling" \
    "Graceful handling of empty commands" \
    "


close"

# Test 25: Large step numbers
run_test "Large Step Numbers" \
    "Handle large step counts" \
    "order 0
step 100
orderStatus 0
close"

# Test 26: Order status for non-existent order
run_test "Non-existent Order Status" \
    "Error for non-existent order" \
    "orderStatus 99
close"

# Test 27: Volunteer status after deletion
run_test "Volunteer Status After Deletion" \
    "Check volunteer after they're removed" \
    "order 1
step 10
volunteerStatus 1
close"

# Test 28: Close command behavior
run_test "Close Command Output" \
    "Shows all pending/in-process/completed orders" \
    "order 0
order 1
step 2
close"

# ===============================
# STRESS TESTS
# ===============================

echo -e "${GREEN}=== STRESS TESTS ===${NC}"

# Test 29: Maximum orders test
run_test "Maximum Orders Stress Test" \
    "Create many orders and process them" \
    "order 0
order 0
order 1
customer TestCustomer soldier 1 5
order 2
order 2
step 10
log
close"

# Test 30: Complex simulation
run_test "Complex Workflow Simulation" \
    "Complex scenario with multiple customers, orders, and steps" \
    "customer Alice civilian 3 2
customer Bob soldier 4 1
order 0
order 1
order 2
order 3
step 1
volunteerStatus 0
volunteerStatus 1
step 2
orderStatus 0
orderStatus 1
orderStatus 2
orderStatus 3
step 5
customerStatus 0
customerStatus 1
customerStatus 2
customerStatus 3
log
close"

echo -e "${GREEN}=== ALL TESTS COMPLETED ===${NC}"
echo ""
echo "Summary of test categories:"
echo "• Basic Functionality (customer/volunteer status)"
echo "• Order Creation (valid/invalid scenarios)"  
echo "• Simulation Steps (order progression)"
echo "• Volunteer Management (assignment/deletion)"
echo "• Command Syntax (error handling)"
echo "• Action Logging (success/failure tracking)"
echo "• Backup/Restore (state preservation)"
echo "• Dynamic Customer Addition (runtime changes)"
echo "• Edge Cases (empty input, large numbers, etc.)"
echo "• Stress Tests (complex scenarios)"
echo ""
echo "Total tests: 30 comprehensive test scenarios"
