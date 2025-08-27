// ctx:solver_pop(solver [, n]) -> solver
static int lz3_context_solver_pop(lua_State *L) {
    lz3_context *ctxud = (lz3_context *)luaL_checkudata(L, 1, LZ3_CTX_MT);
    lz3_solver  *sud   = (lz3_solver  *)luaL_checkudata(L, 2, LZ3_SOLVER_MT);

    if (sud->ctx != ctxud->ctx) {
        return luaL_error(L, "solver_pop: solver must come from the same context as 'ctx'");
    }

    // optional count argument, defaults to 1
    unsigned n = 1;
    if (!lua_isnoneornil(L, 3)) {
        n = (unsigned)luaL_checkinteger(L, 3);
    }

    Z3_solver_pop(ctxud->ctx, sud->solver, n);

    // return the solver for fluent chaining
    lua_settop(L, 2);
    return 1;
}
