# -Build Variable
CC="gcc"
ARCHIVE="ar"
FLAGS="-Wall -Wextra -std=c11"

# -Build Dir
mkdir --parents ".build/examples"

# -Library
ALLOCLIB=".build/liballoc.a"
$CC $FLAGS -I "include" -c -o ".build/allocators.o" "src/allocators.c"
$CC $FLAGS -I "include" -c -o ".build/allocator.fba.o" "src/allocator.fba.c"
$ARCHIVE crs $ALLOCLIB ".build/allocators.o" ".build/allocator.fba.o"

# Examples
$CC $FLAGS -I "include" -o ".build/examples/allocator.fba" "examples/allocator.fba.c" $ALLOCLIB
