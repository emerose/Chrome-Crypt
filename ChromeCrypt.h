/*
 * ChromeCrypt.h
 *
 * (c) 2010 Sam Quigley <quigley@emerose.com>
 * originally based on BasicPlugin by the Mozilla Corporation, but
 * changed a lot since then.
 */

#include "include/plugin/npapi.h"
#include "include/plugin/npruntime.h"
#include "include/plugin/npfunctions.h"

#define DEBUG_LOGFILE "/tmp/cc.log"
#define DEBUG_LOG_LEVEL DEBUG_LEVEL_INFO
#include "debug.h"

#define DEBUG_NPIDENTIFIER(level, name)\
  if (level >= DEBUG_LOG_LEVEL) {\
    if (browser->identifierisstring(name)) {\
      char *name_string = browser->utf8fromidentifier(name);\
      DEBUG_LOG_FMT(level, "'" #name "' is: '%s'", name_string);\
      browser->memfree(name_string);\
    } else {\
      DEBUG_INFO("'" #name "' is not a string");\
    }\
  }

#define DEBUG_NPVARIANT(level, var)\
  if (level >= DEBUG_LOG_LEVEL) {\
    if (NPVARIANT_IS_STRING(var)) {\
      DEBUG_LOG_FMT(level, "'%s' is '%s'", #var, NPVARIANT_TO_STRING(var));\
    } else if (NPVARIANT_IS_VOID(var)) {\
      DEBUG_LOG_FMT(level, "'%s' is void", #var);\
    } else if (NPVARIANT_IS_NULL(var)) {\
      DEBUG_LOG_FMT(level, "'%s' is NULL", #var);\
    } else if (NPVARIANT_IS_INT32(var)) {\
      DEBUG_LOG_FMT(level, "'%s' is %d", #var, NPVARIANT_TO_INT32(var));\
    } else if (NPVARIANT_IS_DOUBLE(var)) {\
      DEBUG_LOG_FMT(level, "'%s' is %f", #var, NPVARIANT_TO_DOUBLE(var));\
    } else if (NPVARIANT_IS_BOOLEAN(var)) {\
      if (NPVARIANT_TO_BOOLEAN(var)) {\
        DEBUG_LOG_FMT(level, "'%s' is TRUE", #var);\
      } else {\
        DEBUG_LOG_FMT(level, "'%s' is FALSE", #var);\
      }\
    } else if (NPVARIANT_IS_OBJECT(var)) {\
      DEBUG_LOG_FMT(level, "'%s' is an object", #var);\
    } else {\
      DEBUG_LOG_FMT(level, "'%s' has unsupported type", #var);\
    }\
  }


#define PLUGIN_NAME "ChromeCrypt"
#define PLUGIN_DESC "Encryption thingy for Chrome"
#define MIME_TYPES_DESCRIPTION "application/x-chromecrypt:ccrypt:Cypto Plugin for Chrome"

#ifndef HIBYTE
#define HIBYTE(x) ((((uint32)(x)) & 0xff00) >> 8)
#endif

NPError NP_GetEntryPoints(NPPluginFuncs *pluginFuncs);
NPError NP_Initialize(NPNetscapeFuncs *browserFuncs);

  // declarations for NPP_* are in include/plugin/npapi.h

NPObject *newScriptObject(NPP instance);
bool scriptHasMethod(NPObject *npobj, NPIdentifier name);
bool scriptInvokeMethod(NPObject *npobj,
                        NPIdentifier name,
                        const NPVariant *args,
                        uint32_t argCount,
                        NPVariant *result);
bool scriptInvokeDefaultMethod(NPObject *npobj,
                               const NPVariant *args,
                               uint32_t argCount,
                               NPVariant *result);
bool scriptHasProperty(NPObject *npobj, NPIdentifier name);
bool scriptGetProperty(NPObject *npobj, NPIdentifier name, NPVariant *result);
bool scriptSetProperty(NPObject *npobj, NPIdentifier name, const NPVariant *value);
bool scriptRemoveProperty(NPObject *npobj, NPIdentifier name);
