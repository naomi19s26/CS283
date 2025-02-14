1. `fgets()` is a good choice because it reads the entire user input, including white spaces, up to `ARG_MAX` length, and automatically null-terminates the string.  

2. `malloc()` is better because it prevents reallocation in every loop iteration, unlike a fixed-size array, which would require reallocation if the input size exceeds the predefined limit.  

3. If spaces are not trimmed, the shell could misinterpret the executable and arguments, causing errors or unexpected behavior when running commands.  

4. a. Redirection allows users to control where input comes from or where output is sent. Some examples that we should implement in our shell:  
   - We should be able to redirect the output of a command to a file, e.g., `ls > file.txt` in bash.  
   
   -  We should also be able to direct input from a file to a command, e.g., `wc -l < file.txt` in bash.  

   -  We should also be able to combine redirection commands, e.g., `wc -l < file.txt > output.txt` in bash.  

4. b.  The main difference is that redirection is mostly for redirecting input and output between files and commands, while piping is just a chain of commands (these commands might not be linked in any way). In redirection, there must be a transfer of data.  

4. c. Keeping STDERR separate from STDOUT helps distinguish between regular output and error messages. It also makes debugging easier since programs can monitor error codes and outputs to check if they match expected results.  

4. d. An error code should be returned when a command fails, e.g., `0` for success and a negative value for an error. Yes, it would be better to find a way to merge them in the output using 2>&1 as done in bash. We could also return an output message based on the error code returned.  

