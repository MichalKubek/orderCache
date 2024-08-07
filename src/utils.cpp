#include "utils.h"

#include <cstdarg>
#include <sstream>
#include <thread>

void
print_debug_msg (const char *msg, ...)
{
  std::stringstream ss;
  ss << "[ " << std::this_thread::get_id () << " ] "
     << " " << msg << std::endl;
  va_list args;
  va_start (args, msg);
  vprintf (ss.str ().c_str (), args);
  va_end (args);
}


ScopePrint::ScopePrint (const std::string &enter, const std::string &exit)
{
  m_str = enter;
  DEBUG ("%s", m_str.c_str ());
}

ScopePrint::~ScopePrint ()
{
  DEBUG ("%s", m_str.c_str ());
}
