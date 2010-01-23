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
  return NPERR_NO_ERROR;
}

/**
 * Called by the browser to retrieve instance variables from the plugin 
 */
NPError NPP_GetValue(NPP instance,         // the instance
                     NPPVariable variable, // the variable to retrieve
                     void *value)          // where to put the value (?)
{
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
  return NPERR_NO_ERROR;
}

/**
 * Called once, when the plugin is unloaded
 */
void NP_Shutdown(void)
{
  return;
}

/**********************/

/**
 * creates a new NPObject for use in scripting
 */
NPObject *newScriptObject(NPP instance)
{
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

bool scriptHasMethod(NPObject *npobj, NPIdentifier name)
{
  return false;
}

bool scriptInvokeMethod(NPObject *npobj,
                        NPIdentifier name,
                        const NPVariant *args,
                        uint32_t argCount,
                        NPVariant *result)
{
  return false;
}

bool scriptInvokeDefaultMethod(NPObject *npobj,
                               const NPVariant *args,
                               uint32_t argCount,
                               NPVariant *result)
{
  return false;
}

bool scriptHasProperty(NPObject *npobj, NPIdentifier name)
{
  return false;
}

bool scriptGetProperty(NPObject *npobj, NPIdentifier name, NPVariant *result)
{
  return false;
}

bool scriptSetProperty(NPObject *npobj, NPIdentifier name, const NPVariant *value)
{
  return false;
}

bool scriptRemoveProperty(NPObject *npobj, NPIdentifier name)
{
  return false;
}

