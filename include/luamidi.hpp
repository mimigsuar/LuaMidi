#pragma once

#include <lua.hpp>

class LuaMidi {
public:
	LuaMidi();
	~LuaMidi();
	bool load_file(const char *path);
private:
	void _init_notes(); 
	lua_State *_lua;
};
