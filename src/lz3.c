#include <lua.h>
#include <lauxlib.h>
#include <z3.h>
#include <string.h>
#include <stdio.h>

#include "lz3_context.c"
#include "lz3_ast.c"
#include "lz3_sort.c"
#include "lz3_uninterpreted_sort.c"
#include "lz3_const.c"
#include "lz3_bool_sort.c"

static const luaL_Reg lz3_ctx_methods[] = {
    {"close",               lz3_context_close},               // context.c
    {"uninterpreted_sort",  lz3_context_uninterpreted_sort},  // uninterpreted_sort.c
    {"bool_sort",           lz3_context_bool_sort},           // bool_sort.c
    {"const",               lz3_context_const},               // lz3_const.c
    {NULL, NULL}
};

static const luaL_Reg lz3_module[] = {
    {"context", lz3_context_new}, //context.c
    {NULL, NULL}
};

int luaopen_lz3(lua_State *L) {
    LZ3_LUAOPEN_context;
    LZ3_LUAOPEN_sort;
    LZ3_LUAOPEN_ast;

    // module table
    lua_newtable(L);
    luaL_setfuncs(L, lz3_module, 0);

    return 1;
}

