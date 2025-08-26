// ctx:const("varname", sort)
static int lz3_context_const(lua_State *L) {
    lz3_context *ctxud = (lz3_context *)luaL_checkudata(L, 1, LZ3_CTX_MT);
    const char *name = luaL_checkstring(L, 2);

    // accept a lz3_sort userdata as 3rd argument
    lz3_sort *sortud = (lz3_sort *)luaL_checkudata(L, 3, LZ3_SORT_MT);
    
    if (sortud->ctx != ctxud->ctx) {
        return luaL_error(L, "sort must come from the same context as the constant");
    }

    Z3_symbol sym = Z3_mk_string_symbol(ctxud->ctx, name);
    Z3_ast ast = Z3_mk_const(ctxud->ctx, sym, sortud->sort);
    Z3_inc_ref(ctxud->ctx, ast);

    // wrap in userdata
    lz3_ast *ud = (lz3_ast *)lua_newuserdatauv(L, sizeof(lz3_ast), 0);
    ud->ast = ast;
    ud->ctx = ctxud->ctx;
    luaL_getmetatable(L, LZ3_AST_MT);
    lua_setmetatable(L, -2);
    return 1;
}


