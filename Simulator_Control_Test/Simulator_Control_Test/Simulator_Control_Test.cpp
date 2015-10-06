// Simulator_Control_Test.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <iostream>
#include "mfsimulator.h"

int main(int argc, char* argv[])
{
	char ans = 'y';
	char pitch_roll = 'n';
	int pitch_in, roll_in;

	MFSimulator mfsim;

	while (ans == 'y' || ans == 'Y')
	{
		printf("Change pitch or roll or exit (p/r/x)? ");
		std::cin >> pitch_roll;
		
		if (pitch_roll == 'p' || pitch_roll == 'P'){
			printf("Enter pitch data: ");
			std::cin >> pitch_in;
			mfsim.setPitch(pitch_in);
		}
		else if (pitch_roll == 'r' || pitch_roll == 'R'){
			printf("Enter roll data: ");
			std::cin >> roll_in;
			mfsim.setRoll(roll_in);
		}
		else if (pitch_roll == 'x' || pitch_roll == 'X'){
			printf("Exiting.");
			return 0;
		}
		else {
			printf("Enter a valid option you fuck!!\n");
			continue;
		}



	}
	return 0;
}

