#include "luamidi.hpp"

#include <iostream>
#include <vector>

LuaMidi::LuaMidi()
{
	_lua = luaL_newstate();
	luaL_openlibs(_lua);
}

LuaMidi::~LuaMidi()
{
	lua_close(_lua);
}

bool LuaMidi::load_file(const char *path)
{
	luaL_loadfile(_lua, path);
	
	_init_notes();	

	if (lua_pcall(_lua, 0, LUA_MULTRET, 0)) {
		const char *str = lua_tostring(_lua, -1);
		std::cout << "Lua error: " << str << std::endl;
		return false;
	
	}

	return true;
}

void LuaMidi::_init_notes()
{
	int middle_c = 60;
	char notes[7] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'}; 
	char octave = '3'; 
	int c_note = 2; 
	lua_pushnumber(_lua, middle_c); 


	for (i=middle_c; i < 128; i++) {
		
	}
}
