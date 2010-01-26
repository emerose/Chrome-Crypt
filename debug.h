/*
 *  debug.h
 *  ChromeCrypt
 *
 *  Created by Sam Quigley on 1/23/10.
 *  Copyright 2010 emerose.com. All rights reserved.
 *
 */

#include <assert.h>

enum DEBUG_LEVELS {
  DEBUG_LEVEL_TOOMUCH = 0,
  DEBUG_LEVEL_TRACE,
  DEBUG_LEVEL_INFO,
  DEBUG_LEVEL_WARN,
  DEBUG_LEVEL_ERR,
  DEBUG_LEVEL_CRIT,
  DEBUG_LEVEL_NONE,
};

#ifndef DEBUG_LOGFILE
#define DEBUG_LOGFILE stderr
#endif

#ifndef DEBUG_LOG_LEVEL
#define DEBUG_LOG_LEVEL DEBUG_LEVEL_INFO
#endif

#ifndef NDEBUG
  #define DEBUG_LOG_FMT(lvl, msg, ...) do {\
    if (lvl > DEBUG_LOG_LEVEL) {\
      debug_write_message(__FILE__, __LINE__, __func__, #lvl, msg, ##__VA_ARGS__);\
    }\
  } while (0)
#else
  #define DEBUG_LOG_FMT(lvl, msg, ...)
#endif

#define DEBUG_LOG(lvl, msg) DEBUG_LOG_FMT(lvl, msg "%s", "")
#define DEBUG_CRIT(msg)     DEBUG_LOG(DEBUG_LEVEL_CRIT, msg)
#define DEBUG_ERR(msg)      DEBUG_LOG(DEBUG_LEVEL_ERR, msg)
#define DEBUG_WARN(msg)     DEBUG_LOG(DEBUG_LEVEL_WARN, msg)
#define DEBUG_INFO(msg)     DEBUG_LOG(DEBUG_LEVEL_INFO, msg)
#define DEBUG_TRACE(msg)    DEBUG_LOG(DEBUG_LEVEL_TRACE, msg)
#define DEBUG_TOOMUCH(msg)  DEBUG_LOG(DEBUG_LEVEL_TOOMUCH, msg)

static FILE *debug_fileptr = NULL;

void debug_write_message(const char *file, int line, const char *func, const char *level, const char *msg, ...)
{
  va_list argp;
  const char *level_string = level + 12; // skip past "DEBUG_LEVEL_"
  char *message = NULL;

  if (debug_fileptr == NULL) {
      // first time DEBUG_LOG has been called
    debug_fileptr = fopen(DEBUG_LOGFILE, "a+");
    assert(debug_fileptr);
  }

  va_start(argp, msg);
  vasprintf(&message, msg, argp);
  assert(message);
  va_end(argp);
  
  fprintf(debug_fileptr, "[%-8.8s] %-70.70s[%s:%d:%s()]\n", level_string, message, file, line, func);
  fflush(debug_fileptr);
  free(message);
}