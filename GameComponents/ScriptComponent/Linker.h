#ifndef __SCRIPTCOMPONENT_GUARD_linklib__
#define __SCRIPTCOMPONENT_GUARD_linklib__

#if defined(_DEBUG)
	#pragma comment (lib,"ScriptComponent-mt-d.lib")
#elif !defined(_DEBUG)
	#pragma comment (lib,"ScriptComponent-mt.lib")
#else
	#error link: no suitable library
#endif

#endif // __SCRIPTCOMPONENT_GUARD_linklib__