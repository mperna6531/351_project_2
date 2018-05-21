// main.cpp
#include <iostream>
#include "memorymanager.hpp"

int main() {
	std::string fileInput;
	int memSize, pageSize;
	
	std::cout << "Memory size: ";
	std::cin >> memSize;
	std::cout << "Page size: ";
	std::cin >> pageSize;

	while (!(memSize % pageSize == 0)) {
		std::cout << "Error: Memory size must be a multiple of page size." << std::endl;
		std::cout << "Please re-enter memory size and page size." << std::endl;
		std::cout << "Memory size: ";
		std::cin >> memSize;
		std::cout << "Page size: ";
		std::cin >> pageSize;
	}

	std::cout << "Input file: ";
	std::cin >> fileInput;

	MemoryManager mm(memSize, pageSize, fileInput);
	mm.simulate();

	return 0;
}
