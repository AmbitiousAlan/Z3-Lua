// ctx:true()
static int lz3_context_mk_true(lua_State *L) {
    lz3_context *ctxud = (lz3_context *)luaL_checkudata(L, 1, LZ3_CTX_MT);

    Z3_ast ast = Z3_mk_true(ctxud->ctx);
    Z3_inc_ref(ctxud->ctx, ast);

    // wrap in userdata
    lz3_ast *ud = (lz3_ast *)lua_newuserdatauv(L, sizeof(lz3_ast), 0);
    ud->ast = ast;
    ud->ctx = ctxud->ctx;

    luaL_getmetatable(L, LZ3_AST_MT);
    lua_setmetatable(L, -2);

    return 1;
}

