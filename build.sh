# -Build Variable
CC="gcc"
ARCHIVE="ar"
FLAGS="-Wall -Wextra -std=c11"

# -Build Dir
mkdir --parents ".build/examples"

# -Library
ALLOCLIB=".build/liballoc.a"
$CC $FLAGS -I "include" -c -o ".build/allocator.o" "src/allocator.c"
$CC $FLAGS -I "include" -c -o ".build/allocator.page.o" "src/allocator.page.c"
$ARCHIVE crs $ALLOCLIB ".build/allocator.o" ".build/allocator.page.o"
