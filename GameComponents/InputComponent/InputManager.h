#ifndef __InputManager__
#define __InputManager__

#include "Linker.h"

#include <Interface.h>
#include <Singleton.h>
#include <InputKeysEnum.h>

class InputComponentDLL_API InputManager : public Interface, public CHL::Singleton<InputManager>
{
protected:	
	InputManager();
	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;
	friend CHL::Singleton<InputManager>;
public:
	virtual void Init();
	virtual void Work(double realTime, double deltaTime);
	virtual void Shutdown();

	InputKeysEnum::KeyStatus GetCurrentKeyState(InputKeysEnum::KeyCode key);
	std::pair<long, long> CursorPosition();
};

#endif //__InputManager__
