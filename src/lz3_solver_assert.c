// ctx:solver_assert(solver, ast1 [, ast2 ...]) -> solver
static int lz3_context_solver_assert(lua_State *L) {
    lz3_context *ctxud = (lz3_context *)luaL_checkudata(L, 1, LZ3_CTX_MT);
    lz3_solver  *sud   = (lz3_solver  *)luaL_checkudata(L, 2, LZ3_SOLVER_MT);
    int n = lua_gettop(L);

    if (n < 3) {
        return luaL_error(L, "solver_assert expects: ctx, solver, at least one AST");
    }

    // solver must belong to this context
    if (sud->ctx != ctxud->ctx) {
        return luaL_error(L, "solver_assert: solver must come from the same context as 'ctx'");
    }

    // Precompute Bool sort for validation (from this ctx)
    Z3_sort bool_sort = Z3_mk_bool_sort(ctxud->ctx);

    for (int i = 3; i <= n; ++i) {
        lz3_ast *aud = (lz3_ast *)luaL_checkudata(L, i, LZ3_AST_MT);

        // context check
        if (aud->ctx != ctxud->ctx) {
            return luaL_error(L, "solver_assert: argument #%d must come from the same context as 'ctx'", i - 2);
        }

        // sort check: only Bool can be asserted
        Z3_sort as = Z3_get_sort(ctxud->ctx, aud->ast);
        if (as != bool_sort) {
            return luaL_error(L, "solver_assert: argument #%d must be Bool sort", i - 2);
        }

        Z3_solver_assert(ctxud->ctx, sud->solver, aud->ast);
    }

    // return the solver (fluent chaining)
    lua_settop(L, 2);
    return 1;
}

