#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
public:
    int Pid();                               // COMPLETED: See src/process.cpp
    std::string User();                      // COMPLETED: See src/process.cpp
    std::string Command();                   // COMPLETED: See src/process.cpp
    float CpuUtilization() const;                  // COMPLETED: See src/process.cpp
    std::string Ram();                       // COMPLETED: See src/process.cpp
    long int UpTime();                       // COMPLETED: See src/process.cpp
    bool operator<(Process const& a) const;  // COMPLETED: See src/process.cpp

    void Pid(int procId);
  // COMPLETED: Declare any necessary private members
private:
    int procId_;
};

#endif
