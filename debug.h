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
  DEBUG_TOOMUCH = 0,
  DEBUG_TRACE,
  DEBUG_INFO,
  DEBUG_WARN,
  DEBUG_ERR,
  DEBUG_CRIT,
  DEBUG_NONE,
};

#ifndef DEBUG_LOGFILE
#define DEBUG_LOGFILE stderr
#endif

#ifndef DEBUG_LOG_LEVEL
#define DEBUG_LOG_LEVEL INFO
#endif

#ifndef NDEBUG
  // hugely inefficient — but that's not really the point.
#define DEBUG_LOG(lvl, msg) do {\
  if (lvl > DEBUG_LOG_LEVEL) {\
    debug_write_message(__FILE__, __LINE__, __func__, msg);\
  }\
} while (0)
#else
#define DEBUG_LOG(lvl, msg) 
#endif

static FILE *debug_fileptr = NULL;

void debug_write_message(const char *file, int line, const char *func, const char *msg)
{
  if (debug_fileptr == NULL) {
      // first time DEBUG_LOG has been called
    debug_fileptr = fopen(DEBUG_LOGFILE, "a+");
    assert(debug_fileptr);
  }

  fprintf(debug_fileptr, "%s:%d:%s():\t%s\n", file, line, func, msg);
  fflush(debug_fileptr);
}