#ifndef __LuaAnimationObject__
#define __LuaAnimationObject__

#include <lua.hpp>
#include <luabind\luabind.hpp>
#include <string>
#include "LuaAnimation.h"
#include "LuaNode.h"

namespace LuaAnimationObject
{
	class BasicAnimationObject
	{
	public:
		BasicAnimationObject();
		BasicAnimationObject(LuaAnimation::Animation anim, LuaNode::Node rootNode);
		void Release();
		std::string ID;

		static void Register(lua_State *lua);
	};

	class AnimationPlayer
	{
	public:
		AnimationPlayer();
		AnimationPlayer(BasicAnimationObject v);
		void Release();
		std::string ID;

		static void Register(lua_State *lua);
	};

	class AnimationController
	{
	public:
		AnimationController();
		AnimationController(AnimationPlayer v);
		void Release();
		std::string ID;

		static void Register(lua_State *lua);
	};

	void RegisterAllLuaFunction(lua_State *lua);
}

#endif //__LuaAnimationObject__