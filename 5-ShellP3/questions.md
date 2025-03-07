1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

I handled this implementation using waitpid() to ensure that all child processes complete before the shell continues. Without waitpid(), the program could start accepting new input before previous commands finish, which could cause unexpected behavior.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

It is important to close unused pipe ends after dup2() because it prevents deadlocks. Leaving them open could cause the system to hit the standard open file descriptor limit.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

cd is implemented as a built-in command because it involves changing the shell's directory. If it were executed as an external command, the directory change would be lost when the child process exits.

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

To support an arbitrary number of piped commands, I would dynamically allocate memory using malloc, allowing for reallocation if needed. Some trade-offs include increased complexity in memory management, potential inefficiency for small executions, and the need to handle allocation failures.