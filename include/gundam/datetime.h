#ifndef _DATETIME_H
#define _DATETIME_H
#include <cstdio>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>
//类型说明：绝对时间
//格式:年-月-日 时:分:秒
class DateTime {
 private:
  time_t t_;

 public:
  DateTime() {}
  DateTime(const char* str) {
    int year, month, day, hour, minute, second;
    sscanf(str, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute,
           &second);
    tm tm_;
    tm_.tm_year = year - 1900;
    tm_.tm_mon = month - 1;
    tm_.tm_mday = day;
    tm_.tm_hour = hour;
    tm_.tm_min = minute;
    tm_.tm_sec = second;
    tm_.tm_isdst = 0;
    this->t_ = mktime(&tm_);
  }
  std::string to_string() const {
    char str[200];
    strftime(str, 64, "%Y-%m-%d %H:%M:%S", localtime(&this->t_));
    return std::string{str};
  }
  friend std::ostream& operator<<(std::ostream& out, const DateTime b) {
    out << b.to_string();
    return out;
  }
  bool operator==(const DateTime b) const { return this->t_ == b.t_; }
  bool operator!=(const DateTime b) const { return this->t_ != b.t_; }
};
#endif