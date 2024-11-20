
# desh - A Simple Unix Shell  

## Overview  
This project is a simple Unix shell written in C, named `desh`. It supports basic shell functionality, built-in commands, and the ability to execute external programs.  

## Features  
1. Customizable prompt that displays the current working directory.  
2. Execution of built-in commands such as `exit`, `which`, `list`, `cd`, `pwd`, `prompt`, `printenv`, and `setenv`.  
3. Wildcard support for `*` and `?` in file and directory listings.  
4. Ability to handle external commands via `$PATH` or absolute/relative paths.  
5. Signal handling for `Ctrl-C`, `Ctrl-Z`, and `Ctrl-D`.  
6. Environment variable manipulation (`printenv`, `setenv`).  

---

## Getting Started  

### Cloning the Repository  
```bash
git clone git@gitlab.eecis.udel.edu:YOUR_USERNAME/cisc361-project-2.git
cd cisc361-project-2
```

### Building the Shell  
To compile the shell, run the following:  
```bash
make
```

This will generate an executable named `desh`.  

### Running the Shell  
Start the shell with:  
```bash
./desh
```

---

## Built-in Commands  
### `exit`  
Exits the shell. Optionally accepts a numerical exit code.  

### `which [command]`  
Finds the path of the first matching executable in `$PATH`.  

### `list [directory]`  
Lists files in the current directory or specified directories.  

### `pwd`  
Prints the current working directory.  

### `cd [directory]`  
Changes the working directory. Supports `cd -` for returning to the previous directory and `cd` (with no arguments) for returning to the home directory.  

### `prompt [prefix]`  
Customizes the shell prompt. If no argument is provided, prompts for a new prefix.  

### `printenv [VAR]`  
Displays the value of environment variables. If no variable is provided, displays all environment variables.  

### `setenv [VAR] [VALUE]`  
Sets or modifies environment variables.  

### `pid`  
Prints the process ID of the shell.  

---

## Signal Handling  
- **Ctrl-C**: Ignored by the shell but not by child processes.  
- **Ctrl-Z**: Ignored by the shell but not by child processes.  
- **Ctrl-D**: Does not terminate the shell.  

---

## Testing  
Use the following commands to test the shell:  
1. Execute built-in commands:  
   ```bash
   cd /usr
   pwd
   list
   ```
2. Execute external commands:  
   ```bash
   ls -l
   /bin/ls -l
   ```
3. Environment variable manipulation:  
   ```bash
   setenv TEST_VAR test_value
   printenv TEST_VAR
   ```

For a complete test suite, refer to the **Testing** section in the project description.

---

## Notes  
- Ensure no memory leaks by using `valgrind`.  
- Any warnings during compilation must be resolved.  

---

## Submission  
This project was completed as part of CISC361 coursework. To submit, push all changes to the GitLab repository and verify functionality with the provided test commands.  

--- 
