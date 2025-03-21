#!/usr/bin/env bats

# File: student_tests.sh
# Extended tests for remote shell project without teardown

setup() {
  # Start the server in the background on port 7990.
  rm -f server_output.log
  ./dsh -s -p 7990 &
  server_pid=$!
  sleep 1  # Give the server some time to start.
}

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Pipeline execution produces expected output in client mode" {
  run ./dsh -c -p 7990 <<EOF
ls | grep ".c"
stop-server
EOF
  stripped_output=$(echo "$output" | tr -d '[:space:]' | sed 's/[^a-zA-Z0-9]//g')
  expected_output="socketclientmodeaddr1270017990dsh4dshclicdshlibcrshclicrshservercdsh4clientrequestedservertostopstoppingcmdloopreturned0"
  echo "Output :$stripped_output" 
  echo "Output2: $expected_output"

  [ "$status" -eq 0 ]
  [ "$stripped_output" = "$expected_output" ]
}



@test "no commands given produces expected output in client mode" {
    run ./dsh -c -p 7990 <<EOF

stop-server
EOF
  stripped_output=$(echo "$output" | tr -d '[:space:]' | sed 's/[^a-zA-Z0-9]//g')
  expected_output="socketclientmodeaddr1270017990dsh4warningnocommandsprovideddsh4clientrequestedservertostopstoppingcmdloopreturned0"
  echo "Output :$stripped_output" 
  echo "Output2: $expected_output"

  [ "$status" -eq 0 ]
  [ "$stripped_output" = "$expected_output" ]  

}


@test "stop-server command closes the client" {
    run ./dsh -c -p 7990 <<EOF
stop-server
EOF
  stripped_output=$(echo "$output"  | tr -d '[:space:]' | sed 's/[^a-zA-Z0-9]//g')
  expected_output="socketclientmodeaddr1270017990dsh4clientrequestedservertostopstoppingcmdloopreturned0"
  echo "Output1:$stripped_output" 
  echo "Output2: $expected_output"

  [ "$status" -eq 0 ]
  [ "$stripped_output" = "$expected_output" ]
}


@test "cd command works as intended without second command" {
        run ./dsh -c -p 7990 <<EOF
cd
EOF
  stripped_output=$(pwd | tr -d '[:space:]' | sed 's/[^a-zA-Z0-9]//g')
  expected_output="homene347CS2836RShell"
  echo "Output1:$stripped_output" 
  echo "Output2: $expected_output"
  [ "$stripped_output" = "$expected_output" ]
  [ "$status" -eq 0 ]
}


@test "commands with quotations work as expected" {
  run ./dsh -c -p 7990 <<EOF
  echo "hi    yes"
stop-server
EOF
  stripped_output=$(echo "$output" | tr -d '[:space:]' | sed 's/[^a-zA-Z0-9]//g')
  expected_output="socketclientmodeaddr1270017990dsh4hiyesdsh4clientrequestedservertostopstoppingcmdloopreturned0"
  echo "Output :$stripped_output" 
  echo "Output2: $expected_output"

  [ "$status" -eq 0 ]
  [ "$stripped_output" = "$expected_output" ]
}

@test "invalid commands give expected output" {
    run ./dsh -c -p 7990 <<EOF
hi
stop-server
EOF
  stripped_output=$(echo "$output" | tr -d '[:space:]' | sed 's/[^a-zA-Z0-9]//g')
  echo "Output :$stripped_output" 

  [ "$status" -eq 0 ]

}

@test "too many piped commands" {
    run ./dsh -c -p 7990 <<EOF
    ls -l | wc -l | grep ".c" | echo check | echo hello | echo save | echo maybe | echo great | echo you | pwd | cd | ls | echo work
    stop-server
EOF
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh4>error:pipinglimitedto"

    echo "Stripped Output: $stripped_output"
    echo "Expected Output: $expected_output"

    [ "$status" -eq 0 ]
    [[ "$stripped_output" == *"$expected_output"* ]]  
}

@test "Single command execution works" {
    run ./dsh -c -p 7990 <<EOF
echo hi
stop-server
EOF
  stripped_output=$(echo "$output" | tr -d '[:space:]' | sed 's/[^a-zA-Z0-9]//g')
  expected_output="socketclientmodeaddr1270017990dsh4hidsh4clientrequestedservertostopstoppingcmdloopreturned0"
  echo "Output :$stripped_output" 
  echo "Output2: $expected_output"

  [ "$status" -eq 0 ]
  [ "$stripped_output" = "$expected_output" ]

}



