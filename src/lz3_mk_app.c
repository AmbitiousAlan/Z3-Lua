// ctx:mk_app(func_decl, arg1, arg2, ...)
static int lz3_context_mk_app(lua_State *L) {
    lz3_context *ctxud = (lz3_context *)luaL_checkudata(L, 1, LZ3_CTX_MT);
    lz3_func_decl *fud = (lz3_func_decl *)luaL_checkudata(L, 2, LZ3_FUNC_DECL_MT);

    if (fud->ctx != ctxud->ctx) {
        return luaL_error(L, "function declaration must come from the same context");
    }

    int nargs = lua_gettop(L) - 2;
    Z3_ast args[nargs];

    for (int i = 0; i < nargs; i++) {
        lz3_ast *argud = (lz3_ast *)luaL_checkudata(L, i + 3, LZ3_AST_MT);
        if (argud->ctx != ctxud->ctx) {
            return luaL_error(L, "all arguments must come from the same context as the function");
        }
        args[i] = argud->ast;
    }

    Z3_ast app = Z3_mk_app(ctxud->ctx, fud->decl, nargs, args);
    Z3_inc_ref(ctxud->ctx, app);

    lz3_ast *resud = (lz3_ast *)lua_newuserdatauv(L, sizeof(lz3_ast), 0);
    resud->ast = app;
    resud->ctx = ctxud->ctx;

    luaL_getmetatable(L, LZ3_AST_MT);
    lua_setmetatable(L, -2);

    return 1;
}

