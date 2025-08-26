// ctx:uninterpreted_sort("varname")
static int lz3_context_uninterpreted_sort(lua_State *L) {
    lz3_context *ctxud = (lz3_context *)luaL_checkudata(L, 1, LZ3_CTX_MT);
    const char *name = luaL_checkstring(L, 2);

    Z3_symbol sym = Z3_mk_string_symbol(ctxud->ctx, name);
    Z3_sort sort = Z3_mk_uninterpreted_sort(ctxud->ctx, sym);
    Z3_inc_ref(ctxud->ctx, (Z3_ast)sort); // sorts are asts

    // wrap in userdata
    lz3_sort *ud = (lz3_sort *)lua_newuserdatauv(L, sizeof(lz3_sort), 0);
    ud->sort = sort;
    ud->ctx = ctxud->ctx;
    luaL_getmetatable(L, LZ3_SORT_MT);
    lua_setmetatable(L, -2);
    return 1;
}
