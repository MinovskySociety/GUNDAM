#ifndef _GUNDAM_UTIL_STRING_H 
#define _GUNDAM_UTIL_STRING_H

#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <cctype>

namespace GUNDAM {

namespace util {

// inline std::vector<std::string> split(std::string const &input);

// inline std::vector<std::string> split(const std::string &s, char delim);

// inline std::string tolower(const std::string& str);

template <typename Out>
void split(const std::string &s, char delim, Out result) {
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        *result++ = item;
    }
}

inline std::vector<std::string> split(std::string const &input) { 
    std::istringstream buffer(input);
    std::vector<std::string> ret((std::istream_iterator<std::string>(buffer)), 
                                 std::istream_iterator<std::string>());
    return ret;
}

inline std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

inline std::string tolower(const std::string& str) {
  std::string my_str = str;
  std::transform(my_str.begin(), 
                 my_str.end(), 
                 my_str.begin(), [](unsigned char c){ return std::tolower(c);} );
  return my_str;
}
      
};

};


#endif // _GUNDAM_UTIL_STRING_H