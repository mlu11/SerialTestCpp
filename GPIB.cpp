

#include "stdafx.h"
#include "comport.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include "GPIB.h"

using namespace std;

// set frequency with runCMD
void setFreq(int freq_input, DWORD error, char* port, DWORD timeOut)
{
	char freq_cmd[253];
	sprintf_s(freq_cmd, ":FREQ %d MHZ", freq_input);
	runCmd(freq_cmd, error, port, timeOut); // run command
}

// set frequency with runCMD
void setPow(int pow_input, DWORD error, char* port, DWORD timeOut)
{
	char pow_cmd[253];
	sprintf_s(pow_cmd, ":FREQ %d MHZ", pow_input);
	runCmd(pow_cmd, error, port, timeOut); // run command
}

//open port
void openPort(char* port)
{
	DWORD error = 0;
	// Open port
	error = PxSerialOpen(port);
	if (error != 0)
	{
		printf("Error %08x opening %s.\n", error, port);
		// stop the program.
	}
}

void outputON(DWORD error, char* port)
{
	char outputOn[253] = ":OUTPut ON";
	runCmd(outputOn, error, port, 0);
}
void outputOFF(DWORD error, char* port)
{
	char outputOff[253] = ":OUTPut OFF";
	runCmd(outputOff, error, port, 0);
}

// A internal method that takes a command line and execute it remotely
//git
// at signal generator.
void runCmd(char* cmd, DWORD error, char* port, DWORD timeOut)
{
	// Append CR and LF
	char buffer[256];
	DWORD elapsedTime = 0;
	DWORD lastRead = timeGetTime();
	DWORD bytesRead = 0;
	DWORD TIMEOUT = timeOut;

	sprintf_s(buffer, "%s\r\n", cmd);

	// Write command to port
	DWORD written = 0;
	error = PxSerialWrite(buffer, (DWORD)strlen(buffer), &written);

	if (error != 0)
	{
		printf("Error %08x writing %d.\n", error, *port);
	}

	while (elapsedTime <= TIMEOUT)
	{
		//DWORD bytesRead;
		error = PxSerialRead(buffer, sizeof(buffer) - 1, &bytesRead);
		if (error != 0)
		{
			printf("Error %08x reading %d.\n", error, *port);
			break;
		}

		if (bytesRead > 0)
		{
			buffer[bytesRead] = 0;    // Append NULL to print to console
			printf("%s", buffer);

			lastRead = timeGetTime();
		}

		elapsedTime = timeGetTime() - lastRead;
	}
} // end runCmd
