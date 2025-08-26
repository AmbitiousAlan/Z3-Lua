#define LZ3_AST_MT "lz3.ast"

typedef struct {
    Z3_context ctx;
    Z3_ast ast;
} lz3_ast;


// __gc for ast
static int lz3_ast_gc(lua_State *L) {
    lz3_ast *ud = (lz3_ast *)luaL_checkudata(L, 1, LZ3_AST_MT);
    if (ud && ud->ast) {
        Z3_dec_ref(ud->ctx, ud->ast);
        ud->ast = NULL;
    }
    return 0;
}

static int lz3_ast_tostring(lua_State *L) {
    lz3_ast *ud = (lz3_ast *)luaL_checkudata(L, 1, LZ3_AST_MT);

    // Default: just print pointer
    const char *kind_str = NULL;
    char buf[128];

    Z3_ast ast = ud->ast;
    Z3_ast_kind k = Z3_get_ast_kind(ud->ctx, ast);

    switch (k) {
    case Z3_NUMERAL_AST:
        snprintf(buf, sizeof(buf), "z3.ast: numeral %s",
                 Z3_get_numeral_string(ud->ctx, ast));
        kind_str = buf;
        break;

    case Z3_APP_AST: {
        Z3_app app = Z3_to_app(ud->ctx, ast);
        Z3_func_decl d = Z3_get_app_decl(ud->ctx, app);
        Z3_symbol s = Z3_get_decl_name(ud->ctx, d);

        if (Z3_get_symbol_kind(ud->ctx, s) == Z3_STRING_SYMBOL) {
            snprintf(buf, sizeof(buf), "z3.ast: const %s",
                     Z3_get_symbol_string(ud->ctx, s));
        } else {
            snprintf(buf, sizeof(buf), "z3.ast: const (symbol #%d)",
                     Z3_get_symbol_int(ud->ctx, s));
        }
        kind_str = buf;
        break;
    }

    default:
        snprintf(buf, sizeof(buf), "z3.ast: kind %d @%p", k, (void*)ast);
        kind_str = buf;
        break;
    }

    lua_pushstring(L, kind_str);
    return 1;
}


#define LZ3_LUAOPEN_ast \
luaL_newmetatable(L, LZ3_AST_MT); \
lua_newtable(L); \
lua_setfield(L, -2, "__index"); \
luaL_setfuncs(L, (const luaL_Reg[]){ \
    {"__gc", lz3_ast_gc}, \
    {"__tostring", lz3_ast_tostring}, \
    {NULL, NULL} \
}, 0); \
lua_pop(L, 1);
