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

#define PLUGIN_NAME "ChromeCrypt"
#define PLUGIN_DESC "Encryption thingy for Chrome"

NPError NP_GetEntryPoints(NPPluginFuncs *pluginFuncs);
NPError NP_Initialize(NPNetscapeFuncs *browserFuncs);
NPError NPP_New(NPMIMEType pluginType, NPP instance, uint16_t mode, int16_t argc, char* argn[], char* argv[], NPSavedData* saved);
NPError NPP_GetValue(NPP instance, NPPVariable variable, void *value);
NPError NPP_Destroy(NPP instance, NPSavedData** save);
void    NP_Shutdown(void);


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
