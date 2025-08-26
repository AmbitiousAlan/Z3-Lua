-- test_lz3.lua
local lz3 = require("lz3")

local function header(s) print(("\n=== %s ==="):format(s)) end
local function ok(cond, msg)
    if cond then print("[OK]  " .. msg)
    else error("[FAIL] " .. msg, 2) end
end

local function has_method(obj, name)
    local mt = getmetatable(obj)
    local idx = mt and mt.__index
    if type(idx) == "table" then return type(idx[name]) == "function" end
    if type(idx) == "function" then return type(obj[name]) == "function" end
    return type(obj[name]) == "function"
end

local function pcall_err(f, ...)
    local ok_, err = pcall(f, ...)
    return ok_, err
end

header("Context & basic sorts")

local ctx = lz3.context()
print(ctx)
ok(tostring(ctx):match("^z3%.context"), "context tostring looks right")

local Entity = ctx:uninterpreted_sort("Entity")
print(Entity)
ok(tostring(Entity):match("^z3%.sort"), "uninterpreted sort created")

local Bool = ctx:bool_sort()
print(Bool)
ok(tostring(Bool):match("^z3%.sort"), "bool_sort created")

header("Constants & function declarations")

local John = ctx:const("John", Entity)
local Mary = ctx:const("Mary", Entity)
print(John, Mary)
ok(tostring(John):match("^z3%.ast"), "const John is ast")
ok(tostring(Mary):match("^z3%.ast"), "const Mary is ast")

-- Unary predicate: IsQueen(Entity) -> Bool
local IsQueen = ctx:mk_func_decl("IsQueen", {Entity}, Bool)
ok(type(IsQueen) == "userdata", "mk_func_decl (unary) returned userdata")

-- Binary predicate: Married(Entity, Entity) -> Bool
local Married = ctx:mk_func_decl("Married", {Entity, Entity}, Bool)
ok(type(Married) == "userdata", "mk_func_decl (binary) returned userdata")

-- Build applications
local IsQueen_John = ctx:mk_app(IsQueen, John)
local IsQueen_Mary = ctx:mk_app(IsQueen, Mary)
local Married_John_John = ctx:mk_app(Married, John, John)

print(IsQueen_John, IsQueen_Mary, Married_John_John)
ok(tostring(IsQueen_John):match("^z3%.ast"), "mk_app(IsQueen, John) -> ast")
ok(tostring(Married_John_John):match("^z3%.ast"), "mk_app(Married, John, John) -> ast")

header("mk_true / mk_false terminals")

local T = ctx:mk_true()
local F = ctx:mk_false()
ok(tostring(T):match("^z3%.ast"), "mk_true() -> ast")
ok(tostring(F):match("^z3%.ast"), "mk_false() -> ast")

header("Optional: Solver smoke test (skips if methods not wired)")

local s = ctx:mk_solver()
if s and (has_method(s, "assert") or has_method(s, "add")) and has_method(s, "check") then
    -- Accept either :assert or :add naming
    local solver_assert = s.assert or s.add

    -- Simple consistent facts
    solver_assert(s, IsQueen_Mary)
    solver_assert(s, Married_John_John)

    local r = s:check()
    print("solver:check() ->", r)
    ok(r == "sat" or r == 0 or r == true, "solver found model for simple facts")

    -- Optional model/eval
    if has_method(s, "model") and has_method(s, "eval") then
        local m = s:model()
        if m then
            local v = s:eval(IsQueen_Mary, true) -- model completion
            print("eval(IsQueen(Mary)) ->", v)
            ok(v ~= nil, "eval returned a value")
        end
    end
else
    print("[SKIP] solver add/assert/check/model not available yet; continuing")
end

header("Cross-context safety")

local ctx2 = lz3.context()
local ok_, err = pcall_err(function()
    -- Must error: using a sort from ctx in ctx2:const
    return ctx2:const("X", Entity)
end)
ok(not ok_ and tostring(err):match("same context"), "cross-context sort use errors as expected")

header("GC sanity (no crashes expected)")

do
    for i = 1, 100 do
        local tmpB = ctx:bool_sort()
        local tmpC = ctx:const("tmp" .. i, Entity)
        local tmpP = ctx:mk_func_decl("P" .. i, {Entity}, Bool)
        local tmpA = ctx:mk_app(tmpP, tmpC)
        -- drop scope
    end
    collectgarbage()
end

print("\nAll basic tests passed (with skips where features aren’t wired yet).")

