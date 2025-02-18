#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include "lua.h"
#include "core.h"
#include "graphics.h"

lua_State *L;


int luaBgSet(lua_State *L)
{
    u16 x = lua_tonumber(L, -3);
    u16 y = lua_tonumber(L, -2);
    u8 tileIndex = lua_tonumber(L, -1);
    bgSet(x, y, tileIndex);
    return 0; // number of results
}
int luaBgGet(lua_State *L)
{
    u16 x = lua_tonumber(L, -2);
    u16 y = lua_tonumber(L, -1);
    lua_pushnumber(L, bgGet(x, y));
    return 1; // number of results
}

int luaGetMouseX(lua_State *L)
{
    lua_pushnumber(L, (calcMousePosition().x));
    return 1; // number of results
}
int luaGetMouseY(lua_State *L)
{
    lua_pushnumber(L, (calcMousePosition().y));
    return 1; // number of results
}

int luaBtn(lua_State *L)
{
    u8 btncode = lua_tonumber(L, -1);
    lua_pushnumber(L, btn(btncode));
    return 1; // number of results
}
int luaSetScrollX(lua_State *L)
{
    int x = lua_tonumber(L, -1);
    setScrollX(x);
    return 0; // number of results
}
int luaSetScrollY(lua_State *L)
{
    int y = lua_tonumber(L, -1);
    setScrollY(y);
    return 0; // number of results
}
int luaGetKeyPressed(lua_State *L)
{
    lua_pushnumber(L, getKeyPressed());
    return 1; // number of results
}
int luaGetCharPressed(lua_State *L)
{
    lua_pushnumber(L, getCharPressed());
    return 1; // number of results
}
void printStack(lua_State *L) {
    int top = lua_gettop(L);
    printf("Total elements in the stack: %d\n", top);
    for (int i = 1; i <= top; i++) {
        int t = lua_type(L, i);
        switch (t) {
            case LUA_TSTRING:
                printf("%d String: %s\n",i ,lua_tostring(L, i));
                break;
            case LUA_TBOOLEAN:
                printf("%d Boolean: %s\n",i, lua_toboolean(L, i) ? "true" : "false");
                break;
            case LUA_TNUMBER:
                printf("%d Number: %g\n",i, lua_tonumber(L, i));
                break;
            default:
                printf("%d Other: %s\n",i, lua_typename(L, t));
                break;
        }
    }
}
int luaBgTileLoad(lua_State *L)
{
    u8 index = lua_tonumber(L, -2);
    tile out;
    lua_pushnil(L);  // first key
    int y = 0;
    printf("\n");
    while (lua_next(L, -2) != 0) {
        // key is at -2 and value is at -1
        lua_pushnil(L);  // first key
        while (lua_next(L, -2) != 0) {
            // key is at -2 and value is at -1
            u8 key = lua_tonumber(L, -2);
            u8 value = lua_tonumber(L, -1);
            out[y][key-1] = value;
            lua_pop(L, 1);
        }
        y++;
        lua_pop(L, 1);
    }

    bgTileLoad(index, out);

    return 0; // number of results

}

void registerFunctions()
{
    lua_register(L, "bgSet", luaBgSet);
    lua_register(L, "bgGet", luaBgGet);
    lua_register(L, "getMouseX", luaGetMouseX);
    lua_register(L, "getMouseY", luaGetMouseY);
    lua_register(L, "btn", luaBtn);
    lua_register(L, "setScrollX", luaSetScrollX);
    lua_register(L, "setScrollY", luaSetScrollY);
    lua_register(L, "getKeyPressed", luaGetKeyPressed);
    lua_register(L, "getCharPressed", luaGetCharPressed);
    lua_register(L, "bgTileLoad", luaBgTileLoad);
}



void initLua(string script)
{
    L = luaL_newstate();
    luaL_openlibs(L);
    registerFunctions();
    if (luaL_dostring(L, script) != LUA_OK) {
        fprintf(stderr, "Error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    }

}
void closeLua()
{
    lua_close(L);
}


void execLuaVBLANK()
{
    lua_getglobal(L, "Vblank");
    if(lua_isfunction(L, -1))
        lua_call(L, 0, 0);
    else
    {
        lua_pop(L, 1);
        //printf("Vblank not found!!!\n");
    }
}
void execLuaLoop()
{
    lua_getglobal(L, "loop");
    if(lua_isfunction(L, -1))
        lua_call(L, 0, 0);
    else
    {
        lua_pop(L, 1);
        printf("Loop not found!!!\n");
    }
}
void execLuaSetup()
{
    lua_getglobal(L, "setup");
    if(lua_isfunction(L, -1))
        lua_call(L, 0, 0);
    else
    {
        lua_pop(L, 1);
        printf("Setup not found!!!\n");
    }
}

