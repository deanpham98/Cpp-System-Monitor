#ifndef PROCESS_H
#define PROCESS_H

#include "linux_parser.h"
#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
	public:
		// Constructors
		Process(int id) : pid(id) { 
			username = LinuxParser::User(id);
			cmd = LinuxParser::Command(id);
		}

		// Member Functions
		int Pid();                               // TODO: See src/process.cpp
		std::string User();                      // TODO: See src/process.cpp
		std::string Command();                   // TODO: See src/process.cpp
		float CpuUtilization() const;                  // TODO: See src/process.cpp
		std::string Ram() const;                       // TODO: See src/process.cpp
		long int UpTime();                       // TODO: See src/process.cpp
		bool operator<(Process const& a) const;  // TODO: See src/process.cpp

	// TODO: Declare any necessary private members
	private:
		int pid;
		std::string username;
		std::string cmd;
};

#endif