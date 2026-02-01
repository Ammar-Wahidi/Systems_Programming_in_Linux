# Unix Shell Implementations

A collection of progressively complex Unix shell implementations in C, demonstrating fundamental operating system concepts and system programming techniques.

## 📚 Course Information

**Instructor:** Reda Maher  
**Reference:** *The Linux Programming Interface* by Michael Kerrisk

## 📋 Table of Contents

- [Overview](#overview)
- [Shell Implementations](#shell-implementations)
- [Compilation](#compilation)
- [Usage](#usage)
- [Features by Shell](#features-by-shell)
- [Utility Programs](#utility-programs)
- [Examples](#examples)
- [Technical Details](#technical-details)

## 🔍 Overview

This repository contains four shell implementations, each building upon the previous one with increasing functionality. Additionally, it includes custom implementations of common Unix utilities (`cp`, `mv`, `echo`, `pwd`).

## 🐚 Shell Implementations

### 1. FemtoShell (Basic)
The simplest shell implementation with minimal functionality.

**Features:**
- Basic command prompt
- `exit` command
- `echo` command
- Simple input parsing

### 2. PicoShell (Intermediate)
Adds built-in commands and external command execution.

**Features:**
- All FemtoShell features
- `pwd` - print working directory
- `cd` - change directory (with `~` support)
- External command execution via `fork()` and `execvp()`
- Proper error handling

### 3. NanoShell (Advanced)
Introduces variable management and substitution.

**Features:**
- All PicoShell features
- Local variable assignment (`x=value`)
- Variable substitution (`$variable`)
- `export` command for environment variables
- Enhanced `echo` with variable expansion
- Enhanced `cd` with variable expansion

### 4. MicroShell (Full-Featured)
The most advanced implementation with I/O redirection.

**Features:**
- All NanoShell features
- Input redirection (`<`)
- Output redirection (`>`)
- Error redirection (`2>`)
- Redirection with variable substitution
- Comprehensive error handling

## 🔨 Compilation

### Compile Individual Shells

```bash
# Compile FemtoShell
gcc -o femtoshell "Femto Shell/FemtoShell.c" -Wall -Wextra

# Compile PicoShell
gcc -o picoshell "Pico Shell/Pico_Shell.c" -Wall -Wextra

# Compile NanoShell
gcc -o nanoshell "Nano Shell/Nano_Shell.c" -Wall -Wextra

# Compile MicroShell
gcc -o microshell "Micro Shell/Micro_Shell.c" -Wall -Wextra
```

### Compile Utilities

```bash
# Navigate to Unix Utility folder or use full path
gcc -o cp "Unix Utility/cp.c" -Wall -Wextra
gcc -o mv "Unix Utility/mv.c" -Wall -Wextra
gcc -o echo "Unix Utility/echo.c" -Wall -Wextra
gcc -o pwd "Unix Utility/pwd.c" -Wall -Wextra
```

### Compile with Debugging Symbols

```bash
gcc -g -o microshell "Micro Shell/Micro_Shell.c" -Wall -Wextra
```

### Compile All at Once (Windows)

```bash
gcc -o femtoshell "Femto Shell/FemtoShell.c" && gcc -o picoshell "Pico Shell/Pico_Shell.c" && gcc -o nanoshell "Nano Shell/Nano_Shell.c" && gcc -o microshell "Micro Shell/Micro_Shell.c" && gcc -o cp "Unix Utility/cp.c" && gcc -o mv "Unix Utility/mv.c" && gcc -o echo "Unix Utility/echo.c" && gcc -o pwd "Unix Utility/pwd.c"
```

### Using Makefile 

Create a `Makefile` in the root directory:

```makefile
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: femtoshell picoshell nanoshell microshell cp mv echo pwd

femtoshell:
	$(CC) $(CFLAGS) -o femtoshell "Femto Shell/FemtoShell.c"

picoshell:
	$(CC) $(CFLAGS) -o picoshell "Pico Shell/Pico_Shell.c"

nanoshell:
	$(CC) $(CFLAGS) -o nanoshell "Nano Shell/Nano_Shell.c"

microshell:
	$(CC) $(CFLAGS) -o microshell "Micro Shell/Micro_Shell.c"

cp:
	$(CC) $(CFLAGS) -o cp "Unix Utility/cp.c"

mv:
	$(CC) $(CFLAGS) -o mv "Unix Utility/mv.c"

echo:
	$(CC) $(CFLAGS) -o echo "Unix Utility/echo.c"

pwd:
	$(CC) $(CFLAGS) -o pwd "Unix Utility/pwd.c"

clean:
	rm -f femtoshell picoshell nanoshell microshell cp mv echo pwd

.PHONY: all clean
```

Then simply run:
```bash
make
```

## 🚀 Usage

### Running a Shell

```bash
./femtoshell    # Run FemtoShell
./picoshell     # Run PicoShell
./nanoshell     # Run NanoShell
./microshell    # Run MicroShell
```

### Running Utilities

```bash
./cp source.txt destination.txt
./mv old_name.txt new_name.txt
./echo Hello World
./pwd
```

## ✨ Features by Shell

| Feature | FemtoShell | PicoShell | NanoShell | MicroShell |
|---------|------------|-----------|-----------|------------|
| `exit` | ✅ | ✅ | ✅ | ✅ |
| `echo` | ✅ | ✅ | ✅ | ✅ |
| `pwd` | ❌ | ✅ | ✅ | ✅ |
| `cd` | ❌ | ✅ | ✅ | ✅ |
| External commands | ❌ | ✅ | ✅ | ✅ |
| Variables | ❌ | ❌ | ✅ | ✅ |
| Variable substitution | ❌ | ❌ | ✅ | ✅ |
| `export` | ❌ | ❌ | ✅ | ✅ |
| I/O redirection | ❌ | ❌ | ❌ | ✅ |

## 🛠️ Utility Programs

### cp - Copy Files
```c
// Copies source file to destination
Usage: ./cp source destination
```

### mv - Move/Rename Files
```c
// Moves/renames source file to destination
Usage: ./mv source destination
```

### echo - Print Arguments
```c
// Prints arguments separated by spaces
Usage: ./echo [arguments...]
```

### pwd - Print Working Directory
```c
// Prints current working directory
Usage: ./pwd
```

## 📖 Examples

### FemtoShell Example

```
$ ./femtoshell
Femto shell prompt > echo Hello World

Hello World
Femto shell prompt > exit

Good Bye
```

### PicoShell Example

```
$ ./picoshell
Pico shell prompt > pwd
/home/user/projects
Pico shell prompt > cd /tmp
Pico shell prompt > pwd
/tmp
Pico shell prompt > ls -la
total 16
drwxrwxrwt  8 root root 4096 Feb  1 10:30 .
drwxr-xr-x 20 root root 4096 Jan 15 08:22 ..
Pico shell prompt > exit
Good Bye
```

### NanoShell Example

```
$ ./nanoshell
Nano Shell Prompt > name=John
Nano Shell Prompt > age=25
Nano Shell Prompt > echo Hello $name, you are $age years old
Hello John, you are 25 years old
Nano Shell Prompt > export PATH=/usr/local/bin:$PATH
Nano Shell Prompt > export name
Nano Shell Prompt > exit
Good Bye
```

### MicroShell Example

```
$ ./microshell
Nano Shell Prompt > echo Hello World > output.txt
Nano Shell Prompt > cat < output.txt
Hello World
Nano Shell Prompt > ls nonexistent 2> error.log
Nano Shell Prompt > cat error.log
ls: cannot access 'nonexistent': No such file or directory
Nano Shell Prompt > myvar=data.txt
Nano Shell Prompt > cat < $myvar > result.txt
Nano Shell Prompt > exit
Good Bye
```

## 🔧 Technical Details

### System Calls Used

- **Process Management:** `fork()`, `execvp()`, `wait()`, `waitpid()`
- **File Operations:** `open()`, `read()`, `write()`, `close()`, `unlink()`
- **I/O Redirection:** `dup()`, `dup2()`
- **Directory Operations:** `getcwd()`, `chdir()`
- **Environment:** `getenv()`, `setenv()`

### Memory Management

- Dynamic allocation with `malloc()`, `realloc()`
- Proper cleanup with `free()`
- String duplication with `strdup()`

### Key Concepts Demonstrated

1. **Process Creation and Management**
   - Forking child processes
   - Executing external programs
   - Process synchronization

2. **File Descriptors and I/O**
   - Standard I/O redirection
   - File descriptor manipulation
   - Buffered and unbuffered I/O

3. **String Processing**
   - Tokenization with `strtok()`
   - Variable substitution
   - Command parsing

4. **Environment Variables**
   - Local shell variables
   - Exported environment variables
   - Variable expansion

5. **Error Handling**
   - System call error checking
   - User-friendly error messages
   - Exit status management

## 📝 Project Structure

```
.
├── .gitignore
├── README.md
│
├── Femto Shell/
│   └── FemtoShell.c           # Basic shell implementation
│
├── Pico Shell/
│   └── Pico_Shell.c           # Intermediate shell with built-ins
│
├── Nano Shell/
│   └── Nano_Shell.c           # Advanced shell with variables
│
├── Micro Shell/
│   └── Micro_Shell.c          # Full-featured shell with I/O redirection
│
└── Unix Utility/
    ├── cp.c                   # Copy utility
    ├── mv.c                   # Move utility
    ├── echo.c                 # Echo utility
    └── pwd.c                  # Print working directory utility
```

## 🎯 Learning Objectives

This project demonstrates:
- Unix system programming fundamentals
- Process management in C
- File I/O and redirection
- Shell command parsing and execution
- Memory management in C
- Error handling best practices

## 📚 References

- **The Linux Programming Interface** by Michael Kerrisk
- Linux man pages (`man 2 fork`, `man 3 exec`, etc.)

---

**Note:** These shells are educational implementations and should not be used as production shells. They are designed to demonstrate core concepts of shell programming and Unix system calls.
