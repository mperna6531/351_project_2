#pragma once

#include "process.h"
#include "queue.h"
#include "memory.h"

Process* assignProcessList(const char* filePath);

char* getAnnouncementPrefix(int currentTime);

int getNumOfProcessFromFile(FILE* filePtr);

int main();

int processNumericInputFromUser(const char* output, int(*func)(int));

void assignAvailableMemoryWaitingProcs(int currentTime);

void clearStdin(char* buf);

void terminateCompletedProcs(int currentTime);

void enqueueNewArrivedProces(int currentTime);

void getUserInput(int* mem, int* page, char* filePath);

int isOneTwoOrThree(int t);

void mainLoop();

int multipleOneHundred(int t);

void printTurnaroundTime();

void promptForFileName(char* res);
