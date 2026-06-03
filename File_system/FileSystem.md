

## 1. Introduction

In Linux, every file and directory contains not only data but also metadata.
This metadata is stored in a data structure called an **inode**.

Metadata includes information such as:

* File type
* File size
* Permissions
* Owner information
* Important timestamps

The `stat()` family of system calls is the proper way for a C program to access this information.
Instead of parsing shell command output such as `ls -l`, a program can directly read file metadata using system calls like `stat()` and `lstat()`. 
[In this exercise, we build a small command-line tool named `filestat`, similar to the Linux `stat` command.](https://man7.org/linux/man-pages/man0/sys_stat.h.0p.html)

---

## 2. Requirements

The program must:

1. Receive exactly one command-line argument, which is the path to a file or directory.
2. Print a usage message if the argument is missing or invalid.
3. Use `lstat()` to read metadata of the given path.
4. Display the following information:

* File Path
* File Type
* Size in bytes
* Last Modified time

The program must identify at least these file types:

* Regular File
* Directory
* Symbolic Link

---

## 3. Source Code: `filestat.c`

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[])
{
    struct stat sb;
    char time_buf[100];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (lstat(argv[1], &sb) == -1) {
        perror("lstat");
        exit(EXIT_FAILURE);
    }

    printf("File Path: %s\n", argv[1]);

    printf("File Type: ");

    if (S_ISREG(sb.st_mode)) {
        printf("Regular File\n");
    } else if (S_ISDIR(sb.st_mode)) {
        printf("Directory\n");
    } else if (S_ISLNK(sb.st_mode)) {
        printf("Symbolic Link\n");
    } else {
        printf("Other\n");
    }

    printf("Size: %ld bytes\n", (long)sb.st_size);

    struct tm *tm_info = localtime(&sb.st_mtime);

    if (tm_info == NULL) {
        perror("localtime");
        exit(EXIT_FAILURE);
    }

    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);

    printf("Last Modified: %s\n", time_buf);

    return EXIT_SUCCESS;
}
```

---

## 4. Explanation

### Header Files

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
```

These headers are used for:

* Standard input/output
* Program exit status
* `struct stat`
* `lstat()`
* Time formatting functions

---

### Checking Command-Line Arguments

```c
if (argc != 2) {
    fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
    exit(EXIT_FAILURE);
}
```

The program expects exactly one argument.

Example:

```bash
./filestat test.txt
```

If the user does not provide a file path, the program prints:

```bash
Usage: ./filestat <file_path>
```

---

### Using `lstat()`

```c
if (lstat(argv[1], &sb) == -1) {
    perror("lstat");
    exit(EXIT_FAILURE);
}
```

The `lstat()` system call reads metadata of the file or directory and stores it in the `struct stat` variable `sb`.

Unlike `stat()`, `lstat()` does not follow symbolic links.
If the path is a symbolic link, `lstat()` returns information about the link itself.

---

### Detecting File Type

```c
if (S_ISREG(sb.st_mode)) {
    printf("Regular File\n");
} else if (S_ISDIR(sb.st_mode)) {
    printf("Directory\n");
} else if (S_ISLNK(sb.st_mode)) {
    printf("Symbolic Link\n");
} else {
    printf("Other\n");
}
```

The file type is stored inside the `st_mode` field.

The program uses these macros:

| Macro       | Meaning       |
| ----------- | ------------- |
| `S_ISREG()` | Regular file  |
| `S_ISDIR()` | Directory     |
| `S_ISLNK()` | Symbolic link |

---

### Printing File Size

```c
printf("Size: %ld bytes\n", (long)sb.st_size);
```

The file size is stored in the `st_size` field of `struct stat`.

The size is printed in bytes.

---

### Printing Last Modified Time

```c
struct tm *tm_info = localtime(&sb.st_mtime);
strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);
```

The last modification time is stored in:

```c
sb.st_mtime
```

This value is a `time_t` timestamp.
The program converts it into a human-readable format using `localtime()` and `strftime()`.

Example output format:

```text
2026-06-01 18:30:15
```

---

## 5. Compilation

Compile the program using `gcc`:

```bash
gcc filestat.c -o filestat
```

---

## 6. Running the Program

Example with a regular file:

```bash
./filestat test.txt
```

Example with a directory:

```bash
./filestat /home/user
```

Example with a symbolic link:

```bash
ln -s test.txt link.txt
./filestat link.txt
```

---

## 7. Sample Output

```text
File Path: test.txt
File Type: Regular File
Size: 128 bytes
Last Modified: 2026-06-01 18:30:15
```

---

## 8. Conclusion

The `filestat` program demonstrates how to use the `lstat()` system call to access file metadata in Linux.

It can display important information such as:

* File path
* File type
* File size
* Last modification time

This approach is more reliable than parsing shell command output because the program directly accesses metadata from the operating system. 

