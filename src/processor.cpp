#include "processor.h"
#include "linux_parser.h"

// COMPLETEED: Return the aggregate CPU utilization
float Processor::Utilization()
{
    float nonidle = LinuxParser::ActiveJiffies();
    // Idle = idle + iowait
    float idle = LinuxParser::IdleJiffies();
    // Total = Idle + NonIdle
    // CPU_Percentage = (Total - idled)/totald
    if ((nonidle + idle) == 0)
        return 0.0;
    
    return nonidle / (nonidle + idle);
}
