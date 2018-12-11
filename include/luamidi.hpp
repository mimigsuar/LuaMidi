#pragma once

#include <lua.hpp>
#include <vector>

class LuaMidi {
public:
	LuaMidi();
	~LuaMidi();
	bool load_file(const char *path);
private:
	void _init_midilib();
	
	// Lua Functions //
	static int _notename_to_midi(lua_State *L);  
	static int _change_tempo();

	lua_State *_lua;

	std::vector<int> tempos;
};
