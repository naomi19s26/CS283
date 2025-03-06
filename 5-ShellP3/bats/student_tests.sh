#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "command with extra arguments work" {
    run ./dsh <<EOF
ls -l
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="total43drwx--x--x2ne347domainusers2Mar521:34bats-rwx--x--x1ne347domainusers27904Mar521:32dsh-rw-------1ne347domainusers258Mar516:31dsh_cli.c-rw-------1ne347domainusers9883Mar521:32dshlib.c-rw-------1ne347domainusers2268Mar516:07dshlib.h-rw-------1ne347domainusers619Mar516:07makefile-rw-------1ne347domainusers1068Mar516:09questions.mddsh3>dsh3>cmdloopreturned0"
    echo "Stripped Output: $stripped_output"

    [ "$status" -eq 0 ]                          
    [ "$stripped_output" = "$expected_output" ]                   
}

@test "one command runs correctly" {
    run ./dsh <<EOF
echo "hello world"
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="helloworlddsh3>dsh3>cmdloopreturned0"

    # Debugging output (only printed if the test fails)
    echo "Captured stdout:"
    echo "$output"
    echo "Exit Status: $status"
    echo "Stripped Output: $stripped_output"

    # Assertions
    [ "$status" -eq 0 ]                          # Check that the exit status is 0 (success)
   [ "$stripped_output" = "$expected_output" ]            # Check that the output matches exactly
}

@test "two piped commands run correctly" {
    run ./dsh <<EOF
ls -l | wc -l
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="8dsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:"
    echo "$output"
    echo "Exit Status: $status"
    echo "Stripped Output: $stripped_output"
    echo "Expected Output: $expected_output"

    # Assertions
    [ "$status" -eq 0 ]            
    [ "$stripped_output" = "$expected_output" ]  
}

@test "multiple piped commands work correctly" {
    run ./dsh <<EOF
ls -l | grep .c | wc -l
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="2dsh3>dsh3>cmdloopreturned0"

    echo "Exit Status: $status"
    echo "Stripped Output: $stripped_output"
    echo "Expected Output: $expected_output"

    [ "$status" -eq 0 ]              
    [ "$stripped_output" = "$expected_output" ]  
}