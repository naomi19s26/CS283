#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@setup() {
    ./dsh -s -p 1234
    server_pid=$!

    for i in {1..10}; do
        if nc -z localhost 1234; then
            break
        fi
        sleep 0.5
    done
    if ! nc -z localhost 1234; then
        echo "Server failed to start"
        exit 1
    fi
}

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "client runs correctly with server" {
    run ./dsh -c -p 1234 <<EOF
EOF
    [ "$status" -eq 0 ]
}

@test "single commands executes correctly" {
    run ./dsh -c -p 1234 <<EOF
echo "hi"
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    echo "Output: $stripped_output"
    [ "$status" -eq 1 ]

}




