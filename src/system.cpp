#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TOTEST: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TOTEST: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  vector<int> pids = LinuxParser::Pids();
  processes_.clear();
  for (auto it = pids.cbegin(); it != pids.cend(); ++it) {
  	processes_.push_back(Process(*it));
  }
  
  std::sort(processes_.begin(), processes_.end());
  
  return processes_;
}

// TOTEST: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TOTEST: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TOTEST: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TOTEST: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TOTEST: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TOTEST: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }