#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  if (seconds<=0) {
    return "00:00:00";
  }
  string time;
  long t;
  t = seconds/3600;
  time = TimeToString(t) + ":";
  t = (seconds/60)%60;
  time += TimeToString(t) + ":";
  t = seconds%60;
  time += TimeToString(t);
  return time;
}

string Format::TimeToString(long t) {
  if (t<10) {
    return "0" + std::to_string(t);
  }
  else {
    return std::to_string(t);
  }
}