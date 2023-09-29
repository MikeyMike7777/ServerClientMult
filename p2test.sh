#!/bin/bash

#--------------------------------------------------
# Author: Michael Mathews
# Assignment: Program 2
# Class: CSI 4337
# File: p2test.sh
# Description: This script tests mult thread functionality
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
test_command "$VAL ./mult-thread 0 1" "Answer: 1"
test_command "$VAL ./mult-thread -1 1" "Error: Invalid N or T value"
test_command "$VAL ./mult-thread 1 -1" "Error: Invalid N or T value"
test_command "$VAL ./mult-thread abc 1" "Error: Invalid N or T value"
test_command "$VAL ./mult-thread 1 abc" "Error: Invalid N or T value"
test_command "$VAL ./mult-thread" "Usage: ./mult <N> <T>"
test_command "$VAL ./mult-thread 1 2 3" "Usage: ./mult <N> <T>"
test_command "$VAL ./mult-thread 20 1" "Answer: 2432902008176640000"
test_command "$VAL ./mult-thread 20 2" "Answer: 2432902008176640000"
test_command "$VAL ./mult-thread 0 2" "Answer: 1"

# Run speed test should 789 if f.c is replaced
speed_test "$VAL ./mult-thread 3 1" "Answer: 6"
speed_test "$VAL ./mult-thread 3 2" "Answer: 6"

# Run short circuit test
short_circuit_test "$VAL ./mult-thread 10 1"

# Run basic tests on Java program
test_command "java Mult 0 1" "Answer: 1"
test_command "java Mult -1 1" "Error: Invalid N or T value"
test_command "java Mult 1 -1" "Error: Invalid N or T value"
test_command "java Mult abc 1" "Error: Invalid N or T value"
test_command "java Mult 1 abc" "Error: Invalid N or T value"
test_command "java Mult" "Usage: java Mult <N> <T>"
test_command "java Mult 1 2 3" "Usage: java Mult <N> <T>"
test_command "java Mult 20 1" "Answer: 2432902008176640000"
test_command "java Mult 20 2" "Answer: 2432902008176640000"
test_command "java Mult 0 2" "Answer: 1"

# Print test summary
echo "Tests Passed: $PASSED"
echo "Tests Failed: $FAILED"

# Set exit code based on test results
if [ $FAILED -ne 0 ]; then
  exit 1
fi

