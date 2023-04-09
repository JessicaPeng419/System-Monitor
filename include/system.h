#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu();                   // COMPLETED: See src/system.cpp
  std::vector<Process>& Processes();  // COMPLETED: See src/system.cpp
  float MemoryUtilization();          // COMPLETED: See src/system.cpp
  long UpTime();                      // COMPLETED: See src/system.cpp
  int TotalProcesses();               // COMPLETED: See src/system.cpp
  int RunningProcesses();             // COMPLETED: See src/system.cpp
  std::string Kernel();               // COMPLETED: See src/system.cpp
  std::string OperatingSystem();      // COMPLETED: See src/system.cpp

  // TODO: Define any necessary private members
 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
};

#endif