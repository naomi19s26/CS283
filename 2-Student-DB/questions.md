1. Yes, I believe giving get_student its own function improves the database application because it makes the code more reusable instead of repeating the same logic.

2. This function is poorly implemented because it returns a pointer to a variable that only exists inside the function. When the function ends, the variable disappears, causing a dangling pointer.

3. This works because it dynamically allocates memory, so the data stays in memory after the function returns. However, if the memory is not freed properly, it can cause memory leaks.

4. a. The ls command shows the file size based on the highest byte written. Since student records are stored based on their IDs, the file size increases as higher IDs are added.
   b. The disk usage stayed the same because the file was still within one 4KB block. When ID=64 was added, the file went over 4KB, so another block was needed, increasing disk usage.
   c. Adding a student with a very large ID made the file look huge in ls. But since most of that space wasn't actually written, the disk didn't store it, so du showed only a small increase.
