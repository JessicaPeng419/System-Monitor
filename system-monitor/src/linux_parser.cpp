#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::getline;

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
  string os, kernel, version;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  std::ifstream filestream (kProcDirectory + kMeminfoFilename);
  string line;
  string key;
  string value;
  float memTotal = 0.0;
  float memFree = 0.0;

  if (filestream.is_open()) {
    while(getline(filestream, line)){
      // remove space and ':'
      std::remove(line.begin(), line.end(), ' ');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        // search for MemTotal
        if (key == "MemTotal") {
          memTotal = std::stof(value);
        }
        // search for MemFree
        else if (key == "MemFree") {
          memFree = std::stof(value);
          break;
        }
      }
    }
  }
  return ((memTotal - memFree)/memTotal);
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  long systemUptime, idleUptime;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    getline(filestream, line);
    std::istringstream lineStream(line);
    lineStream >> systemUptime >> idleUptime;
  }
  return 0;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  return UpTime() * sysconf(_SC_CLK_TCK);
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  string line, propValue;
  long jiffies = 0;
  std::ifstream pidFileStream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (pidFileStream.is_open()){
    while (getline(pidFileStream, line)) {
      std::istringstream  linestream(line);
      for (int i{0}; i<17; i++) {
        linestream >> propValue;
        if (i >= 13) {
          jiffies = stol(propValue);
          }
        }
      }
    }
    return jiffies;
  }


// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
    auto cpu_load = LinuxParser::CpuUtilization();
    vector<int> cpu_states {
        CPUStates::kUser_, CPUStates::kNice_, CPUStates::kSystem_, CPUStates::kSoftIRQ_,
        CPUStates::kSteal_, CPUStates::kGuest_, CPUStates::kGuestNice_
    };
    float non_idle = 0.0;
    for (int& i : cpu_states) {
      non_idle += std::stol(cpu_load[i]);
    }
    return non_idle;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
    auto cpu_load = LinuxParser::CpuUtilization();
    vector<int> cpu_states {CPUStates::kIdle_, CPUStates::kIOwait_};
    float idle = 0.0;
    for (int& i : cpu_states) {
      idle += std::stol(cpu_load[i]);
    }
    return idle;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
    vector<string> segments;
    string part;
    std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
    if (filestream.is_open()) {
      int count = 0;
      while (getline(filestream, part, ' ') && count < 12) {
        segments.push_back(part);
        count ++;
      }
    }
    // remove the first 2 elements
    vector<decltype(segments)::value_type> (segments.begin() + 2, segments.end()).swap(segments);
    return segments;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
    string line, key, value;
    int processes = 0;
    std::ifstream filestream (kProcDirectory + kStatFilename);
    if (filestream.is_open()) {
      while(getline(filestream, line)){
        std::istringstream linestream(line);
        while (linestream >> key >> value) {
          if (key == "processes") {
          processes = std::stoi(value);
          break;
          }
        }
      }
    }
    return processes;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
    string line, key, value;
    int running_processes = 0;
    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open()) {
      while (getline(filestream, line)) {
        std::istringstream  linestream(line);
        while(linestream >> key >> value) {
          if(key == "procs_running") {
            running_processes = std::stoi(value);
            break;
          }
        }
      }
    }
    return running_processes;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
    string cmd = "";
    std::ifstream filestream (kProcDirectory + to_string(pid) + kCmdlineFilename);
    if (filestream.is_open()) {
      getline(filestream, cmd);
      return cmd;
    }
    return string();
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
    string line, key, value;
    int vm_size = 0;
    std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
    if (filestream.is_open()) {
      while (getline(filestream, line)) {
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

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
    string line, key, value, uid = "";
    std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
    if (filestream.is_open()) {
      while(getline(filestream, line)) {
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

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
    string line, cur_x, cur_uid, cur_usrname, p_usrname = "NoUser";
    string p_uid = Uid(pid);
    std::ifstream filestream(kPasswordPath);
    if (filestream.is_open()) {
      while (getline(filestream, line)) {
        std::istringstream  linestream(line);
        getline(linestream, cur_usrname, ':');
        getline(linestream, cur_x, ':');
        getline(linestream, cur_uid, ':');
        if (cur_uid == p_uid) {
          p_usrname = cur_usrname;
          break;
        }
      }
    }
    return p_usrname;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
    string line, value;
    long jiffies = 0;
    std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
    if (filestream.is_open()) {
      while (getline(filestream, line)) {
        std::istringstream linestream(line);
        for (int i{0}; i<22; i++) {
          linestream >> value;
          if (i == 21)
            jiffies = std::stol(value);
        }
      }
    }
    return (UpTime() - (jiffies / sysconf(_SC_CLK_TCK)));
}
