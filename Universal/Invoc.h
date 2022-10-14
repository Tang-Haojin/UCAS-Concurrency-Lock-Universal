#ifndef __INVOC_H__
#define __INVOC_H__

#include <string>

class Invoc {
public:
  std::string *name;
  Invoc(const char init[]) : name(new std::string(init)){};
  Invoc() : name(){};
};

#endif
