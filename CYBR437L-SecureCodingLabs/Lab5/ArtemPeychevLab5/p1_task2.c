/* Part 1, Task 2: Convert the comparison and jumps back to C. */
int main(void) {
    int a = 25;       /* movl $0x19,-0x4(%rbp) */
    int b = 30;       /* movl $0x1e,-0x8(%rbp) */
    int result;

    /* mov loads a into eax. cmp compares a with b.
       jle goes to the else branch when a <= b.
       Otherwise movl $0x1 stores 1 in result.
       jmp skips over the else branch afterward. */
    if (a > b) {
        result = 1;
    } else {
        result = 0;  /* movl $0x0,-0xc(%rbp) */
    }
    return 0;        /* mov $0x0,%eax */
}
