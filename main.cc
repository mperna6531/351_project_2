// main.cpp
#include <iostream>
#include "memorymanager.hpp"

int main() {
  std::string fileInput;
	int memSize, pageSize;
	bool validPage = false;

	std::cout << "Memory size: ";
	std::cin >> memSize;
	std::cout << "Page size: ";
	std::cout << "Page size (1:100, 2:200, 3:300): ";

  
  while (!validPage) {
	  std::cin.ignore();
	  std::cin >> pageSize;
	  switch (pageSize) {
	    case 1: 
	      pageSize = 100;
		  validPage = true;
		  break;
	    case 2:
	      pageSize = 200;
		  validPage = true;
		  break;
	    case 3: 
	      pageSize = 400;
		  validPage = true;
		  break;
	    default:
	      std::cout << "Please make a valid selection: ";
	  }
	}

	std::cout << "Workload file: ";
	std::cin >> fileInput;

	MemoryManager mm(memSize, pageSize, fileInput);
	mm.simulate();

	return 0;
}