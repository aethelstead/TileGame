#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>

/*
  "\x1b[30m", // TEXT_COLOR_BLACK
  "\x1b[31m", // TEXT_COLOR_RED
  "\x1b[32m", // TEXT_COLOR_GREEN
  "\x1b[33m", // TEXT_COLOR_YELLOW
  "\x1b[34m", // TEXT_COLOR_BLUE
  "\x1b[35m", // TEXT_COLOR_MAGENTA
  "\x1b[36m", // TEXT_COLOR_CYAN
  "\x1b[37m", // TEXT_COLOR_WHITE
  "\x1b[90m", // TEXT_COLOR_BRIGHT_BLACK
  "\x1b[91m", // TEXT_COLOR_BRIGHT_RED
  "\x1b[92m", // TEXT_COLOR_BRIGHT_GREEN
  "\x1b[93m", // TEXT_COLOR_BRIGHT_YELLOW
  "\x1b[94m", // TEXT_COLOR_BRIGHT_BLUE
  "\x1b[95m", // TEXT_COLOR_BRIGHT_MAGENTA
  "\x1b[96m", // TEXT_COLOR_BRIGHT_CYAN
  "\x1b[97m", // TEXT_COLOR_BRIGHT_WHITE
*/

#define DEBUG

#define LOGLEVEL_NONE   0
#define LOGLEVEL_ERROR  1
#define LOGLEVEL_WARN   2
#define LOGLEVEL_INFO   3
#define LOGLEVEL_DEBUG  4
#define LOGLEVEL_TRACE  5

#define LOGLEVEL_MAX LOGLEVEL_TRACE

#define LOGERROR(msg) if (LOGLEVEL_MAX >= LOGLEVEL_ERROR) std::cout << "\x1b[91m [ ERR ]  " << msg << std::endl;
#define LOGWARN(msg)  if (LOGLEVEL_MAX >= LOGLEVEL_WARN)  std::cout << "\x1b[33m [ WRN ]  " << msg << std::endl;
#define LOGINFO(msg)  if (LOGLEVEL_MAX >= LOGLEVEL_INFO)  std::cout << "\x1b[97m [ INF ]  " << msg << std::endl;
#define LOGDEBUG(msg) if (LOGLEVEL_MAX >= LOGLEVEL_DEBUG) std::cout << "\x1b[96m [ DBG ]  " << msg << std::endl;
#define LOGTRACE(msg) if (LOGLEVEL_MAX >= LOGLEVEL_TRACE) std::cout << "\x1b[93m [ TRA ]  " << msg << std::endl;

#endif
