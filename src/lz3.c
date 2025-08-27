#include <lua.h>
#include <lauxlib.h>
#include <z3.h>
#include <string.h>
#include <stdio.h>

#include "lz3_context.c"
#include "lz3_ast.c"
#include "lz3_sort.c"
#include "lz3_func_decl.c"
#include "lz3_solver.c"
#include "lz3_mk_solver.c"
#include "lz3_uninterpreted_sort.c"
#include "lz3_const.c"
#include "lz3_bool_sort.c"
#include "lz3_mk_func_decl.c"
#include "lz3_mk_false.c"
#include "lz3_mk_true.c"
#include "lz3_mk_app.c"
#include "lz3_mk_eq.c"
#include "lz3_solver_assert.c"
#include "lz3_mk_forall_const.c"
#include "lz3_solver_push.c"
#include "lz3_solver_pop.c"
#include "lz3_solver_check.c"

static const luaL_Reg lz3_ctx_methods[] = {
    {"close",               lz3_context_close},               // context.c
    {"uninterpreted_sort",  lz3_context_uninterpreted_sort},  // uninterpreted_sort.c
    {"bool_sort",           lz3_context_bool_sort},           // bool_sort.c
    {"const",               lz3_context_const},               // lz3_const.c
    {"mk_func_decl",        lz3_context_mk_func_decl},        // lz3_mk_func_decl.c
    {"mk_solver",           lz3_context_mk_solver},           // lz3_mk_solver.c  
    {"mk_false",            lz3_context_mk_false},            // lz3_mk_false.c  
    {"mk_true",             lz3_context_mk_true},             // lz3_mk_true.c 
    {"mk_app",              lz3_context_mk_app},              // lz3_mk_app.c 
    {"mk_eq",               lz3_context_mk_eq},               // lz3_mk_eq.c 
    {"solver_assert",       lz3_context_solver_assert},       // lz3_solver_assert.c 
    {"mk_forall_const",     lz3_context_mk_forall_const},     // lz3_mk_forall_const.c 
    {"solver_push",         lz3_context_solver_push},         // lz3_solver_push.c 
    {"solver_pop",          lz3_context_solver_pop},          // lz3_solver_pop.c 
    {"solver_check",        lz3_context_solver_check},        // lz3_solver_check.c 
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
    LZ3_LUAOPEN_func_decl;
    LZ3_LUAOPEN_solver;

    // module table
    lua_newtable(L);
    luaL_setfuncs(L, lz3_module, 0);

    return 1;
}

