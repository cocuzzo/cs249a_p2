#ifndef SHIPPING_TYPES_H
#define SHIPPING_TYPES_H

#include <string>
#include <sstream>
#include <cfloat>
#include "Nominal.h"
#include "Types.h"

namespace Shipping {

// forward declarations (so Activity doesn't choke)
// class Engine;
// class Location;
// class Segment;

#define MAX_BUF 64
#define MAX_COST DBL_MAX
#define MAX_MILE DBL_MAX
#define MAX_SPEED DBL_MAX
#define MAX_TIME DBL_MAX
#define MAX_CAPACITY DBL_MAX

class Exception {
public:
  Exception(char const * str) : what_(str) { }
  Exception(std::string str) : what_(str) { }
  std::string what() const { return what_; }
  virtual ~Exception() { }
protected:

private:
  std::string what_;
};

// ordinal types
class Mile : public Ordinal<Mile, double> {
public:
  Mile(double num = 0.0) : Ordinal<Mile, double>(num) {
    if (num < 0.0) {
      std::ostringstream err;
      err << "Mile value error: " << num;
      throw Exception( err.str() );
    }
  }
  std::string toString() { 
    char buf[MAX_BUF];
    std::snprintf(buf, MAX_BUF-1, "%.2f", value_);
    return std::string(buf);
  }
  static Mile Max() { return Mile(MAX_MILE);  }
};

class Difficulty : public Ordinal<Difficulty, double> {
public:
  Difficulty(double num = 1.0) : Ordinal<Difficulty, double>(num) {
    if (num < 1.0 || num > 5.0) {
      std::ostringstream err;
      err << "Difficulty value error: " << num;
      throw Exception( err.str() );
    }
  }
  std::string toString() { 
    char buf[MAX_BUF];
    std::snprintf(buf, MAX_BUF-1, "%.2f", value_);
    return std::string(buf);
  }
};

class Speed : public Ordinal<Speed, double> {
public:
  Speed(double num = 1.0) : Ordinal<Speed, double>(num) {
    if (num <= 0.0) {
      std::ostringstream err;
      err << "Speed value error: " << num;
      throw Exception( err.str() );
    }
  }
  std::string toString() { 
    char buf[MAX_BUF];
    std::snprintf(buf, MAX_BUF-1, "%.2f", value_);
    return std::string(buf);
  }
  static Speed Max() { return Speed(MAX_SPEED); }
};

class Cost : public Ordinal<Cost, double> {
public:
  Cost(double num = 0.0) : Ordinal<Cost, double>(num) {
    if (num < 0.0) {
      std::ostringstream err;
      err << "Cost value error: " << num;
      throw Exception( err.str() );
    }
  }
  std::string toString() { 
    char buf[MAX_BUF] = {0};
    std::snprintf(buf, MAX_BUF-1, "%.2f", value_);
    return std::string(buf);
  }
  static Cost Max() { return Cost(MAX_COST);  }
};

class Capacity : public Ordinal<Capacity, int> {
public:
  Capacity(int num = 0) : Ordinal<Capacity, int>(num) {
    if (num < 0) {
      std::ostringstream err;
      err << "Capacity value error: " << num;
      throw Exception( err.str() );
    }
  }
  std::string toString() { return std::to_string(value_); }
};

class Time : public Ordinal<Time, double> {
public:
  Time(double num = 0.0) : Ordinal<Time, double>(num) {
    if (num < 0.0) {
      std::ostringstream err;
      err << "Time value error: " << num;
      throw Exception( err.str() );
    }
  }
  std::string toString() { 
    char buf[MAX_BUF];
    std::snprintf(buf, MAX_BUF-1, "%.2f", value_);
    return std::string(buf);
  }
  static Time Max() { return Time(MAX_TIME);  }
};

} /* end shipping namespace */

#endif