/* 
 * ChromeCrypt.c
 *
 * (c) 2010 Sam Quigley <quigley@emerose.com>
 * originally based on BasicPlugin by the Mozilla Corporation, but
 * changed a lot since then.
 */

#include "ChromeCrypt.h"

static NPNetscapeFuncs *browser; // we store the browser function table here for future use

/**
 * Called once (before NP_Initialize?) in order to set up pointers required for
 * most other function calls.
 * NB: This function is almost entirely undocumented; this is the result of me
 * sifting through Other People's Code.  No guarantees.
 */
NPError NP_GetEntryPoints(NPPluginFuncs* pluginFuncs)
{
  DEBUG_LOG(DEBUG_TRACE, "NP_GetEntryPoints");
 
  if (pluginFuncs== NULL) {
    DEBUG_LOG(DEBUG_CRIT, "pluginFuncs is NULL!");
    return NPERR_INVALID_FUNCTABLE_ERROR;
  }
  
  if (pluginFuncs->size < sizeof(NPPluginFuncs)) {
    DEBUG_LOG(DEBUG_CRIT, "pluginFuncs has wrong size!");
    return NPERR_INVALID_FUNCTABLE_ERROR;
  }
  
  pluginFuncs->version       = (NP_VERSION_MAJOR << 8) | NP_VERSION_MINOR;
  pluginFuncs->newp          = NPP_New;
  pluginFuncs->destroy       = NPP_Destroy;
  pluginFuncs->setwindow     = NPP_SetWindow;
  pluginFuncs->newstream     = NPP_NewStream;
  pluginFuncs->destroystream = NPP_DestroyStream;
  pluginFuncs->asfile        = NPP_StreamAsFile;
  pluginFuncs->writeready    = NPP_WriteReady;
  pluginFuncs->write         = NPP_Write;
  pluginFuncs->print         = NPP_Print;
  pluginFuncs->event         = NPP_HandleEvent;
  pluginFuncs->urlnotify     = NPP_URLNotify;
  pluginFuncs->getvalue      = NPP_GetValue;
  pluginFuncs->setvalue      = NPP_SetValue;
  pluginFuncs->javaClass     = NULL;
  
  return NPERR_NO_ERROR;
}

/** 
 * Called only once, when the plugin is loaded.
 */
NPError NP_Initialize(NPNetscapeFuncs* browserFuncs)
{  
  DEBUG_LOG(DEBUG_TRACE, "NP_Initialize");

  if (HIBYTE(browserFuncs->version) > NP_VERSION_MAJOR) {
    DEBUG_LOG(DEBUG_CRIT, "Wrong API Version!");
    return NPERR_INCOMPATIBLE_VERSION_ERROR;    
  }

  if (browserFuncs->size < sizeof(NPNetscapeFuncs)) {
    DEBUG_LOG(DEBUG_CRIT, "browserFuncs has wrong size!");
    return NPERR_INVALID_FUNCTABLE_ERROR;    
  }
  
  browser = browserFuncs;
  return NPERR_NO_ERROR;
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
  DEBUG_LOG(DEBUG_TRACE, "NPP_New");
  
  return NPERR_NO_ERROR;
}

/**
 * Called by the browser to retrieve instance variables from the plugin 
 */
NPError NPP_GetValue(NPP instance,
                     NPPVariable variable,
                     void *value)
{
  DEBUG_LOG(DEBUG_TRACE, "NPP_GetValue");
  
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
 * Called by the browser to retrieve instance variables from the plugin 
 */
NPError NPP_SetValue(NPP instance,         // the instance
                     NPNVariable variable, // the variable to retrieve
                     void *value)          // where to put the value (?)
{
  DEBUG_LOG(DEBUG_TRACE, "NPP_SetValue");

  return NPERR_GENERIC_ERROR;
}


/**
 * Called to destroy an instance of the plugin
 */
NPError NPP_Destroy(NPP instance, NPSavedData** save)
{
  DEBUG_LOG(DEBUG_TRACE, "NPP_Destroy");
  
  return NPERR_NO_ERROR;
}

/**
 * Called once, when the plugin is unloaded
 */
void NP_Shutdown(void)
{
  DEBUG_LOG(DEBUG_TRACE, "NP_Shutdown");
  
  return;
}

/**
 * Not really sure when this is called — or even *if* it's called — but its
 * existence seems to be very important.
 */
char* NP_GetMIMEDescription(void)
{
  DEBUG_LOG(DEBUG_TRACE, "NP_GetMIMEDescription");
  
  return(MIME_TYPES_DESCRIPTION);
}

/**
 * Called when a window is created, moved, sized, or destroyed.
 */
NPError NPP_SetWindow(NPP instance, NPWindow *window)
{
  DEBUG_LOG(DEBUG_TRACE, "NPP_SetWindow");
  
  return NPERR_NO_ERROR;
}

/**
 * Called to notify plugin of a new data stream
 */
NPError NPP_NewStream(NPP        instance, 
                      NPMIMEType type,
                      NPStream*  stream,
                      NPBool     seekable,
                      uint16*    stype)
{
  DEBUG_LOG(DEBUG_TRACE, "NPP_NewStream");
  
  return NPERR_NO_ERROR;  
}

/**
 * Called to notify plugin when a stream is about to be closed
 */
NPError NPP_DestroyStream(NPP       instance, 
                          NPStream* stream, 
                          NPReason  reason)
{
  DEBUG_LOG(DEBUG_TRACE, "NPP_DestroyStream");
  
  return NPERR_NO_ERROR;
}

/**
 * "Provides a local file name for the data from a stream," whatever that means
 */
void NPP_StreamAsFile(NPP         instance,
                      NPStream*   stream,
                      const char* fname)
{
  DEBUG_LOG(DEBUG_TRACE, "NPP_StreamAsFile");
  
  return;
}

/**
 * Returns max number of bytes that plugin can consume
 */
int32_t NPP_WriteReady(NPP instance, NPStream* stream)
{
  DEBUG_LOG(DEBUG_TRACE, "NPP_WriteReady");
  
  return 0;
}

/**
 * Delivers data to a plugin
 */
int32_t NPP_Write(NPP instance, 
                NPStream* stream,
                int32_t offset, 
                int32_t len, 
                void* buf)
{
  DEBUG_LOG(DEBUG_TRACE, "NPP_Write");
  
  return len;
}

/**
 * Asks plugin to print itself 
 */
void NPP_Print(NPP instance, NPPrint* PrintInfo)
{
  DEBUG_LOG(DEBUG_TRACE, "NPP_Print");
  
  return;
}

/**
 * Delivers window event to plugin
 */
int16_t NPP_HandleEvent(NPP instance, void* event)
{
  DEBUG_LOG(DEBUG_TRACE, "NPP_HandleEvent");
  
  return FALSE;
}

/**
 * Notifies plugin that URL request has completed
 */
void NPP_URLNotify(NPP         instance, 
                   const char* url,
                   NPReason    reason, 
                   void*       notifyData)
{
  DEBUG_LOG(DEBUG_TRACE, "NPP_URLNotify");
  
  return;
}


/**********************/

/**
 * creates a new NPObject for use in scripting
 */
NPObject *newScriptObject(NPP instance)
{
  DEBUG_LOG(DEBUG_TRACE, "newScriptObject");
  
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
  DEBUG_LOG(DEBUG_TRACE, "scriptHasMethod");
  
  
  
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
  DEBUG_LOG(DEBUG_TRACE, "scriptInvokeMethod");
  
  STRINGZ_TO_NPVARIANT("HELLO", *result);
  return true;
}

bool scriptInvokeDefaultMethod(NPObject *npobj,
                               const NPVariant *args,
                               uint32_t argCount,
                               NPVariant *result)
{
  DEBUG_LOG(DEBUG_TRACE, "scriptInvokeDefaultMethod");
  
  STRINGZ_TO_NPVARIANT("HELLO", *result);
  return true;
}

bool scriptHasProperty(NPObject *npobj, NPIdentifier name)
{
  DEBUG_LOG(DEBUG_TRACE, "scriptHasProperty");
  if (browser->identifierisstring(name)) {
    char *name_string = browser->utf8fromidentifier(name);
    DEBUG_LOG(DEBUG_INFO, "name is:");
    DEBUG_LOG(DEBUG_INFO, name_string);
    browser->memfree(name_string);
  }
  else {
    DEBUG_LOG(DEBUG_INFO, "name is not a string");
  }
  
  return true;
}

bool scriptGetProperty(NPObject *npobj, NPIdentifier name, NPVariant *result)
{
  DEBUG_LOG(DEBUG_TRACE, "scriptGetProperty");
  if (browser->identifierisstring(name)) {
    char *name_string = browser->utf8fromidentifier(name);
    DEBUG_LOG(DEBUG_INFO, "name is:");
    DEBUG_LOG(DEBUG_INFO, name_string);
    browser->memfree(name_string);
  }
  else {
    DEBUG_LOG(DEBUG_INFO, "name is not a string");
  }  
  
  STRINGZ_TO_NPVARIANT("HELLO", *result);
  return true;
}

bool scriptSetProperty(NPObject *npobj, NPIdentifier name, const NPVariant *value)
{
  DEBUG_LOG(DEBUG_TRACE, "scriptSetProperty");
  
  return true;
}

bool scriptRemoveProperty(NPObject *npobj, NPIdentifier name)
{
  DEBUG_LOG(DEBUG_TRACE, "scriptRemoveProperty");
  
  return true;
}

