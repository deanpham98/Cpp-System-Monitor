#include "processor.h"
#include "linux_parser.h"

#include <vector>
#include <string>

using std::vector;
using LinuxParser::kUser_;
using LinuxParser::kNice_;
using LinuxParser::kSystem_;
using LinuxParser::kIdle_;
using LinuxParser::kIOwait_;
using LinuxParser::kIRQ_;
using LinuxParser::kSoftIRQ_;
using LinuxParser::kSteal_;
using LinuxParser::kGuest_;
using LinuxParser::kGuestNice_;

// // Constructor
// Processor::Processor() {
//   	prev_stat = vector<int>(10);
// 	Utilization();
// }

// TOTEST: Return the aggregate CPU utilization
float Processor::Utilization() {
	vector<long> stat = LinuxParser::CpuUtilization();
	
//   	int PrevIdle = prev_stat[kIdle_] + prev_stat[kIOwait_];
  	float Idle = stat[kIdle_] + stat[kIOwait_];
//   	int PrevNonIdle = prev_stat[kUser_] + prev_stat[kNice_] + prev_stat[kSystem_] + prev_stat[kIRQ_] + prev_stat[kSoftIRQ_] + prev_stat[kSteal_];
  	float NonIdle = stat[kUser_] + stat[kNice_] + stat[kSystem_] + stat[kIRQ_] + stat[kSoftIRQ_] + stat[kSteal_];
//   	int PrevTotal = PrevIdle + PrevNonIdle;
  	float Total = Idle + NonIdle;
//   	int totald = Total - PrevTotal;
//   	int idled = Idle - PrevIdle;
  
  	float CPU_Percentage = NonIdle / Total;
//   	prev_stat = stat;
  	return CPU_Percentage;
}