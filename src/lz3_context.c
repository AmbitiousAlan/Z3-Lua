#define LZ3_CTX_MT "lz3.context"

typedef struct {
    Z3_context ctx;
} lz3_context;

// Convert a Lua value at idx to a string for Z3_set_param_value
static void to_param_string(lua_State *L, int idx, char *buf, size_t buflen) {
    int t = lua_type(L, idx);
    switch (t) {
        case LUA_TSTRING:
            snprintf(buf, buflen, "%s", lua_tostring(L, idx));
            break;
        case LUA_TBOOLEAN:
            snprintf(buf, buflen, "%s", lua_toboolean(L, idx) ? "true" : "false");
            break;
        case LUA_TNUMBER:
            if (lua_isinteger(L, idx))
                snprintf(buf, buflen, "%lld", (long long)lua_tointeger(L, idx));
            else
                snprintf(buf, buflen, "%.17g", lua_tonumber(L, idx));
            break;
        default:
            snprintf(buf, buflen, "%s", luaL_tolstring(L, idx, NULL)), lua_pop(L,1);
            break;
    }
}

// __gc: delete context
static int lz3_context_gc(lua_State *L) {
    lz3_context *ud = (lz3_context *)luaL_checkudata(L, 1, LZ3_CTX_MT);
    if (ud && ud->ctx) {
        Z3_del_context(ud->ctx);
        ud->ctx = NULL;
    }
    return 0;
}

// :close() — optional manual close
static int lz3_context_close(lua_State *L) {
    return lz3_context_gc(L);
}

// tostring
static int lz3_context_tostring(lua_State *L) {
    lz3_context *ud = (lz3_context *)luaL_checkudata(L, 1, LZ3_CTX_MT);
    lua_pushfstring(L, "z3.context: %p", (void*)ud->ctx);
    return 1;
}

// z3.context([cfg_table])
// cfg_table is a Lua table { key = value, ... } passed to Z3_set_param_value
static int lz3_context_new(lua_State *L) {
    int has_cfg = !lua_isnoneornil(L, 1);
    if (has_cfg && !lua_istable(L, 1)) {
        return luaL_argerror(L, 1, "expected table or nil");
    }

    Z3_config cfg = Z3_mk_config();
    if (has_cfg) {
        lua_pushnil(L); // first key
        while (lua_next(L, 1) != 0) {
            // stack: ... key value
            if (lua_type(L, -2) != LUA_TSTRING) {
                lua_pop(L, 1); // pop value
                return luaL_error(L, "config keys must be strings");
            }
            const char *key = lua_tostring(L, -2);
            char valbuf[128];
            to_param_string(L, -1, valbuf, sizeof(valbuf));
            Z3_set_param_value(cfg, key, valbuf);
            lua_pop(L, 1); // pop value, keep key for next lua_next
        }
    }

    // Use reference-counted context variant
    Z3_context ctx = Z3_mk_context_rc(cfg);
    Z3_del_config(cfg);

    if (!ctx) {
        return luaL_error(L, "Z3_mk_context_rc returned NULL");
    }

    // Create userdata
    lz3_context *ud = (lz3_context *)lua_newuserdatauv(L, sizeof(lz3_context), 0);
    ud->ctx = ctx;

    // Set metatable
    luaL_getmetatable(L, LZ3_CTX_MT);
    lua_setmetatable(L, -2);

    return 1; // userdata
}

static const luaL_Reg lz3_ctx_meta[] = {
    {"__gc",       lz3_context_gc}, //context.c
    {"__tostring", lz3_context_tostring}, //context.c
    {NULL, NULL}
};



#define LZ3_LUAOPEN_context \
luaL_newmetatable(L, LZ3_CTX_MT);\
lua_newtable(L);\
luaL_setfuncs(L, lz3_ctx_methods, 0);\
lua_setfield(L, -2, "__index");\
luaL_setfuncs(L, lz3_ctx_meta, 0);\
lua_pop(L, 1);
