local lz3 = require("lz3")

-- create context
local ctx = lz3.context()

-- sorts
local Entity  = ctx:uninterpreted_sort("Entity")
local Bool    = ctx:bool_sort()

-- constants (people)
local John    = ctx:const("John",  Entity)
local Edgar   = ctx:const("Edgar", Entity)
local Queen   = ctx:const("Queen", Entity)
local Rick    = ctx:const("Rick",  Entity)

-- predicates / functions
local is_human    = ctx:mk_func_decl("is_human",    {Entity},        Bool)
local beautiful   = ctx:mk_func_decl("beautiful",   {Entity},        Bool)
local powerful    = ctx:mk_func_decl("powerful",    {Entity},        Bool)
local would_marry = ctx:mk_func_decl("would_marry", {Entity,Entity}, Bool)

-- solver
local s = ctx:mk_solver()

-- helper literals
local T = ctx:mk_true()
local F = ctx:mk_false()

-- Facts
do
    local ihJ  = ctx:mk_app(is_human, John)
    ctx:solver_assert(s, ctx:mk_eq(ihJ, T))

    local ihE  = ctx:mk_app(is_human, Edgar)
    ctx:solver_assert(s, ctx:mk_eq(ihE, T))

    local ihQ  = ctx:mk_app(is_human, Queen)
    ctx:solver_assert(s, ctx:mk_eq(ihQ, T))

    local ihR  = ctx:mk_app(is_human, Rick)
    ctx:solver_assert(s, ctx:mk_eq(ihR, F))

    local bJ   = ctx:mk_app(beautiful, John)        -- John is ugly
    ctx:solver_assert(s, ctx:mk_eq(bJ, F))

    local bQ   = ctx:mk_app(beautiful, Queen)       -- Queen is beautiful
    ctx:solver_assert(s, ctx:mk_eq(bQ, T))

    local pJ   = ctx:mk_app(powerful, John)         -- assume power attrs
    ctx:solver_assert(s, ctx:mk_eq(pJ, F))

    local pE   = ctx:mk_app(powerful, Edgar)
    ctx:solver_assert(s, ctx:mk_eq(pE, F))
end

-- Quantified rules: forall x . would_marry(A, x) == P(x)
local x = ctx:const("x", Entity)

do
    local lhs = ctx:mk_app(would_marry, John,  x)
    local rhs = ctx:mk_app(is_human,    x)
    local body = ctx:mk_eq(lhs, rhs)
    ctx:solver_assert(s, ctx:mk_forall_const(0, {x}, body))
end

do
    local lhs = ctx:mk_app(would_marry, Edgar, x)
    local rhs = ctx:mk_app(beautiful,   x)
    local body = ctx:mk_eq(lhs, rhs)
    ctx:solver_assert(s, ctx:mk_forall_const(0, {x}, body))
end

do
    local lhs = ctx:mk_app(would_marry, Queen, x)
    local rhs = ctx:mk_app(powerful,    x)
    local body = ctx:mk_eq(lhs, rhs)
    ctx:solver_assert(s, ctx:mk_forall_const(0, {x}, body))
end

-- Queries (keep names for nicer printing)
local Q = {
    {"John",  John,  "Edgar", Edgar},
    {"John",  John,  "Queen", Queen},
    {"Edgar", Edgar, "Queen", Queen},
    {"Edgar", Edgar, "John",  John },
    {"Queen", Queen, "Edgar", Edgar},
    {"Queen", Queen, "John",  John },
    {"Rick",  Rick,  "John",  John },
    {"John",  John,  "Rick",  Rick },
}

for i = 1, #Q do
    local an, a, bn, b = table.unpack(Q[i])
    ctx:solver_push(s)

    local wm = ctx:mk_app(would_marry, a, b)
    ctx:solver_assert(s, ctx:mk_eq(wm, T))

    local res = ctx:solver_check(s)
    print(("%s would marry %s? %s"):format(an, bn, (res == "sat") and "Yes" or "No"))

    ctx:solver_pop(s)
end

