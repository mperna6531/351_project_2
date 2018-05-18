#pragma once

#include "process.h"

struct Frame
{
	int assigned;
	char location[40];
	int procAssign;
	int pageNum;
};

struct frameList
{
	Frame* frames;
	int numOfFrames;
	int pageSize;
};

frameList* createFrameList(int numOfFrames, int pageSize)
{
	frameList *f;

	f = malloc(sizeof(frameList));

	f->frames = (Frame*)malloc(sizeof(Frame)) *numOfFrames);
	f->pageSize = pageSize;
	f->numOfFrames = numOfFrames;

	for (int i = 0; i < f->numOfFrames; i++)
	{
		f->frames[i].assigned = 0;
	}

	return f;
}

int procFitMemory(frameList* list, Process* proc)
{
	int numFreeFrames = 0;

	for (int i = 0; i < list->numOfFrames; i++)
	{
		if (!list->frames[i].assigned)
		{
			numFreeFrames += 1;
		}
	}

	return (numFreeFrames * list->pageSize) >= proc->memReqs;
}

void fitProcMemory(frameList* list, Process* proc)
{
	int remainingMem, currentPage = 1;

	remainingMem = proc->memReqs;

	for (int i = 0; i < list->numOfFrames; i++)
	{
		if (!list->frames[i].assigned)
		{
			list->frames[i].assigned = 1;

			list->frames[i].pageNum = currentPage;

			list->frames[i].procAssign = proc->pid;

			currentPage++;
			remainingMem -= list->pageSize;
		}

		if (remainingMem <= 0)
		{
			break;
		}
	}
}

void printFrameList(frameList* list)
{
	int inFreeBlock = 0, start;

	printf("\tMemory map:\n");

	for (int i = 0; i < list->numOfFrames; i++)
	{
		if (!inFreeBlock && !list->frames[i].assigned)
		{
			inFreeBlock = 1;
			start = i;
		}
		else if (inFreeBlock && list->frames[i].assigned)
		{
			inFreeBlock = 0;
			printf("\t\t%d-%d: Free frame(s) \n", 
				start * list->pageSize, 
				(i * list->pageSize) - 1);
		}

		if (list->frames[i].assigned)
		{
			printf("\t\t%d-%d: Process %d, Page %d\n",
				i * list->pageSize,
				((i + 1) * list->pageSize) - 1,
				list->frames[i].procAssign,
				list->frames[i].pageNum);
		}
	}

	if (inFreeBlock)
	{
		printf("\t\t%d-%d: Free frame(s)\n",
			start * list->pageSize,
			((i)* list->pageSize) - 1);
	}
}

int frameListEmpty(frameList* list)
{
	for (int i = 0; i < list->numOfFrames; i++)
	{
		if (list->frames[i].assigned)
		{
			return 0;
		}
	}

	return 1;
}

void freeMemoryPID(frameList* list, int pid)
{
	Frame* frame;

	for (int i = 0; i < list->numOfFrames; i++)
	{
		frame = &list->frames[i];

		if (frame->procAssign == pid)
		{
			frame->procAssign = 0;
			frame->pageNum = 0;
			frame->assigned = 0;
		}
	}
}