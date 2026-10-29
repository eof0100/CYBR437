/* Part 1, Task 3: Convert both fun and main back to C. */
int fun(int a, int b) {
    /* edi and esi hold the two integer arguments.
       mov saves them at rbp-4 and rbp-8, then loads a into edx
       and b into eax. add computes a+b; sub $0x1 subtracts 1.
       eax holds the value that ret returns to the caller. */
    return a + b - 1;
}

int main(void) {
    int a = 5;       /* movl $0x5,-0x4(%rbp) */
    int b = 6;       /* movl $0x6,-0x8(%rbp) */
    /* mov puts b into esi and a into edi before call fun.
       After the call, mov %eax,-0x4(%rbp) saves the result in a. */
    a = fun(a, b);
    return 0;        /* mov $0x0,%eax */
}
