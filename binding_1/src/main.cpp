#include <iostream>
#include <string>
#pragma comment(lib, "liblua53.a")

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

int lua_HostFunction(lua_State* L) 
{
    float a = (float)lua_tonumber(L, 1);
    float b = (float)lua_tonumber(L, 2);
    float c = a+b;
    lua_pushnumber(L, c);
    return 1;
}

int lab() 
{
    struct Player
    {
        std::string title;
        std::string name;
        std::string family;
        int level;
    } player;

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    lua_register(L, "HostFunction", lua_HostFunction);

    int r = luaL_dofile(L, "script.lua");

    if(r == LUA_OK)
    {
        lua_getglobal(L, "DoAThing");
        if(lua_isfunction(L,-1))
        {
            lua_pushnumber(L, 3.5f);
            lua_pushnumber(L, 7.1f);
            if(lua_pcall(L,2,1,0) == LUA_OK) {
                std::cout << (float)lua_tonumber(L,-1) << std::endl;
            } else {
                std::cout << lua_tostring(L, -1) << std::endl;
            }
        }
        
    } 
    else
    {
        std::string errormsg = lua_tostring(L, -1);
        std::cout << errormsg << std::endl;
    }

    //system("read -p 'Press Enter to continue...' var");
    lua_close(L);

    return 0;
}