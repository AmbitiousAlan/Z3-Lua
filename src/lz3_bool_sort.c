// ctx:bool_sort()
static int lz3_context_bool_sort(lua_State *L) {
    lz3_context *ctxud = (lz3_context *)luaL_checkudata(L, 1, LZ3_CTX_MT);

    Z3_sort sort = Z3_mk_bool_sort(ctxud->ctx);
    Z3_inc_ref(ctxud->ctx, (Z3_ast)sort); // sorts are asts (ref-counted)

    // wrap in userdata
    lz3_sort *ud = (lz3_sort *)lua_newuserdatauv(L, sizeof(lz3_sort), 0);
    ud->sort = sort;
    ud->ctx  = ctxud->ctx;
    luaL_getmetatable(L, LZ3_SORT_MT);
    lua_setmetatable(L, -2);
    return 1;
}
