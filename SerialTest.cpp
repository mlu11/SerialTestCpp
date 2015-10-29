// SerialTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "comport.h"
#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include "SerialTest.h"

using namespace std;

// A internal method that takes a command line and execute it remotely
//git
// at signal generator.
void runCmd(char* cmd, DWORD error, char port, DWORD timeOut)
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
		printf("Error %08x writing %d.\n", error, port);
	}

	while (elapsedTime <= TIMEOUT)
	{
		//DWORD bytesRead;
		error = PxSerialRead(buffer, sizeof(buffer) - 1, &bytesRead);
		if (error != 0)
		{
			printf("Error %08x reading %d.\n", error, port);
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
}

int _tmain(int argc, _TCHAR* argv[])
{
	char* outputOn = ":OUTPut ON";
	char* outputOff = ":OUTPut OFF";

	char* port = new char[30];
	char pow_cmd[253];
    char freq_cmd[253];
	int freq_input = 0; // takes input as MHZ
	int pow_max = 0;
	int pow_min = 0;
	int steps = 0;
	int timeOut = 550;
	DWORD error = 0;
	

	printf(" Please enter the following commands: \n");

	//port entry
	printf(" Please enter the port number(COM#, like COM5 or COM6): ");
	cin >> port;

	//power entry
	printf(" Please enter the MAX amplitude(dBm): ");
	cin >> pow_max;

	printf(" Please enter the MIN amplitude(dBm): ");
	cin >> pow_min;

	printf(" Please enter the steps of the power sweep: ");
	cin >> steps;

	//frequency entry
	printf(" Please enter the desired freqency(MHz): ");
	cin >> freq_input;
	sprintf_s(freq_cmd, ":FREQ %d MHZ", freq_input);

	//time delay
	printf(" Please enter the desired delay for each instruction during sweep(seconds): ");
	cin >> timeOut;
	timeOut = timeOut *1000;

	// Open port
	error = PxSerialOpen(port);
	if (error != 0)
	{
		printf("Error %08x opening %s.\n", error, port);
		// stop the program.
	}
	else
	{
		do
		{	
			
			// Sweep run 
			//turn on output
			runCmd(outputOn, error, *port, 0);
			runCmd(freq_cmd, error, *port, 0);
			for (int i = pow_min; i <= pow_max; i += (pow_max - pow_min)/(steps - 1))
			{
				sprintf_s(pow_cmd, ":POW %d", i);
				runCmd(pow_cmd, error, *port, timeOut);
			}
			//trun of output
			runCmd(outputOff, error, *port, 0);

			//port value is stored on stack
			//power entry
			printf(" \n Please enter the MAX amplitude(dBm): ");
			cin >> pow_max;

			printf(" Please enter the MIN amplitude(dBm): ");
			cin >> pow_min;

			printf(" Please enter the steps of the power sweep: ");
			cin >> steps;

			//frequency entry
			printf(" Please enter the desired freqency(MHz): ");
			cin >> freq_input;
			sprintf_s(freq_cmd, ":FREQ %d MHZ", freq_input);

			//time delay
			printf(" Please enter the desired delay for each instruction during sweep(seconds): ");
			cin >> timeOut;
			timeOut = timeOut * 1000;

			//Todo: determine actually break 
			if (pow_cmd == "break" || freq_cmd == "break")
			{
				break;
			}

		} while (1);
	}

	printf("\n");

	// Close port
	error = PxSerialClose();
	if (error != 0)
	{
		printf("Error %08x closing %s.\n", error, port);
	}

	delete[]port; // get rid of spointers
	delete[]outputOff;
	delete[]outputOn;

	return 0;
}


 
