#include "processor.h"
# include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  float nonidle = LinuxParser::ActiveJiffies();
  float idle = LinuxParser::IdleJiffies();
  if ((nonidle + idle) == 0) {
    return 0.0;
  }
  return nonidle/(nonidle + idle);

}