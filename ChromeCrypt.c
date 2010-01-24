/* 
 * ChromeCrypt.c
 *
 * (c) 2010 Sam Quigley <quigley@emerose.com>
 * originally based on BasicPlugin by the Mozilla Corporation, but
 * changed a lot since then.
 */

#include "ChromeCrypt.h"


static NPNetscapeFuncs *browser; // we store the browser function table here for future use

void LOG_DEBUG(const char *msg)
{
  FILE *file = fopen("/tmp/cc.log", "a+");
	if (file) {
		fprintf(file, "%s\n", msg);
		fclose(file);
  } else {
    abort();
  }
}

/**
 * Called once (before NP_Initialize?) in order to set up pointers required for
 * most other function calls.
 * NB: This function is almost entirely undocumented; this is the result of me
 * sifting through Other People's Code.  No guarantees.
 */
NPError NP_GetEntryPoints(NPPluginFuncs* pluginFuncs)
{
  LOG_DEBUG("NP_GetEntryPoints");
 
  if (pluginFuncs== NULL) {
    LOG_DEBUG("pluginFuncs is NULL!");
    return NPERR_INVALID_FUNCTABLE_ERROR;
  }
  
  if (pluginFuncs->size < sizeof(NPPluginFuncs)) {
    LOG_DEBUG("pluginFuncs has wrong size!");
    return NPERR_INVALID_FUNCTABLE_ERROR;
  }
  
  
  pluginFuncs->version  = (NP_VERSION_MAJOR << 8) | NP_VERSION_MINOR;
  pluginFuncs->newp     = NPP_New;
  pluginFuncs->destroy  = NPP_Destroy;
  
  return NPERR_NO_ERROR;
}

/** 
 * Called only once, when the plugin is loaded.
 */
NPError NP_Initialize(NPNetscapeFuncs* browserFuncs)
{  
  LOG_DEBUG("NP_Initialize");

  if (((browserFuncs->version) >> 8) > NP_VERSION_MAJOR) {
    LOG_DEBUG("Wrong API Version!");
    return NPERR_INCOMPATIBLE_VERSION_ERROR;    
  }

  if (browserFuncs->size < sizeof(NPNetscapeFuncs)) {
    LOG_DEBUG("browserFuncs has wrong size!");
    return NPERR_INVALID_FUNCTABLE_ERROR;    
  }
  
  browser = browserFuncs;
  return NPERR_NO_ERROR;
}

/**
 * Not really sure when this is called — or even *if* it's called — but its
 * existence seems to be very important.
 */
char* NP_GetMIMEDescription(void)
{
  LOG_DEBUG("NP_GetMIMEDescription");
  
  return(MIME_TYPES_DESCRIPTION);
}

/**
 * Called to create a new instance of the plugin
 */
NPError NPP_New(NPMIMEType pluginType, // ptr to MIME type for plugin instance
                NPP instance,          // the new instance
                uint16_t mode,         // does plugin share window w/ other content?
                int16_t argc,          // # of attributes in EMBED tag
                char* argn[],          // EMBED tag attribute names
                char* argv[],          // EMBED tag attribute values
                NPSavedData* saved)    // prev saved instance data (?)
{
  LOG_DEBUG("NPP_New");
  
  return NPERR_NO_ERROR;
}

/**
 * Called by the browser to retrieve instance variables from the plugin 
 */
NPError NPP_GetValue(NPP instance,         // the instance
                     NPPVariable variable, // the variable to retrieve
                     void *value)          // where to put the value (?)
{
  LOG_DEBUG("NPP_GetValue");
  
  switch(variable) {
    case NPPVpluginNameString:
      *((char **)value) = PLUGIN_NAME;
      return NPERR_NO_ERROR;

    case NPPVpluginDescriptionString:
      *((char **)value) = PLUGIN_DESC;
      return NPERR_NO_ERROR;

    case NPPVpluginScriptableNPObject:// Scriptable plugin interface (for accessing from javascript)
      *((NPObject **)value) = newScriptObject(instance);
      return NPERR_NO_ERROR;

    case NPPVpluginWindowBool:
      *((bool **)value) = FALSE;
      return NPERR_NO_ERROR;

    default:
      return NPERR_GENERIC_ERROR;
  }
}

/**
 * Called to destroy an instance of the plugin
 */
NPError NPP_Destroy(NPP instance, NPSavedData** save)
{
  LOG_DEBUG("NPP_Destroy");
  
  return NPERR_NO_ERROR;
}

/**
 * Called once, when the plugin is unloaded
 */
void NP_Shutdown(void)
{
  LOG_DEBUG("NP_Shutdown");
  
  return;
}

/**********************/

/**
 * creates a new NPObject for use in scripting
 */
NPObject *newScriptObject(NPP instance)
{
  LOG_DEBUG("newScriptObject");
  
    // first, we allocate and zero an NPClass struct
  NPClass *class = malloc(sizeof(NPClass));
  memset(class, 0, sizeof(NPClass));
  
    // now, we fill it in with the right function pointers
  class->structVersion  = NP_CLASS_STRUCT_VERSION;
  class->hasMethod      = scriptHasMethod;
  class->invoke         = scriptInvokeMethod;
  class->invokeDefault  = scriptInvokeDefaultMethod;
  class->hasProperty    = scriptHasProperty;
  class->getProperty    = scriptGetProperty;
  class->setProperty    = scriptSetProperty;
  class->removeProperty = scriptRemoveProperty;
  
  return browser->createobject(instance, class);
}

/**
 * returns true if the object +npobj+ has the method +name+
 */
bool scriptHasMethod(NPObject *npobj, NPIdentifier name)
{
  LOG_DEBUG("scriptHasMethod");
  
  return true; // pretend we have all methods
}

/**
 * invoke method +name+ on +npobh+ with +argCount+ arguments specified
 * in array +args+; return the result in +result+
 */
bool scriptInvokeMethod(NPObject *npobj,
                        NPIdentifier name,
                        const NPVariant *args,
                        uint32_t argCount,
                        NPVariant *result)
{
  LOG_DEBUG("scriptInvokeMethod");
  
  STRINGZ_TO_NPVARIANT("HELLO", *result);
  return true;
}

bool scriptInvokeDefaultMethod(NPObject *npobj,
                               const NPVariant *args,
                               uint32_t argCount,
                               NPVariant *result)
{
  LOG_DEBUG("scriptInvokeDefaultMethod");
  
  STRINGZ_TO_NPVARIANT("HELLO", *result);
  return true;
}

bool scriptHasProperty(NPObject *npobj, NPIdentifier name)
{
  LOG_DEBUG("scriptHasProperty");
  
  return true;
}

bool scriptGetProperty(NPObject *npobj, NPIdentifier name, NPVariant *result)
{
  LOG_DEBUG("scriptGetProperty");
  
  STRINGZ_TO_NPVARIANT("HELLO", *result);
  return true;
}

bool scriptSetProperty(NPObject *npobj, NPIdentifier name, const NPVariant *value)
{
  LOG_DEBUG("scriptSetProperty");
  
  return true;
}

bool scriptRemoveProperty(NPObject *npobj, NPIdentifier name)
{
  LOG_DEBUG("scriptRemoveProperty");
  
  return true;
}

