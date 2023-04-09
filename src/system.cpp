#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// COMPLETED: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// COMPLETED: Return a container composed of the system's processes
vector<Process>& System::Processes()
{
    vector<int> pids = LinuxParser::Pids();
    processes_.clear();

  	vector<Process> temp{};
    for (int pid : pids) {
        Process proc;
        proc.Pid(pid);
      	temp.emplace_back(proc);
    }
  
  	std::copy_if(temp.begin(), temp.end(), std::back_inserter(processes_), [](Process p){ return p.Command() != ""; } );
  
    std::sort(processes_.begin(), processes_.end());
    return processes_;
}

// COMPLETED: Return the system's kernel identifier (string)
string System::Kernel() { return LinuxParser::Kernel(); }

// COMPLETED: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// COMPLETED: Return the operating system name
string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// COMPLETED: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// COMPLETED: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// COMPLETED: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
