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
  DEBUG_TRACE("NP_GetEntryPoints");
 
  if (pluginFuncs == NULL) {
    DEBUG_CRIT("pluginFuncs is NULL!");
    return NPERR_INVALID_FUNCTABLE_ERROR;
  }

    // NB: according to the google gears code, Webkit on OSX will pass a 0 for 
    // size here, apparently expecting us to treat it as an output parameter.
    // I haven't seen that behavior (yet)...
  if (pluginFuncs->size < sizeof(NPPluginFuncs)) {
    DEBUG_CRIT("pluginFuncs has wrong size!");
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
  DEBUG_TRACE("NP_Initialize");

  if (HIBYTE(browserFuncs->version) > NP_VERSION_MAJOR) {
    DEBUG_CRIT("Wrong API Version!");
    return NPERR_INCOMPATIBLE_VERSION_ERROR;    
  }

  if (browserFuncs->size < sizeof(NPNetscapeFuncs)) {
    DEBUG_CRIT("browserFuncs has wrong size!");
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
  DEBUG_TRACE("NPP_New");
  if (instance == NULL) return NPERR_INVALID_INSTANCE_ERROR;
  
    // make this a windowless plugin
  return browser->setvalue(instance, NPPVpluginWindowBool, NULL);
}

/**
 * Called by the browser to retrieve instance variables from the plugin 
 */
NPError NPP_GetValue(NPP instance,
                     NPPVariable variable,
                     void *value)
{
  NPObject *scriptObj;

  DEBUG_TRACE("NPP_GetValue");
  if (instance == NULL) return NPERR_INVALID_INSTANCE_ERROR;
  
  switch(variable) {
    case NPPVpluginNameString:
      *((char **)value) = PLUGIN_NAME;
      return NPERR_NO_ERROR;

    case NPPVpluginDescriptionString:
      *((char **)value) = PLUGIN_DESC;
      return NPERR_NO_ERROR;

    case NPPVpluginScriptableNPObject:// Scriptable plugin interface (for accessing from javascript)
      scriptObj = newScriptObject(instance);
      *((NPObject **)value) = scriptObj;
      browser->retainobject(scriptObj);  // caller expects it retained.
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
  DEBUG_TRACE("NPP_SetValue");
  if (instance == NULL) return NPERR_INVALID_INSTANCE_ERROR;

  return NPERR_GENERIC_ERROR;
}


/**
 * Called to destroy an instance of the plugin
 */
NPError NPP_Destroy(NPP instance, NPSavedData** save)
{
  DEBUG_TRACE("NPP_Destroy");
  if (instance == NULL) return NPERR_INVALID_INSTANCE_ERROR;
  
  return NPERR_NO_ERROR;
}

/**
 * Called once, when the plugin is unloaded
 */
void NP_Shutdown(void)
{
  DEBUG_TRACE("NP_Shutdown");
  
  return;
}

/**
 * Not really sure when this is called — or even *if* it's called — but its
 * existence seems to be very important.
 */
char* NP_GetMIMEDescription(void)
{
  DEBUG_TRACE("NP_GetMIMEDescription");
  
  return(MIME_TYPES_DESCRIPTION);
}

/**
 * Called when a window is created, moved, sized, or destroyed.
 */
NPError NPP_SetWindow(NPP instance, NPWindow *window)
{
  DEBUG_TRACE("NPP_SetWindow");
  if (instance == NULL) return NPERR_INVALID_INSTANCE_ERROR;
  
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
  DEBUG_TRACE("NPP_NewStream");
  if (instance == NULL) return NPERR_INVALID_INSTANCE_ERROR;
  
  return NPERR_NO_ERROR;  
}

/**
 * Called to notify plugin when a stream is about to be closed
 */
NPError NPP_DestroyStream(NPP       instance, 
                          NPStream* stream, 
                          NPReason  reason)
{
  DEBUG_TRACE("NPP_DestroyStream");
  if (instance == NULL) return NPERR_INVALID_INSTANCE_ERROR;
  
  return NPERR_NO_ERROR;
}

/**
 * "Provides a local file name for the data from a stream," whatever that means
 */
void NPP_StreamAsFile(NPP         instance,
                      NPStream*   stream,
                      const char* fname)
{
  DEBUG_TRACE("NPP_StreamAsFile");
  if (instance == NULL) return;
  
  return;
}

/**
 * Returns max number of bytes that plugin can consume
 */
int32_t NPP_WriteReady(NPP instance, NPStream* stream)
{
  DEBUG_TRACE("NPP_WriteReady");
  if (instance == NULL) return NPERR_INVALID_INSTANCE_ERROR;
  
    // report that we're ready to consume quite a few bytes
  return 0x0fffffff;
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
  DEBUG_TRACE("NPP_Write");
  if (instance == NULL) return NPERR_INVALID_INSTANCE_ERROR;
  
  return len;
}

/**
 * Asks plugin to print itself 
 */
void NPP_Print(NPP instance, NPPrint* PrintInfo)
{
  DEBUG_TRACE("NPP_Print");
  if (instance == NULL) return;
  
  return;
}

/**
 * Delivers window event to plugin
 */
int16_t NPP_HandleEvent(NPP instance, void* event)
{
  DEBUG_TRACE("NPP_HandleEvent");
  if (instance == NULL) return NPERR_INVALID_INSTANCE_ERROR;
  
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
  DEBUG_TRACE("NPP_URLNotify");
  if (instance == NULL) return;
  
  return;
}


/**********************/

/**
 * creates a new NPObject for use in scripting
 */
NPObject *newScriptObject(NPP instance)
{
  DEBUG_TRACE("newScriptObject");
  assert(instance);
  
    // first, we allocate and zero an NPClass struct
  NPClass *class = malloc(sizeof(NPClass));
  memset(class, 0, sizeof(NPClass));
  
    // now, we fill it in with the right function pointers
  class->structVersion  = NP_CLASS_STRUCT_VERSION;
    // use malloc/free
  class->allocate       = NULL;
  class->deallocate     = NULL;
  class->invalidate     = NULL;
    // scripty bits:
  class->hasMethod      = scriptHasMethod;
  class->invoke         = scriptInvokeMethod;
  class->invokeDefault  = scriptInvokeDefaultMethod;
  class->invokeDefault  = NULL;
  class->hasProperty    = scriptHasProperty;
  class->getProperty    = scriptGetProperty;
  class->setProperty    = scriptSetProperty;
  class->removeProperty = scriptRemoveProperty;
  class->removeProperty = NULL;
  
  return browser->createobject(instance, class);
}

/**
 * returns true if the object +npobj+ has the method +name+
 */
bool scriptHasMethod(NPObject *npobj, NPIdentifier name)
{
  DEBUG_TRACE("scriptHasMethod");
  DEBUG_NPIDENTIFIER(DEBUG_LEVEL_INFO, name);  
  
  return true; // pretend we have all methods
}

/**
 * invoke method +name+ on +npobj+ with +argCount+ arguments specified
 * in array +args+; return the result in +result+
 */
bool scriptInvokeMethod(NPObject *npobj,
                        NPIdentifier name,
                        const NPVariant *args,
                        uint32_t argCount,
                        NPVariant *result)
{
  DEBUG_TRACE("scriptInvokeMethod");
  DEBUG_NPIDENTIFIER(DEBUG_LEVEL_INFO, name);
  
  STRINGZ_TO_NPVARIANT("HELLO", *result);
  return true;
}

bool scriptInvokeDefaultMethod(NPObject *npobj,
                               const NPVariant *args,
                               uint32_t argCount,
                               NPVariant *result)
{
  DEBUG_TRACE("scriptInvokeDefaultMethod");
  
  STRINGZ_TO_NPVARIANT("HELLO", *result);
  return true;
}

bool scriptHasProperty(NPObject *npobj, NPIdentifier name)
{
  DEBUG_TRACE("scriptHasProperty");
  DEBUG_NPIDENTIFIER(DEBUG_LEVEL_INFO, name);
  
  return true;
}

bool scriptGetProperty(NPObject *npobj, NPIdentifier name, NPVariant *result)
{
  DEBUG_TRACE("scriptGetProperty");
  DEBUG_NPIDENTIFIER(DEBUG_LEVEL_INFO, name);
  
  STRINGZ_TO_NPVARIANT("HELLO", *result);
  return true;
}

bool scriptSetProperty(NPObject *npobj, NPIdentifier name, const NPVariant *value)
{
  DEBUG_TRACE("scriptSetProperty");
  DEBUG_NPIDENTIFIER(DEBUG_LEVEL_INFO, name);
  
  return true;
}

bool scriptRemoveProperty(NPObject *npobj, NPIdentifier name)
{
  DEBUG_TRACE("scriptRemoveProperty");
  DEBUG_NPIDENTIFIER(DEBUG_LEVEL_INFO, name);
  
  return true;
}

