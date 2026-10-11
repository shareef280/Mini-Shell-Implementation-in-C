# Linux Mini Shell in C

A lightweight command-line shell developed in C to explore Linux system programming, process management, command execution, inter-process communication, and signal handling.

## Overview

The **Linux Mini Shell** is a command-line interpreter that accepts user input, identifies commands, and executes built-in or external programs. The project demonstrates fundamental operating system concepts through Linux system calls and C programming.

It provides a simplified shell environment with support for command pipelines, signal handling, and basic foreground and background job management.

## Features

* **Built-in Commands:** Supports commands such as `cd`, `pwd`, `echo`, `exit`, and `clear`.
* **External Command Execution:** Executes external programs using process creation and program execution system calls.
* **Process Management:** Uses `fork()` and `execvp()` to create processes and execute programs.
* **Pipeline Support:** Connects commands using pipes for inter-process communication.
* **Signal Handling:** Handles signals such as `SIGINT` and `SIGTSTP`.
* **Basic Job Control:** Includes basic job tracking and foreground/background job-related commands.
* **Custom Shell Prompt:** Provides a shell prompt and basic prompt customization.
* **Build Automation:** Uses a Makefile to simplify compilation and cleanup.

## Technologies Used

| Technology         | Purpose                        |
| ------------------ | ------------------------------ |
| C Programming      | Core application development   |
| Linux              | Execution environment          |
| GCC                | Compilation                    |
| Makefile           | Build automation               |
| Linux System Calls | Process creation and execution |
| Pipes              | Inter-process communication    |
| Signals            | Process and interrupt handling |

## Project Structure

```text
linux-mini-shell/
├── main.c
├── main_h.h
├── functions.c
├── scan.c
├── external_commd.txt
├── Makefile
├── README.md
└── .gitignore
```

### File Description

* `main.c` — Program entry point and shell initialization.
* `main_h.h` — Header declarations, macros, and shared definitions.
* `functions.c` — Implements shell commands, process execution, pipes, signal handling, and basic job tracking.
* `scan.c` — Handles user input and command identification.
* `external_commd.txt` — Contains the list of recognized external command names.
* `Makefile` — Defines compilation and cleanup rules.
* `README.md` — Project documentation.
* `.gitignore` — Excludes generated files from version control.

## Prerequisites

Before building the project, ensure that you have:

* A Linux environment or compatible Linux development environment.
* GCC compiler.
* GNU Make.
* Basic knowledge of C programming and Linux commands.

## Installation and Execution

### 1. Clone the Repository

```bash
git clone https://github.com/YOUR_USERNAME/linux-mini-shell.git
```

Replace `YOUR_USERNAME` with your GitHub username.

### 2. Navigate to the Project Directory

```bash
cd linux-mini-shell
```

### 3. Compile the Project

```bash
make
```

### 4. Run the Shell

```bash
./minishell
```

### 5. Clean Generated Files

```bash
make fclean
```

## Usage Examples

After launching the shell, try the following commands:

### Execute Basic Commands

```bash
pwd
ls
echo Hello World
```

### Change the Current Directory

```bash
cd ..
pwd
```

### Execute Commands Through a Pipeline

```bash
cat main.c | grep include
```

### Use Shell Commands

```bash
clear
jobs
exit
```

**Note:** Command availability depends on the implementation and the external command list configured in the project.

## Key Concepts Demonstrated

### 1. Process Creation

The `fork()` system call creates a child process, allowing the shell to execute commands independently of its main process.

### 2. Program Execution

The `execvp()` function replaces the current process image with the requested program when command execution succeeds.

### 3. Inter-Process Communication

The `pipe()` system call provides a communication channel between processes. The `dup2()` system call can redirect file descriptors to connect the output of one command to the input of another.

### 4. Signal Handling

Signal handling allows the shell to respond to events such as keyboard interrupts and stop requests.

### 5. Job Management

Basic job tracking provides a foundation for understanding process states and foreground/background execution.

## Learning Outcomes

Through this project, I explored:

* Linux process creation and program execution.
* System calls and their role in operating systems.
* Inter-process communication using pipes.
* Signal handling and process control.
* Command parsing and execution.
* Compilation and build automation using Makefiles.
* Practical application of operating system concepts in C.

## Current Limitations

This project is an educational mini shell and is not intended to be a complete POSIX-compliant shell.

Depending on the command and input, some advanced shell features may be unsupported, including:

* Advanced quoting and escape handling.
* Input and output redirection.
* Environment-variable expansion.
* Complete POSIX job control.
* Comprehensive command parsing and error recovery.

## Future Enhancements

Potential improvements include:

* Support for input and output redirection.
* Improved command parsing and quoted arguments.
* Enhanced foreground and background process management.
* Better error handling and memory management.
* Support for additional shell features.

## Author

Shareef Sab

Electronics and Communication Engineering Student

Interested in C Programming, Linux System Programming, Embedded Systems, and Operating Systems.



*Developed as a learning project to understand Linux system programming and operating system fundamentals.*
