# 🐚 A Linux Shell Implementation

This project implements a basic shell in **C** under **Linux**. The shell can execute user commands and includes built-in functionality for managing background processes.

## ✨ Features

- Executes both **foreground** and **background** commands
- Supports built-in commands: `exit`, `cd`, and `jobs`
- Manages up to **4 background processes** simultaneously
- Tracks and reports on **process status**
- Handles **command parsing** and execution
- Prevents **zombie processes** by properly reaping child processes

## 🔧 Built-in Commands

### `exit`
Terminates the shell after waiting for all background processes to complete.

### `cd <directory>`
Changes the current working directory.

### `jobs`
Displays a list of currently running background processes.

## 📊 Background Process Management

- Commands ending with `&` are executed **in the background**.
- Up to **4 background processes** can run simultaneously.
- Background process status is **reported** when they start and finish.

## ⚠️ Error Handling

The shell provides specific error messages for various conditions:

- **Invalid commands**
- **Too many background processes**
- **System call failures** with corresponding `errno` values

## 🛠️ Building the Project

To build the project:

```bash
make
```

To clean up build files:

```bash
make clean
```

## ▶️ Usage

After building, run the shell with:

```bash
./hw1shell
```

You'll be presented with the prompt `hw1shell$`, where you can enter commands.

### 📌 Example Commands

```bash
hw1shell$ ls -l
hw1shell$ sleep 10 &
hw1shell$ jobs
hw1shell$ cd /tmp
hw1shell$ echo "Hello, world!"
hw1shell$ exit
```

## 🧑‍💻 Implementation Details

- Uses `fork()` to create child processes
- Executes commands with `execvp()`
- Manages process waiting with `waitpid()`
- Implements directory changing with `chdir()`
- Handles background process tracking with a custom data structure
- Supports up to **1024 characters** per command line


