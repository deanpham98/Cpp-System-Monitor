#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
/*********System*********/
// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TOTEST: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string path{kProcDirectory + kMeminfoFilename};
  std::ifstream is(path);
  if (is.is_open()) {
  	string line;
    float mem_total, mem_free, buffers, cached, slab;
    while(std::getline(is, line)) {
    	std::istringstream iss(line);
      	string key;
      	
      	iss >> key;
      	if (key == "MemTotal:") {
        	iss >> mem_total;
        } else if (key == "MemFree:") {
        	iss >> mem_free;
        } else if (key == "Buffers:") {
        	iss >> buffers;
        } else if (key == "Cached:") {
        	iss >> cached;
        } else if (key == "Slab:") {
        	iss >> slab;
        }
    }

    return (mem_total - (mem_free + buffers + cached + slab)) / mem_total;
  }
  
  return 0.0;
}

// TOTEST: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string path{kProcDirectory + kStatFilename};
  std::ifstream is(path);
  string line;
  
  if (is.is_open()) {
    while (std::getline(is, line)) {
      std::istringstream iss(line);
      string key;
      iss >> key;
      if  (key == "processes") {
          int total;
          iss >> total;
          return total;
      }
    }
  }
  
  return 0;
}

// TOTEST: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string path{kProcDirectory + kStatFilename};
  std::ifstream is(path);
  string line;
  
  if (is.is_open()) {
    while (std::getline(is, line)) {
      std::istringstream iss(line);
      string key;
      iss >> key;
      if  (key == "procs_running") {
          int total;
          iss >> total;
          return total;
      }
    }
  }
  
  return 0;
}

// TOTEST: Read and return the system uptime
long LinuxParser::UpTime() {
  string path{kProcDirectory + kUptimeFilename};
  std::ifstream is(path);
  
  if (is.is_open()) {
    long num;
    is >> num;
    return num;
  }
  return 0;
}


/*********Processor**********/
// // TODO: Read and return the number of jiffies for the system
// long LinuxParser::Jiffies() { return 0; }

// // TODO: Read and return the number of active jiffies for a PID
// // REMOVE: [[maybe_unused]] once you define the function
// long LinuxParser::ActiveJiffies(int pid) { return 0; }

// // TODO: Read and return the number of active jiffies for the system
// long LinuxParser::ActiveJiffies() { return 0; }

// // TODO: Read and return the number of idle jiffies for the system
// long LinuxParser::IdleJiffies() { return 0; }

// TOTEST: Read and return CPU utilization
vector<long> LinuxParser::CpuUtilization() {
  vector<long> ret;
  string path{kProcDirectory + kStatFilename};
  std::ifstream is(path);
  
  if (is.is_open()) {
  	string line;
    while (std::getline(is, line)) {
    	string first;
      	std::istringstream iss(line);
      	iss >> first;
      	if (first == "cpu") {
          	long num;
          	while(iss >> num) {
            	ret.push_back(num);
              	if (ret.size() == 10)
                  return ret;
            }
        }
    }
  }
  
  return ret;
}

/*********Process**********/
// TOTEST: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
	string path {kProcDirectory + to_string(pid) + kCmdlineFilename};
	std::ifstream is(path);
	string ret;
	if (is.is_open()) {
		std::getline(is, ret);
	}
	return ret;
}

// TOTEST: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
	string path {kProcDirectory + to_string(pid) + kStatusFilename};
	std::ifstream is(path);
	if (is.is_open()) {
		string line;
		while(std::getline(is, line)) {
			std::istringstream iss(line);
			string key;
			iss >> key;
			if (key == "VmSize:") {
				float val;
				iss >> val;
              	std::ostringstream os;
              	os << std::fixed << std::setprecision(2) << (val / 1024);
				return os.str();
			}
		}
	}
	return string();
}

// TOTEST: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
	string path {kProcDirectory + to_string(pid) + kStatusFilename};
	std::ifstream is(path);
	
	if (is.is_open()) {
		string line;
		while(std::getline(is, line)) {
			std::istringstream iss(line);
			string key;
			iss >> key;
			if (key == "Uid:") {
				string uid;
				iss >> uid;
				return uid;
			}
		}
	}
	
	return string();
}

// TOTEST: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
	string path {kPasswordPath};
	std::ifstream is(path);
	
	if (is.is_open()) {
		string line;
		while(std::getline(is, line)) {
			size_t idx = line.find(":" + Uid(pid) + ":");
			if (idx != string::npos) {
				string ret;
				for (auto it = line.cbegin(); it != line.cend(); ++it) {
                  	if (*it == ':') break;
						ret += *it;
				}

				return ret;
			}
		}
	}

	return string();
}

// TOTEST: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  	string path{kProcDirectory + to_string(pid) + kStatFilename};
  	std::ifstream is(path);
	
	if (is.is_open()) {
      	int i = 0;
		string temp;
		while(is >> temp && i < 20) { i++; }
      	long num;
      	is >> num;
      	return num / sysconf(_SC_CLK_TCK);
	}

	return 0;
}

// TOTEST: Read and return the CPU Utilization associated with a process
float LinuxParser::ProcessCpuUtil(int pid) {
	string path {kProcDirectory + to_string(pid) + kStatFilename};
	std::ifstream is(path);
	
	if (is.is_open()) {
      	long utime, stime, cutime, cstime;
        long starttime;
		string temp;
      	for (int i = 1; i <= 13; i++) {
        	is >> temp;
        }
      
      	is >> utime;
      	is >> stime;
      	is >> cutime;
      	is >> cstime;
      	for (int i = 18; i <= 21; i++) {
        	is >> temp;
        }
      
      	is >> starttime;
      
      	float total_time = utime + stime + cutime + cstime;
      	float seconds = UpTime() - (starttime / sysconf(_SC_CLK_TCK));
      	float cpu_usage = (total_time / sysconf(_SC_CLK_TCK)) / seconds;      
      	return cpu_usage;
	}
	
	return 0.0;
}