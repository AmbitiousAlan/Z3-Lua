#define LZ3_FUNC_DECL_MT "lz3.func_decl"

typedef struct {
    Z3_context ctx;
    Z3_func_decl decl;
} lz3_func_decl;


// __gc for func_decl
static int lz3_func_decl_gc(lua_State *L) {
    lz3_func_decl *ud = (lz3_func_decl *)luaL_checkudata(L, 1, LZ3_FUNC_DECL_MT);
    if (ud && ud->decl) {
        Z3_dec_ref(ud->ctx, Z3_func_decl_to_ast(ud->ctx, ud->decl));
        ud->decl = NULL;
    }
    return 0;
}

static int lz3_func_decl_tostring(lua_State *L) {
    lz3_func_decl *ud = (lz3_func_decl *)luaL_checkudata(L, 1, LZ3_FUNC_DECL_MT);

    const char *str = NULL;
    char buf[128];

    Z3_symbol s = Z3_get_decl_name(ud->ctx, ud->decl);
    unsigned arity = Z3_get_arity(ud->ctx, ud->decl);

    if (Z3_get_symbol_kind(ud->ctx, s) == Z3_STRING_SYMBOL) {
        snprintf(buf, sizeof(buf), "z3.func_decl: %s/%u",
                 Z3_get_symbol_string(ud->ctx, s), arity);
    } else {
        snprintf(buf, sizeof(buf), "z3.func_decl: symbol#%d/%u",
                 Z3_get_symbol_int(ud->ctx, s), arity);
    }

    str = buf;
    lua_pushstring(L, str);
    return 1;
}


#define LZ3_LUAOPEN_func_decl \
luaL_newmetatable(L, LZ3_FUNC_DECL_MT); \
lua_newtable(L); \
lua_setfield(L, -2, "__index"); \
luaL_setfuncs(L, (const luaL_Reg[]){ \
    {"__gc", lz3_func_decl_gc}, \
    {"__tostring", lz3_func_decl_tostring}, \
    {NULL, NULL} \
}, 0); \
lua_pop(L, 1);

