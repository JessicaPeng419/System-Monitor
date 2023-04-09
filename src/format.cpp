#include <string>

#include "format.h"

// COMPLETED: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
std::string Format::ElapsedTime(long seconds)
{
    // calculate hours, minutes & seconds
    std::string h = std::to_string(seconds / 3600);
    std::string m = std::to_string((seconds % 3600) / 60);
    std::string s = std::to_string(seconds % 60);

    // add leading zeros if needed
    std::string hh = std::string(2 - h.length(), '0') + h;
    std::string mm = std::string(2 - m.length(), '0') + m;
    std::string ss = std::string(2 - s.length(), '0') + s;

    // compare, concat & return
    if (hh.compare("00") == 0)
        return mm + ":" + ss;
    
    // otherwise, concat & return
    return hh + ":" + mm + ":" + ss;
}
