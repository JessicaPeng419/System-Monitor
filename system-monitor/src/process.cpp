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

// TODO: Return this process's ID
void Process::Pid(int pid) { PID = pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const
{
  float nonidle = LinuxParser::ActiveJiffies();
  float idle = LinuxParser::IdleJiffies();
  return LinuxParser::ActiveJiffies(PID)/(nonidle + idle);
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(PID); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(PID); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(PID); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(PID); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& process) const {
  return CpuUtilization() < process.CpuUtilization();
}