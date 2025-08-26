// ctx:solver()
static int lz3_context_mk_solver(lua_State *L) {
    lz3_context *ctxud = (lz3_context *)luaL_checkudata(L, 1, LZ3_CTX_MT);

    Z3_solver solver = Z3_mk_solver(ctxud->ctx);
    Z3_solver_inc_ref(ctxud->ctx, solver);

    // wrap in userdata
    lz3_solver *ud = (lz3_solver *)lua_newuserdatauv(L, sizeof(lz3_solver), 0);
    ud->solver = solver;
    ud->ctx = ctxud->ctx;

    luaL_getmetatable(L, LZ3_SOLVER_MT);
    lua_setmetatable(L, -2);

    return 1;
}
