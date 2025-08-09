# -Build Variable
CC="gcc"
ARCHIVE="ar"
FLAGS="-Wall -Wextra -std=c11"

# -Build Dir
mkdir --parents ".build/examples"

# -Library
ALLOCLIB=".build/liballoc.a"
$CC $FLAGS -I "include" -c -o ".build/allocator.o" "src/allocator.c"
$CC $FLAGS -I "include" -c -o ".build/allocator.bump.o" "src/allocator.bump.c"
$CC $FLAGS -I "include" -c -o ".build/allocator.fba.o" "src/allocator.fba.c"
$CC $FLAGS -I "include" -c -o ".build/allocator.linear.o" "src/allocator.linear.c"
$CC $FLAGS -I "include" -c -o ".build/allocator.page.o" "src/allocator.page.c"
$ARCHIVE crs $ALLOCLIB ".build/allocator.o" ".build/allocator.bump.o" ".build/allocator.fba.o" ".build/allocator.linear.o" ".build/allocator.page.o"

# Examples
$CC $FLAGS -I "include" -o ".build/examples/allocator.arena" "examples/allocator.arena.c" $ALLOCLIB
$CC $FLAGS -I "include" -o ".build/examples/allocator.bump" "examples/allocator.bump.c" $ALLOCLIB
$CC $FLAGS -I "include" -o ".build/examples/allocator.fba" "examples/allocator.fba.c" $ALLOCLIB
$CC $FLAGS -I "include" -o ".build/examples/allocator.linear" "examples/allocator.linear.c" $ALLOCLIB
$CC $FLAGS -I "include" -o ".build/examples/allocator.gp" "examples/allocator.gp.c" $ALLOCLIB
$CC $FLAGS -I "include" -o ".build/examples/allocator.page" "examples/allocator.page.c" $ALLOCLIB
