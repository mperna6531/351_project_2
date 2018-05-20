// CPSC 351 Assignment 2.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "process.h"
#include "memory.h"
#include "memorymanager.h"

int main() {
	std::string file_name;
	int mem_size, page_size;
	
	std::cout << "Memory size: ";
	std::cin >> mem_size;
	std::cout << "Page size: ";
	std::cin >> page_size;

	while (!(mem_size % page_size == 0)) {
		std::cout << "Error: Memory size must be a multiple of page size." << std::endl;
		std::cout << "Please re-enter memory size and page size." << std::endl;
		std::cout << "Memory size: ";
		std::cin >> mem_size;
		std::cout << "Page size: ";
		std::cin >> page_size;
	}

	std::cout << "Input file: ";
	std::cin >> file_name;

	MemoryManager mm(mem_size, page_size, file_name);
	mm.simulate();

	return 0;
}
