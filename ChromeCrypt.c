/* 
 * ChromeCrypt.c
 *
 * (c) 2010 Sam Quigley <quigley@emerose.com>
 * originally based on BasicPlugin by the Mozilla Corporation, but
 * changed a lot since then.
 */

#include "ChromeCrypt.h"

static NPObject scriptObject;

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
/*      *((NPObject **)value) = &scriptObject;
      return NPERR_NO_ERROR;
*/
      return NPERR_GENERIC_ERROR;

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
