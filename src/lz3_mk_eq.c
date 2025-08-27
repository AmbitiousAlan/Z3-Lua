// ctx:mk_eq(ast1, ast2) -> ast (Bool)
static int lz3_context_mk_eq(lua_State *L) {
    lz3_context *ctxud = (lz3_context *)luaL_checkudata(L, 1, LZ3_CTX_MT);
    lz3_ast *a = (lz3_ast *)luaL_checkudata(L, 2, LZ3_AST_MT);
    lz3_ast *b = (lz3_ast *)luaL_checkudata(L, 3, LZ3_AST_MT);

    // Same-context checks
    if (a->ctx != ctxud->ctx || b->ctx != ctxud->ctx) {
        return luaL_error(L, "mk_eq: both ASTs must come from the same context as 'ctx'");
    }

    // Sort check (Z3 requires equal sorts for equality)
    Z3_sort sa = Z3_get_sort(ctxud->ctx, a->ast);
    Z3_sort sb = Z3_get_sort(ctxud->ctx, b->ast);
    if (sa != sb) {
        return luaL_error(L, "mk_eq: arguments must have the same sort");
    }

    Z3_ast eq = Z3_mk_eq(ctxud->ctx, a->ast, b->ast);
    Z3_inc_ref(ctxud->ctx, eq);

    // wrap in userdata
    lz3_ast *ud = (lz3_ast *)lua_newuserdatauv(L, sizeof(lz3_ast), 0);
    ud->ast = eq;
    ud->ctx = ctxud->ctx;
    luaL_getmetatable(L, LZ3_AST_MT);
    lua_setmetatable(L, -2);
    return 1;
}

