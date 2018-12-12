#include "luamidi.hpp"

#include <cstring>
#include <iostream>

static Song *song; 

LuaMidi::LuaMidi()
{
	song = new Song() ;
	_lua = luaL_newstate();
	luaL_openlibs(_lua);
}

LuaMidi::~LuaMidi()
{
	lua_close(_lua);
	delete song; 
}

bool LuaMidi::load_file(const char *path)
{
	if (luaL_loadfile(_lua, path)) {
		const char *str = lua_tostring(_lua, -1);
		std::cout << str << std::endl;
		return false;
	}
	
	_init_midilib();	

	if (lua_pcall(_lua, 0, LUA_MULTRET, 0)) {
		const char *str = lua_tostring(_lua, -1);
		std::cout << str << std::endl;
		return false;
	
	}

	return true;
}

void LuaMidi::_init_midilib()
{
	lua_pushcfunction(_lua, &LuaMidi::_notename_to_midi);
	lua_setglobal(_lua, "note");

	lua_pushcfunction(_lua, &LuaMidi::_play); 
	lua_setglobal(_lua, "play");

	lua_pushcfunction(_lua, &LuaMidi::_next);
	lua_setglobal(_lua, "next");
}

int LuaMidi::_notename_to_midi(lua_State *L) 
{
	const char *notename = luaL_checkstring(L, 1);
	// note(notename:str) returns midi //
	
	int len = strlen(notename);	
	if (len > 3) {
		luaL_error(L, "Invalid notename %s. Longer than 3 characters.", notename); 
		return 0;
	}

	if (len < 2) {
		luaL_error(L, "Invalid notename %s. Shorter than 2 characters.", notename); 
		return 0;
	}

	// check the octave first //
	int octave = notename[len - 1] - '0';
	if (octave < 0 || octave > 8) {
		luaL_error(L, "Invalid octave %d. The octaves range from 0 to 8.", octave);
		return 0;
	}
	octave += 2;

	// next, get the pitch class //
	int pc = 0;
	switch (toupper(notename[0])) {
		case 'C': 
			pc = 0;
			break;

		case 'D':
			pc = 2;
			break;

		case 'E':
			pc = 4;
			break;

		case 'F':
			pc = 5;
			break;

		case 'G':
			pc = 7;
			break;

		case 'A':
			pc = 9;
			break;

		case 'B':
			pc = 11;
			break;

		default:
			luaL_error(L, "Invalid pitch class %c. Must be a musical letter.", notename[0]); 
			return 0;
	}
	
	// finally, check if it is sharp or flat //
	int delta = 0;
	if (len == 3) {
		if (notename[1] == 'b') {
			delta = -1;
		} else if (notename[1] == '#') {
			delta = 1;
		} else {
			luaL_error(L, "Invalid accidental %c. Only # or b are allowed.", notename[1]); 
			return 0;
		}
	}

	int midi_note = (octave * 12) + pc + delta; 
	lua_pushinteger(L, midi_note);
	return 1;	

}

int LuaMidi::_play(lua_State *L)
{
	std::vector<int> channels;
	std::vector<int> pitches;
	std::vector<double> lengths; 

	if (lua_type(L, 1) == LUA_TNUMBER) {
		channels.push_back(lua_tonumber(L, 1));
	} else if (lua_type(L, 1) == LUA_TTABLE) {
		for (int i = 0; i < luaL_len(L, 1); i++) {
			lua_geti(L, 1, i + 1); 
			int channel = lua_tointeger(L, -1);
			lua_pop(L, 1);
			channels.push_back(channel);	
		}
	}

	if (lua_type(L, 2) == LUA_TNUMBER) {
		pitches.push_back(lua_tonumber(L, 1));
	} else if (lua_type(L, 2) == LUA_TTABLE) {
		for (int i = 0; i < luaL_len(L, 2); i++) {
			lua_geti(L, 2, i + 1); 
			int pitch = lua_tointeger(L, -1);
			lua_pop(L, 1);
			pitches.push_back(pitch);	
		}
	}

	if (lua_type(L, 3) == LUA_TNUMBER) {
		lengths.push_back(lua_tonumber(L, 1));
	} else if (lua_type(L, 3) == LUA_TTABLE) {
		for (int i = 0; i < luaL_len(L, 3); i++) {
			lua_geti(L, 3, i + 1); 
			double length = lua_tonumber(L, -1);
			lua_pop(L, 1);
			lengths.push_back(length);	
		}
	}

	// now combine them ALL //
	for (auto channel : channels) {
		for (auto pitch : pitches) {
			for (auto length : lengths) {
				Note nt;
				nt.channel = channel;
				nt.pitch = pitch;
				nt.length = length;
				song->notes.push_back(nt);
			}
		}
	}

	return 0;
}

int LuaMidi::_next(lua_State *L)
{
	return 0;
}
