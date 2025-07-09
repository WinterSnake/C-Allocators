# -Build Variable
CC="gcc"
FLAGS="-Wall -Wextra -std=c11"

# -Build Dir
mkdir --parents ".build/examples"

# -Library

# Examples
$CC $FLAGS -I "include" -o ".build/examples/allocators" "examples/allocators.c" "src/allocator.fixed_buffer.c"
