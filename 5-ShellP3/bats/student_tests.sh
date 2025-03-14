#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

setup() {
    rm -f file.txt
}

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}


@test "one command runs correctly" {
    run ./dsh <<EOF
echo "hello world"
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="helloworlddsh3>dsh3>cmdloopreturned0"

    echo "Stripped Output: $stripped_output"
    echo "Expected Output: $expected_output"

    # Assertions
    [ "$status" -eq 0 ]           
   [ "$stripped_output" = "$expected_output" ]   
}

@test "two piped commands run correctly" {
    run ./dsh <<EOF
ls | wc -l
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="7dsh3>dsh3>cmdloopreturned0"

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

    echo "Stripped Output: $stripped_output"
    echo "Expected Output: $expected_output"

    [ "$status" -eq 0 ]              
    [ "$stripped_output" = "$expected_output" ]  
}

@test "exit command works as intended" {
	run ./dsh <<EOF
	exit
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh3>dsh3>dsh3>cmdloopreturned0"

    echo "Stripped Output: $stripped_output"
    echo "Expected Output: $expected_output"

	[ "$status" -eq 0 ]
    [ "$stripped_output" = "$expected_output" ]  
}

@test "too many piped commands" {
    run ./dsh <<EOF
    ls -l | wc -l | grep ".c" | echo check | echo hello | echo save | echo maybe | echo great | echo you | pwd | cd | ls | echo work
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh3>error:pipinglimitedto8commandsdsh3>cmdloopreturned0"

    echo "Stripped Output: $stripped_output"
    echo "Expected Output: $expected_output"

	[ "$status" -eq 0 ]
    [ "$stripped_output" = "$expected_output" ]   
}

@test "program continues with correct error message if no commands given" {
    run ./dsh <<EOF

EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh3>warning:nocommandsprovideddsh3>cmdloopreturned0"

    echo "Stripped Output: $stripped_output"
    echo "Expected Output: $expected_output"

	[ "$status" -eq 0 ]
    [ "$stripped_output" = "$expected_output" ]     
}

@test "program works with extra spaces inbetween piped command" {
    run ./dsh <<EOF
    ls | wc               -l
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="7dsh3>dsh3>cmdloopreturned0"

    echo "Stripped Output: $stripped_output"
    echo "Expected Output: $expected_output"

	[ "$status" -eq 0 ]
    [ "$stripped_output" = "$expected_output" ]     

}

@test "program works with leading spaces in given piped command" {
    run ./dsh <<EOF
                   ls | wc -l
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="7dsh3>dsh3>cmdloopreturned0"

    echo "Stripped Output: $stripped_output"
    echo "Expected Output: $expected_output"

	[ "$status" -eq 0 ]
    [ "$stripped_output" = "$expected_output" ]    
}

@test "program works with trailing spaces in given piped command" {
    run ./dsh <<EOF
    ls | wc -l                        
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="7dsh3>dsh3>cmdloopreturned0"

    echo "Stripped Output: $stripped_output"
    echo "Expected Output: $expected_output"

	[ "$status" -eq 0 ]
    [ "$stripped_output" = "$expected_output" ]  
}

@test "program correct process piped commands with no output" {
    run ./dsh <<EOF
    ls | touch file.txt
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh3>dsh3>cmdloopreturned0"

    echo "Stripped Output: $stripped_output"
    echo "Expected Output: $expected_output"

	[ "$status" -eq 0 ]
    [ "$stripped_output" = "$expected_output" ]  
}

@test "empty ouput from a pipe works as intended" {
    run ./dsh <<EOF
    echo "" | wc -l
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="1dsh3>dsh3>cmdloopreturned0"

    echo "Stripped Output: $stripped_output"
    echo "Expected Output: $expected_output"

	[ "$status" -eq 0 ]
    [ "$stripped_output" = "$expected_output" ]  

}

@test "command with empty pipes outputs nothing" {
    run ./dsh <<EOF
     | | | |
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh3>dsh3>cmdloopreturned0"

    echo "Stripped Output: $stripped_output"
    echo "Expected Output: $expected_output"

	[ "$status" -eq 0 ]
    [ "$stripped_output" = "$expected_output" ]  
}