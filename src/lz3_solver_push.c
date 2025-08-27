// ctx:solver_push(solver) -> solver
static int lz3_context_solver_push(lua_State *L) {
    lz3_context *ctxud = (lz3_context *)luaL_checkudata(L, 1, LZ3_CTX_MT);
    lz3_solver  *sud   = (lz3_solver  *)luaL_checkudata(L, 2, LZ3_SOLVER_MT);

    if (sud->ctx != ctxud->ctx) {
        return luaL_error(L, "solver_push: solver must come from the same context as 'ctx'");
    }

    Z3_solver_push(ctxud->ctx, sud->solver);

    // return the solver for fluent chaining
    lua_settop(L, 2);
    return 1;
}
