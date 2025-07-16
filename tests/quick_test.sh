#!/bin/bash

# Quick Validation Test - Essential functionality check
# Run this to quickly verify the system is working correctly

echo "=== QUICK VALIDATION TEST ==="
echo ""

cd "$(dirname "$0")"

if [ ! -f "./bin/run" ]; then
    echo "ERROR: Binary not found. Run 'make' first."
    exit 1
fi

echo "Testing core functionality..."

# Test 1: Basic customer validation
echo "1. Customer validation..."
echo -e "customerStatus 0\ncustomerStatus 1\ncustomerStatus 99\nclose" | ./bin/run configFileExample.txt | grep -E "(CustomerID|Error)" | head -3

# Test 2: Order creation  
echo "2. Order creation..."
echo -e "order 0\norderStatus 0\norder 99\nclose" | ./bin/run configFileExample.txt | grep -E "(OrderID|Error)" | head -2

# Test 3: Simulation progression
echo "3. Order progression..."
echo -e "order 0\nstep 3\norderStatus 0\nclose" | ./bin/run configFileExample.txt | grep -E "OrderStatus" | head -1

# Test 4: Volunteer management
echo "4. Volunteer status..."
echo -e "volunteerStatus 0\nclose" | ./bin/run configFileExample.txt | grep -E "VolunteerID" | head -1

# Test 5: Action logging
echo "5. Action logging..."
echo -e "order 0\nlog\nclose" | ./bin/run configFileExample.txt | grep -E "order 0" | head -1

echo ""
echo "Quick validation complete. If all tests show expected output, system is working correctly."
