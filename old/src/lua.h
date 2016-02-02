#ifndef LUA_H
#define LUA_H

#include <iostream>

extern "C"
{
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}

void * lua_getLudFromTable(lua_State * _state, int _index);
bool lua_setLudOnTable(lua_State * _state, void * _lud, int _index);

class LuaObject
{
  protected:

	static lua_State * luaState;

	int m_luaRef;

	virtual void setLuaReference(int _luaRef);

  public:

	LuaObject();

	static void setState(lua_State * _luaState);

	//virtual static void initLuaObject(lua_State * _luaState) = 0;
	virtual void createLuaInstance() = 0; //Create an instance table and add it to the registry.
	virtual void pushLuaInstance() = 0; //Push aforementioned table on the top
};

#endif
