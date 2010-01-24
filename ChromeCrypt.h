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
#define MIME_TYPES_DESCRIPTION "application/x-chromecrypt:ccrypt:Cypto Plugin for Chrome"

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
