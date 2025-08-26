#define LZ3_SORT_MT "lz3.sort"

typedef struct {
    Z3_context ctx;
    Z3_sort sort;
} lz3_sort;

// __gc for sort
static int lz3_sort_gc(lua_State *L) {
    lz3_sort *ud = (lz3_sort *)luaL_checkudata(L, 1, LZ3_SORT_MT);
    // Z3_sorts are reference-counted, so just decrement
    if (ud && ud->sort) {
        Z3_dec_ref(ud->ctx, (Z3_ast)ud->sort);
        ud->sort = NULL;
    }
    return 0;
}

static int lz3_sort_tostring(lua_State *L) {
    lz3_sort *ud = (lz3_sort *)luaL_checkudata(L, 1, LZ3_SORT_MT);

    char buf[128];
    Z3_sort sort = ud->sort;

    Z3_sort_kind k = Z3_get_sort_kind(ud->ctx, sort);
    Z3_symbol s   = Z3_get_sort_name(ud->ctx, sort);

    if (Z3_get_symbol_kind(ud->ctx, s) == Z3_STRING_SYMBOL) {
        snprintf(buf, sizeof(buf), "z3.sort: %s", Z3_get_symbol_string(ud->ctx, s));
    } else {
        snprintf(buf, sizeof(buf), "z3.sort: kind %d (symbol #%d)",
                 k, Z3_get_symbol_int(ud->ctx, s));
    }

    lua_pushstring(L, buf);
    return 1;
}


#define LZ3_LUAOPEN_sort \
luaL_newmetatable(L, LZ3_SORT_MT);\
lua_newtable(L);\
lua_setfield(L, -2, "__index");\
luaL_setfuncs(L, (const luaL_Reg[]) {\
    {"__gc", lz3_sort_gc},\
    {"__tostring", lz3_sort_tostring},\
    {NULL, NULL}\
}, 0);\
lua_pop(L, 1);
