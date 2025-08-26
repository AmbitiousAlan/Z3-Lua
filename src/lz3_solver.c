#define LZ3_SOLVER_MT "lz3.solver"

typedef struct {
    Z3_context ctx;
    Z3_solver solver;
} lz3_solver;


// __gc for solver
static int lz3_solver_gc(lua_State *L) {
    lz3_solver *ud = (lz3_solver *)luaL_checkudata(L, 1, LZ3_SOLVER_MT);
    if (ud && ud->solver) {
        Z3_solver_dec_ref(ud->ctx, ud->solver);
        ud->solver = NULL;
    }
    return 0;
}

static int lz3_solver_tostring(lua_State *L) {
    lz3_solver *ud = (lz3_solver *)luaL_checkudata(L, 1, LZ3_SOLVER_MT);

    const char *str = Z3_solver_to_string(ud->ctx, ud->solver);
    if (!str) {
        char buf[64];
        snprintf(buf, sizeof(buf), "z3.solver: @%p", (void*)ud->solver);
        lua_pushstring(L, buf);
    } else {
        lua_pushfstring(L, "z3.solver: %s", str);
    }
    return 1;
}


// Registration
#define LZ3_LUAOPEN_solver \
luaL_newmetatable(L, LZ3_SOLVER_MT); \
lua_newtable(L); \
lua_setfield(L, -2, "__index"); \
luaL_setfuncs(L, (const luaL_Reg[]){ \
    {"__gc", lz3_solver_gc}, \
    {"__tostring", lz3_solver_tostring}, \
    {NULL, NULL} \
}, 0); \
lua_pop(L, 1);

