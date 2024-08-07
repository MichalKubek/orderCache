#pragma once
#include <string>

#if defined(DEBUG)
#define DEBUG(format, ...) print_debug_msg (format, ##__VA_ARGS__)
#else
#define DEBUG(format, ...)
#endif

void print_debug_msg (const char *msg, ...);

class ScopePrint
{
private:
  std::string m_str;

public:
  ScopePrint (const std::string&, const std::string&);
  ~ScopePrint ();
};
