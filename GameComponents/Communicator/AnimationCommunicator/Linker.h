#ifndef __ANIMATIONCOMMUNICATOR_GUARD_linklib__
#define __ANIMATIONCOMMUNICATOR_GUARD_linklib__

#if defined(_DEBUG)
#pragma comment (lib,"AnimationCommunicator-mt-d.lib")
#elif !defined(_DEBUG)
#pragma comment (lib,"AnimationCommunicator-mt.lib")
#else
#error link: no suitable library
#endif

#endif // __ANIMATIONCOMMUNICATOR_GUARD_linklib__