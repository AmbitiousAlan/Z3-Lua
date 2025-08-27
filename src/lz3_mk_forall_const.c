// ctx:mk_forall_const(weight, {var1, var2, ...}, body) -> ast
static int lz3_context_mk_forall_const(lua_State *L) {
    lz3_context *ctxud  = (lz3_context *)luaL_checkudata(L, 1, LZ3_CTX_MT);
    unsigned     weight = (unsigned)luaL_checkinteger(L, 2);

    // vars: Lua table
    luaL_checktype(L, 3, LUA_TTABLE);
    size_t num_bound = lua_rawlen(L, 3);

    // body: ast
    lz3_ast *bodyud = (lz3_ast *)luaL_checkudata(L, 4, LZ3_AST_MT);
    if (bodyud->ctx != ctxud->ctx) {
        return luaL_error(L, "mk_forall_const: body must come from same context");
    }

    // Fast path: no bound variables
    if (num_bound == 0) {
        Z3_ast q0 = Z3_mk_forall_const(ctxud->ctx, weight, 0, NULL, 0, NULL, bodyud->ast);
        Z3_inc_ref(ctxud->ctx, q0);
        lz3_ast *ud0 = (lz3_ast *)lua_newuserdatauv(L, sizeof(lz3_ast), 0);
        ud0->ctx = ctxud->ctx;
        ud0->ast = q0;
        luaL_getmetatable(L, LZ3_AST_MT);
        lua_setmetatable(L, -2);
        return 1;
    }

    // VLA: array of bound constants (Z3_app)
    Z3_app bound[num_bound];

    // Fill bound[] from the table
    for (size_t i = 0; i < num_bound; ++i) {
        lua_rawgeti(L, 3, (int)i + 1);            // vars[i+1]
        lz3_ast *vud = (lz3_ast *)luaL_checkudata(L, -1, LZ3_AST_MT);
        lua_pop(L, 1);

        if (vud->ctx != ctxud->ctx) {
            return luaL_error(L, "mk_forall_const: var #%d from different context", (int)i + 1);
        }
        if (!Z3_is_app(ctxud->ctx, vud->ast)) {
            return luaL_error(L, "mk_forall_const: var #%d must be a constant (application)", (int)i + 1);
        }
        Z3_app app = Z3_to_app(ctxud->ctx, vud->ast);
        if (Z3_get_app_num_args(ctxud->ctx, app) != 0) {
            return luaL_error(L, "mk_forall_const: var #%d must be a 0-arity constant", (int)i + 1);
        }
        bound[i] = app;
    }

    Z3_ast q = Z3_mk_forall_const(
        ctxud->ctx,
        weight,
        (unsigned)num_bound,
        bound,
        0, NULL,          // no patterns
        bodyud->ast
    );

    Z3_inc_ref(ctxud->ctx, q);

    // wrap in userdata
    lz3_ast *ud = (lz3_ast *)lua_newuserdatauv(L, sizeof(lz3_ast), 0);
    ud->ctx = ctxud->ctx;
    ud->ast = q;
    luaL_getmetatable(L, LZ3_AST_MT);
    lua_setmetatable(L, -2);
    return 1;
}

