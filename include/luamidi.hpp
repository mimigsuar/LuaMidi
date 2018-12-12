#pragma once

#include <lua.hpp>
#include <vector>

struct Note {
	Note()
	{
		channel = 0;
		pitch = 0;
		start = 0;
		length = 0;
	}
	
	int channel; 
	int pitch; // note 
	double start; // beat 
	double length; // beat
};

struct Song {
	Song() 
	{
		beat = 0.0;
		velocity = 127; 
	}
	
	double beat;
	int velocity;
	std::vector<Note> notes; 
};

class LuaMidi {
public:
	LuaMidi();
	~LuaMidi();
	bool load_file(const char *path);
private:
	void _init_midilib();
	
	// Lua Functions //
	static int _notename_to_midi(lua_State *L);  
	static int _play(lua_State *L);
	static int _next(lua_State *L);

	lua_State *_lua;
};
