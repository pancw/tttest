
#ifndef __LLUA_H_
#define __LLUA_H_

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include <iostream>
#include <sys/time.h>

void luaopen_libs(lua_State *L);
int error_fun(lua_State *state);

#endif
