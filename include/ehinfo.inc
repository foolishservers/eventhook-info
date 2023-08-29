#if defined _pluginsyswrapper_included_
  #endinput
#endif
#define _pluginsyswrapper_included_

public Extension __ext_pluginsyswrapper = 
{
	name = "PluginSys Wrapper",
	file = "pluginsyswrapper.ext",
	autoload = 1,
	required = 1,
};

enum HandleError
{
    HandleError_None = 0,		/**< No error */
    HandleError_Changed,		/**< The handle has been freed and reassigned */
    HandleError_Type,			/**< The handle has a different type registered */
    HandleError_Freed,			/**< The handle has been freed */
    HandleError_Index,			/**< generic internal indexing error */
    HandleError_Access,			/**< No access permitted to free this handle */
    HandleError_Limit,			/**< The limited number of handles has been reached */
    HandleError_Identity,		/**< The identity token was not usable */
    HandleError_Owner,			/**< Owners do not match for this operation */
    HandleError_Version,		/**< Unrecognized security structure version */
    HandleError_Parameter,		/**< An invalid parameter was passed */
    HandleError_NoInherit,		/**< This type cannot be inherited */
};

/**
 * @brief Checks if the event is hooked by the plugin.
 *
 * @param plugin	Handle of the plugin.
 * @param name		Name of the event.
 * @param err       Error, set on handle failure.
 * @return			True if the event is hooked, false otherwise.
 */
native bool IsEventHookedEx(Handle plugin, const char[] name, HandleError &err);

/**
 * @brief Checks if the event is hooked by the plugin.
 *
 * @param plugin	Handle of the plugin.
 * @param name		Name of the event.
 * @return			True if the event is hooked, false otherwise.
 */
stock bool IsEventHooked(Handle plugin, const char[] name)
{
    HandleError err;
    return IsEventHookedEx(plugin, name, err);
}