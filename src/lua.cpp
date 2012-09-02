#include "lua.h"

using namespace std;

void * lua_getLudFromTable(lua_State * _state, int _index)
{
	void * ret = 0;

	if(lua_istable(_state, _index))
	{
		lua_getfield(_state, _index, "__lud");

		ret = lua_touserdata(_state, -1);

		lua_pop(_state, _index);
	}

	return ret;
}
bool lua_setLudOnTable(lua_State * _state, void * _lud, int _index)
{
	if(lua_istable(_state, _index))
	{
		lua_pushlightuserdata(_state, _lud);

		lua_setfield(_state, _index, "__lud");

		return true;
	}

	return false;
}

lua_State * LuaObject::luaState;

void LuaObject::setLuaReference(int _luaRef)
{
	m_luaRef = _luaRef;
}

LuaObject::LuaObject()
{
	m_luaRef = LUA_NOREF;
}

void LuaObject::setState(lua_State * _luaState)
{
	luaState = _luaState;
}
