gcc -std=c11 -O2 -fPIC -shared src/lz3.c -I./src -I./lua -I./z3/api -lz3 -Wl,-rpath,'$ORIGIN/z3' -o build/lz3.so

