#!/usr/bin/env bash
# Run this script from inside the ArtemPeychevLab5 folder.
set -eu
mkdir -p bin images
# Part 1 ONLY: match the simple assembly layout in the handout.
# The handout has no canary in these three assembly examples.
# Part 3 below explicitly enables canaries for every build.
for task in 1 2 3; do
    gcc -O0 -g -Wall -Wextra -fcf-protection=full -fno-stack-protector "p1_task${task}.c" -o "bin/p1_task${task}"
done
# Keep stack canaries enabled in all Part 3 programs.
gcc -O0 -g -Wall -Wextra -fstack-protector-all p3_city_vulnerable.c -o bin/p3_city_vulnerable
gcc -O0 -g -Wall -Wextra -fstack-protector-all p3_city_fixed.c -o bin/p3_city_fixed
# AddressSanitizer detects the array overflow reliably. Stack protection stays on.
gcc -O0 -g -Wall -Wextra -fstack-protector-all -fsanitize=address -fno-omit-frame-pointer p3_copy_vulnerable.c -o bin/p3_copy_vulnerable
gcc -O0 -g -Wall -Wextra -fstack-protector-all -fsanitize=address -fno-omit-frame-pointer p3_copy_fixed.c -o bin/p3_copy_fixed
# Stop at undefined signed overflow rather than assuming wraparound is valid C.
gcc -O0 -g -Wall -Wextra -fstack-protector-all -fsanitize=undefined -fno-sanitize-recover=undefined p4_addition.c -o bin/p4_addition
gcc -O0 -g -Wall -Wextra -fstack-protector-all -fsanitize=undefined -fno-sanitize-recover=undefined p4_subtraction.c -o bin/p4_subtraction
printf '\nBuild finished. Start with QUICKSTART.md.\n'
