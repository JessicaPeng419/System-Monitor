#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
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

// COMPLETED: Read and return the system memory utilization
float LinuxParser::MemoryUtilization()
{
    std::vector<long> memValues{};
    std::string line, propName, propValue;
    std::ifstream memInfoFile(kProcDirectory + kMeminfoFilename);
    if (memInfoFile.is_open())
    {
        int lineNum = 0;
        // 1. only read required lines
        while (std::getline(memInfoFile, line) && lineNum < 2)
        {      
            std::istringstream lineStream(line);
            lineStream >> propName >> propValue;

            // 2. convert string to long & push it to the vector
            memValues.push_back(stol(propValue));

            lineNum++;
        }
    }

    long memTotal = memValues[0];
    return ((memTotal - memValues[1]) * 100.0 / memTotal) / 100.0;
}

// COMPLETED: Read and return the system uptime
long LinuxParser::UpTime()
{
    std::string line;
    long systemUptime, idleUpTime;
    std::ifstream upTimeInfoFile(kProcDirectory + kUptimeFilename);
    if (upTimeInfoFile.is_open())
    {
        std::getline(upTimeInfoFile, line);
        std::istringstream lineStream(line);
        lineStream >> systemUptime >> idleUpTime;
    }

    return systemUptime;
}

// COMPLETED: Read and return the number of jiffies for the system
long LinuxParser::Jiffies()
{
    return UpTime() * sysconf(_SC_CLK_TCK);
}

// COMPLETED: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid)
{
    string line, propValue;
    float jiffies = 0.0;
    std::ifstream pidFileStream(kProcDirectory + std::to_string(pid) + kStatFilename);
    if (pidFileStream.is_open())
    {
        while (getline(pidFileStream, line))
        {
            std::istringstream lineStream(line);
            for (int i{0}; i < 17; i++)
            {
                lineStream >> propValue;
                if (i >= 13)
                    jiffies = stol(propValue);
            }
        }
    }

    return jiffies;
}

// COMPLETED: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies()
{
    auto cpu_load = LinuxParser::CpuUtilization();
    vector<int> cpu_states{
        CPUStates::kUser_, CPUStates::kNice_,
        CPUStates::kSystem_, CPUStates::kSoftIRQ_,
        CPUStates::kSteal_, CPUStates::kGuest_,
        CPUStates::kGuestNice_
    };

    float non_idle = 0.0;
    for (int& i : cpu_states)
      non_idle += stol(cpu_load[i]);
    
    return non_idle;
}

// COMPLETED: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies()
{
    auto cpu_load = LinuxParser::CpuUtilization();
    vector<int> cpu_states{CPUStates::kIdle_, CPUStates::kIOwait_};
    float idle = 0.0;
    for (int& i : cpu_states)
      idle += stol(cpu_load[i]);
    
    return idle;
}

// COMPLETED: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization()
{
    vector<string> segments;
    string part;
    std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
    if (stream.is_open()) {
      int count = 0;
        while (std::getline(stream, part, ' ') && count < 12)
        {
            segments.push_back(part);
            count++;
        } 
    }

    // remove unnecessary first 2 element & make memory more compact
    vector<decltype(segments)::value_type>(segments.begin() + 2, segments.end()).swap(segments);
    
    return segments;
}

// COMPLETED: Read and return the total number of processes
int LinuxParser::TotalProcesses()
{
    string line, key, value;
    int processes = 0;
    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open())
    {
        while (std::getline(filestream, line))
        {
            std::istringstream linestream(line);
            while (linestream >> key >> value) 
            {
                if (key == "processes")
                {
                    processes = std::stoi(value);
                    break;
                }
            }
        }
    }
    return processes;
}

// COMPLETED: Read and return the number of running processes
int LinuxParser::RunningProcesses()
{
    string line, key, value;
    int running_processes = 0;
    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open())
    {
        while (std::getline(filestream, line))
        {
            std::istringstream linestream(line);
            while (linestream >> key >> value) 
            {
                if (key == "procs_running")
                {
                    running_processes = std::stoi(value);
                    break;
                }
            }
        }
    }
    
    return running_processes;
}

// COMPLETED: Read and return the command associated with a process
string LinuxParser::Command(int pid)
{
    string cmd = "";
    std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
    if (filestream.is_open()) {
        std::getline(filestream, cmd);
        return cmd;
    }
    return cmd;
}

// COMPLETED: Read and return the memory used by a process
string LinuxParser::Ram(int pid)
{
    string line, key, value;
    int vm_size = 0;
    std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                            kStatusFilename);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            while (linestream >> key >> value) {
                if (key == "VmSize:") {
                    vm_size = std::stoi(value);
                    break;
                }
            }
        }
    }
    
    return std::to_string(vm_size / 1024);
}

// COMPLETED: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid)
{
    string line, key, value, uid = "";
    std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                            kStatusFilename);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            while (linestream >> key >> value) {
                if (key == "Uid:") {
                    uid = value;
                    break;
                }
            }
        }
    }
    return uid;
}

// COMPLETED: Read and return the user associated with a process
string LinuxParser::User(int pid)
{
    string line, s_this_x, s_this_uid, s_this_usrname, s_usrname = "NoUser";
    string s_desired_uid = Uid(pid);
    std::ifstream filestream(kPasswordPath);
    if (filestream.is_open()) {
        // NOTE: c++ understands ':'singlequoted as char.... for some reason
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            std::getline(linestream, s_this_usrname, ':');
            std::getline(linestream, s_this_x, ':');
            std::getline(linestream, s_this_uid, ':');
            if (s_this_uid == s_desired_uid) {
                s_usrname = s_this_usrname;
                break;
            }
        }
    }
    return s_usrname;
}

// COMPLETED: Read and return the uptime of a process
long LinuxParser::UpTime(int pid)
{
    string line, value;
    float jiffies = 0.0;
    std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                            kStatFilename);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            for (int i{0}; i < 22; i++) {
                linestream >> value;
                if (i == 21)
                    jiffies = std::stol(value);
            }
        }
    }
    return (UpTime() - (jiffies / sysconf(_SC_CLK_TCK)));
}
