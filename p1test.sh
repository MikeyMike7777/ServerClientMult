#!/bin/bash

#--------------------------------------------------
# Author: Michael Mathews
# Assignment: Program 1
# Class: CSI 4337
# File: p1test.sh
# Description: This script tests mult functionality
#--------------------------------------------------

PASSED=0
FAILED=0
VAL=valgrind --tool=memcheck --show-reachable=yes --leak-check=full --track-fds=yes

# Function to perform a test
# Arguments: $1 = Command to run
#            $2 = Expected output
test_command() {
  local COMMAND="$1"
  local EXPECTED="$2"

  echo -n "Running test: '$COMMAND' ... "

  # Run the command and capture its output
  local RESULTS=$($COMMAND 2>&1)

  # Check the exit status of the command
  if [ $? -ne 0 ]; then
    echo "FAILED (command error)"
    ((FAILED++))
    return
  fi

  # Check the command output
  if [[ "$RESULTS" == *"$EXPECTED"* ]]; then
    echo "PASS"
    ((PASSED++))
  else
    echo "FAILED (Expected: $EXPECTED, Got: $RESULTS)"
    ((FAILED++))
  fi
}

speed_test() {
  local COMMAND="$1"
  local EXPECTED="$2"
  echo -n "Running speed test: '$COMMAND' ... "
  local START=$(date +%s)
  local RESULTS=$($COMMAND 2>&1)
  local END=$(date +%s)
  local DIFF=$(( END - START ))
  if [[ "$RESULTS" == *"$EXPECTED"* ]]; then
    echo "PASS (Time: $DIFF seconds)"
  else
    echo "FAILED (Expected: $EXPECTED, Got: $RESULTS, Time: $DIFF seconds)"
    ((FAILED++))
  fi
}

short_circuit_test() {
  local COMMAND="$1"
  echo -n "Running short-circuit test: '$COMMAND' ... "
  local RESULTS=$($COMMAND 2>&1)
  if [[ "$RESULTS" == *"Warning"* ]]; then
    echo "FAILED (Got: $RESULTS)"
    ((FAILED++))
  else
    echo "PASS"
  fi
}

# Run basic tests
test_command "$VAL ./mult 0 1" "Answer: 1"
test_command "$VAL ./mult -1 1" "Error: Invalid N or P value"
test_command "$VAL ./mult 1 -1" "Error: Invalid N or P value"
test_command "$VAL ./mult abc 1" "Error: Invalid N or P value"
test_command "$VAL ./mult 1 abc" "Error: Invalid N or P value"
test_command "$VAL ./mult" "Usage: ./mult <N> <P>"
test_command "$VAL ./mult 1 2 3" "Usage: ./mult <N> <P>"
test_command "$VAL ./mult 20 1" "Answer: 2432902008176640000"
test_command "$VAL ./mult 20 2" "Answer: 2432902008176640000"
test_command "$VAL ./mult 0 2" "Answer: 1"

# Run speed test should 789 if f.c is replaced
speed_test "$VAL ./mult 3 1" "Answer: 6"
speed_test "$VAL ./mult 3 2" "Answer: 6"

# Run short circuit test
short_circuit_test "$VAL ./mult 10 1"

# Compile and run fake fork test
gcc mult.c f.c fake_fork.c -o mult
test_command "$VAL ./mult 3 1" "Error"

# Restore original executable
gcc mult.c f.c -o mult

echo "Tests Passed: $PASSED"
echo "Tests Failed: $FAILED"

# Set exit code based on test results
if [ $FAILED -ne 0 ]; then
  exit 1
fi
