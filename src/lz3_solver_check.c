// ctx:solver_check(solver) -> "sat" | "unsat" | "undef"
static int lz3_context_solver_check(lua_State *L) {
    lz3_context *ctxud = (lz3_context *)luaL_checkudata(L, 1, LZ3_CTX_MT);
    lz3_solver  *sud   = (lz3_solver  *)luaL_checkudata(L, 2, LZ3_SOLVER_MT);

    if (sud->ctx != ctxud->ctx) {
        return luaL_error(L, "solver_check: solver must come from the same context as 'ctx'");
    }

    Z3_lbool r = Z3_solver_check(ctxud->ctx, sud->solver);

    switch (r) {
        case Z3_L_TRUE:
            lua_pushstring(L, "sat");
            break;
        case Z3_L_FALSE:
            lua_pushstring(L, "unsat");
            break;
        default:
            lua_pushstring(L, "undef");
            break;
    }

    return 1; // return result string
}

