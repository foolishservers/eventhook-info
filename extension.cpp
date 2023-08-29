/**
 * vim: set ts=4 :
 * =============================================================================
 * SourceMod Sample Extension
 * Copyright (C) 2004-2008 AlliedModders LLC.  All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, AlliedModders LLC gives you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, AlliedModders LLC grants
 * this exception to all derivative works.  AlliedModders LLC defines further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 *
 * Version: $Id$
 */

#include "extension.h"

/**
 * @file extension.cpp
 * @brief Implement extension code here.
 */

EHInfo g_EHInfo;		/**< Global singleton for extension's main interface */

SMEXT_LINK(&g_EHInfo);

// native bool IsEventHooked(Handle plugin, const char[] name);

struct EventHook
{
	EventHook()
	{
		pPreHook = NULL;
		pPostHook = NULL;
		postCopy = false;
		refCount = 0;
	}
	IChangeableForward *pPreHook;
	IChangeableForward *pPostHook;
	bool postCopy;
	unsigned int refCount;
	std::string name;

	static inline bool matches(const char *name, const EventHook *hook)
	{
		return strcmp(name, hook->name.c_str()) == 0;
	}
	static inline uint32_t hash(const detail::CharsAndLength &key)
	{
		return key.hash();
	}
};

typedef SourceHook::List<EventHook *> EventHookList;

cell_t IsEventHookedEx(IPluginContext *pContext, const cell_t *params)
{
	Handle_t plugin;
	const char *name;
	HandleError *err;

	plugin = static_cast<Handle_t>(params[1]);
	pContext->LocalToString(params[2], const_cast<char **>(&name));
	pContext->LocalToPhysAddr(params[3], reinterpret_cast<cell_t **>(&err));

	IPlugin *pPlugin = plsys->PluginFromHandle(plugin, err);
	if(pPlugin == NULL)
	{
		return static_cast<cell_t>(false);
	}

	EventHookList *pHookList;
	if(!pPlugin->GetProperty("EventHooks", reinterpret_cast<void **>(&pHookList)))
	{
		return static_cast<cell_t>(false);
	}

	EventHookList::iterator it;
	for(it = pHookList->begin(); it != pHookList->end(); it++)
	{
		EventHook *pHook = (*it);

		if(EventHook::matches(name, pHook))
		{
			return static_cast<cell_t>(true);
		}
	}

    return static_cast<cell_t>(false);
}

const sp_nativeinfo_t natives[] = 
{
	{"IsEventHookedEx",	IsEventHookedEx},
	{NULL,				NULL},
};

void EHInfo::SDK_OnAllLoaded()
{
	sharesys->AddNatives(myself, natives);
}
