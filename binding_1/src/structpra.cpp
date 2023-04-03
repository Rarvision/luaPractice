#include <iostream>
#include <cstring>
#include <string>
#pragma comment(lib, "liblua53.a")

extern "C"
{
#include "lua.hpp"
}

struct Point{
    float x;
    float y;
};

Point *p = new Point({1,2});

int lua_getPosition(lua_State *L) {
    
    // Point** userdata = (Point**)lua_newuserdata(L, sizeof(Point*));
    // *userdata = p;
    // luaL_getmetatable(L, "Point");
    // lua_setmetatable(L, 1);

    lua_pushlightuserdata(L, p);
    luaL_getmetatable(L, "Point");
    lua_setmetatable(L, 1);

    std::cout << "getPositionCalled" << std::endl;
    return 1;
}

int point_index(lua_State *L) {
    std::cout << "point_index called" << std::endl;
    Point *point = (Point *)lua_touserdata(L, 1);
    const char *key = luaL_checkstring(L, 2);

    if (std::strcmp(key, "x") == 0) {
        lua_pushinteger(L, point->x);
        return 1;
    } else if (strcmp(key, "y") == 0) {
        lua_pushinteger(L, point->y);
        return 1;
    }

    return 0;
}

int point_newindex(lua_State *L) {
    std::cout << "point_newindex called" << std::endl;
    Point *point = (Point *)lua_touserdata(L, 1);
    const char *key = luaL_checkstring(L, 2);
    int value = luaL_checkinteger(L, 3);

    if (strcmp(key, "x") == 0) {
        point->x = value;
        return 0;
    } else if (strcmp(key, "y") == 0) {
        point->y = value;
        return 0;
    }

    return 0;
}

int point_gc(lua_State *L) {
    Point *point = (Point *)lua_touserdata(L, 1);
    delete point;
    return 0;
}

void register_point_metatable(lua_State* L) {
    luaL_newmetatable(L, "Point");
    lua_pushcfunction(L, point_gc);
    lua_setfield(L, -2, "__gc");
    lua_pushcfunction(L, point_index);
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, point_newindex);
    lua_setfield(L, -2, "__newindex");
    lua_pop(L,1);
}

int main() {
    
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    lua_register(L, "getPosition", lua_getPosition);
    register_point_metatable(L);

    if(luaL_dofile(L, "script.lua") == LUA_OK)
    {
        lua_pcall(L,0,0,0);
        std::cout << p->x << " " << p->y << std::endl;
        
    } 
    else
    {
        std::string errormsg = lua_tostring(L, -1);
        std::cout << errormsg << std::endl;
    }

    lua_close(L);


    return 0;
}