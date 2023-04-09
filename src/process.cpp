#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

void Process::Pid(int procId) { procId_ = procId; }

// COMPLETED: Return this process's ID
int Process::Pid() { return procId_; }

// COMPLETED: Return this process's CPU utilization
float Process::CpuUtilization() const
{
    float nonidle = LinuxParser::ActiveJiffies();
    float idle = LinuxParser::IdleJiffies();
    return LinuxParser::ActiveJiffies(procId_) / (nonidle + idle);
}

// COMPLETED: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(procId_); }

// COMPLETED: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(procId_); }

// COMPLETED: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(procId_); }

// COMPLETED: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(procId_);; }

// COMPLETED: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& process) const
{
    return CpuUtilization() < process.CpuUtilization();
}