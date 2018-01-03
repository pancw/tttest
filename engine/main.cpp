#include <iostream>
#include <cstdio>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "engine_base.h"
#include "llua.h"
#include "ac.h"

lua_State *GlobalL;

static void luaopen_all_libs(lua_State * L)
{
	luaL_openlibs(L);
	luaopen_libs(L);
	luaopen_ac_libs(L);
}

int main()
{
	printf("server start.\n");

	if (chdir("../logic") == -1)
	{
		fprintf(stderr, "bad logic path to dir:%s\n", "../logic");
		return 1;
	}

	// init
	engine_base_init();	
	GlobalL = luaL_newstate();
	luaopen_all_libs(GlobalL);

    lua_pushcclosure(GlobalL, error_fun, 0); 
    int err = luaL_loadfile(GlobalL, "main.lua");   
    if (err)
    {   
        fprintf(stderr, "%s\n", lua_tostring(GlobalL, -1));
        return 1;
    }   

    int ret = lua_pcall(GlobalL, 0, 0, -2);
    if (ret)
    {   
        fprintf(stderr, "call main error:%s\n", lua_tostring(GlobalL, -1));
        return 1;
    }

	// listen

	engine_loop();
	// release
	return (0);
}
