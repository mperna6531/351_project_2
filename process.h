#pragma once

struct Process
{
	int pid;
	int arrivalTime;
	int lifeTime;
	int memReqs;

	int timeAddMemory;
	int isActive;
	int timeDone;
};