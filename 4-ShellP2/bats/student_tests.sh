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


@test "cd command doesn't change directory when no second argument is given" {
	current=$(pwd);
	run ./dsh <<EOF
	cd

EOF
	output=$(pwd);
	[ "$status" -eq 0 ]
	[ "$output" = "$current" ]

}



@test "program continues running even though empty command is given" {
    run ./dsh <<EOF

EOF
	stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')
	echo "$stripped_output"
    [ "${stripped_output}" = "dsh2> warning: no commands provideddsh2> cmd loop returned 0" ]
}

@test "cd moves to new directory" {
    mkdir -p /tmp/test_dir
    run ./dsh <<EOF
    cd /tmp/test_dir
    pwd
EOF

    [ "$status" -eq 0 ]
    [ "${lines[0]}" == "/tmp/test_dir" ]

    rm -r /tmp/test_dir
}

@test "program doesn't perform anything if invalid executable was given" {
    run ./dsh <<EOF
    invalidcommand
EOF

    echo "$output"    
    [ "$status" -eq 0 ]
    [[ "${lines[-1]}" == "cmd loop returned 0" ]]
}

@test "external commands work" {
   	run ./dsh <<EOF
   	echo "hello"
EOF
	[ "$status" -eq 0 ]
    [[ "${lines[-1]}" == "cmd loop returned 0" ]]
}


@test "exit command works as intended" {
	run ./dsh <<EOF
	exit
EOF
	[ "$status" -eq 0 ]
}


@test "shell works as intended with an argument with quotations containing only empty spaces" {
    run ./dsh <<EOF
	echo " "
EOF
	stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')
    echo "$stripped_output"
    [ "$stripped_output" == "dsh2> dsh2> cmd loop returned 0dsh2> dsh2> cmd loop returned 0" ]
    [ "$status" -eq 0 ]
}
