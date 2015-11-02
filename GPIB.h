#ifndef __GPIB_H__
#define __GPIB_H__

#include <stdio.h>
#include <tchar.h>
#include <windows.h>

void setFreq(int freq_input, DWORD error, char* port, DWORD timeOut);
void setPow(int pow_input, DWORD error, char* port, DWORD timeOut);
void openPort(char* port);
void outputON(DWORD error, char* port);
void outputOFF(DWORD error, char* port);
void runCmd(char * cmd, DWORD * error, DWORD * port);

#endif
