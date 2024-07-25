# Windows Keylogger with Network Communication

This C program is a keylogger with network communication capabilities designed to run on Windows. It includes functionalities to:
- Add itself to the Windows Registry for persistence
- Execute shell commands received from a remote server
- Capture keystrokes and send them to a remote server
- Hide the console window to run stealthily

## Features

- **Registry Persistence**: Adds the program to the Windows Registry to ensure it runs at startup.
- **Shell Command Execution**: Receives and executes shell commands from a remote server.
- **Keylogging**: Captures keystrokes and sends them to a remote server.
- **Stealth Mode**: Hides the console window to run without user detection.

## Requirements

- Windows operating system.
- MinGW-w64 installed on a Linux box to compile Windows executables.

## Installation

### On Linux

1. **Install MinGW-w64**:
   ```sh
   sudo apt install mingw-w64
   i686-w64-mingw32-gcc -o client.exe library.c -lwsock32 -lwininet

## On Windows 
    git clone https://github.com/yourusername/windows-keylogger.git
    cd windows-keylogger
    gcc -o client.exe library.c -lwsock32 -lwininet
    ./client.exe

## Functions
  
startAtBoot

Adds the program to the Windows Registry to ensure it runs at startup.

str_cut

Cuts a substring from a given string based on specified indices.

Shell

Executes shell commands received from a remote server and sends the output back.

WinMain

Main function that initializes the program, hides the console window, and connects to the remote server.

## Libraries Used

	•	Libraries Used:
	•	wsock32: For Windows sockets.
	•	wininet: For Internet functions.
	•	windows.h: For various Windows-specific functions and definitions.
	•	winuser.h: For user interface functions like MessageBox.
	•	wininet.h: For Internet functions.
	•	windowsx.h: For helper macros.
	•	sys/stat.h: For directory operations.
	•	sys/types.h: For data types.


    
