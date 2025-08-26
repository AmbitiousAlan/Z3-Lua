// ctx:func_decl("f", {Sort1, Sort2, ...}, RangeSort)
static int lz3_context_mk_func_decl(lua_State *L) {
    lz3_context *ctxud = (lz3_context *)luaL_checkudata(L, 1, LZ3_CTX_MT);
    const char *name   = luaL_checkstring(L, 2);

    // 3rd argument must be a table of domain sorts
    luaL_checktype(L, 3, LUA_TTABLE);
    int domain_count = luaL_len(L, 3);

    Z3_sort domain_sorts[domain_count];   // <-- C99 VLA
    for (int i = 0; i < domain_count; i++) {
        lua_rawgeti(L, 3, i + 1);
        lz3_sort *sortud = (lz3_sort *)luaL_checkudata(L, -1, LZ3_SORT_MT);
        if (sortud->ctx != ctxud->ctx) {
            return luaL_error(L, "domain sort must come from the same context");
        }
        domain_sorts[i] = sortud->sort;
        lua_pop(L, 1);
    }

    // 4th argument: range sort
    lz3_sort *rangeud = (lz3_sort *)luaL_checkudata(L, 4, LZ3_SORT_MT);
    if (rangeud->ctx != ctxud->ctx) {
        return luaL_error(L, "range sort must come from the same context");
    }

    // make the function decl
    Z3_symbol sym = Z3_mk_string_symbol(ctxud->ctx, name);
    Z3_func_decl decl = Z3_mk_func_decl(ctxud->ctx, sym,
                                        domain_count, domain_sorts,
                                        rangeud->sort);

    // increment ref (func_decl is an AST-like object)
    Z3_inc_ref(ctxud->ctx, Z3_func_decl_to_ast(ctxud->ctx, decl));

    // wrap in userdata
    lz3_func_decl *ud = (lz3_func_decl *)lua_newuserdatauv(L, sizeof(lz3_func_decl), 0);
    ud->decl = decl;
    ud->ctx  = ctxud->ctx;

    luaL_getmetatable(L, LZ3_FUNC_DECL_MT);
    lua_setmetatable(L, -2);

    return 1;
}

