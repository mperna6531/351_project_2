# 351_project_2

CPSC 351-02

Group Members:

Michael Perna		mperna@csu.fullerton.edu
Stephen Shinn		smshinn@csu.fullerton.edu
Jasper Chen		  jsprchn2@csu.fullerton.edu
Oscar Cheung		cheung.oscar@csu.fullerton.edu
Katherine Davis	kdavis890892003@csu.fullerton.edu

Program Info:
  This program is designed to simulate the process of memory management at the system level.  A memory size and page size can be input by the user, along with a workload file of processes annd information.  This information is used to simulate the arrival and departure of processes of with various memory requirements.  The processes are assigned to memory and executed on a first in first out basis.  Information about these events is displayed to the user through STDOUT.
  
Language: C++
  Developed on the GNU/Linux operating system using ubuntu 18.04LTS

To Run:
  A makefile is included.  Type 'make' in the terminal of the directory to execute compile command.  The program should automatically execute an one file named 'mem_sim' will be generated.  To execute the program type './mem_sim' in the terminal from the directory where the file is located.  You will then be prompted to enter a memory size, page size, and file name for the simulation.
  
Contributions:
Michael: design and implement MemoryManager class and help with testing.
Stephen: desing and implement FrameList class and help with testing.  
Jasper: design implement Process class and help with testing.
Katherine:  design and implement the Frame class;
Oscar: Project design/documentation.
