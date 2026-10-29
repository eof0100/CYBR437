/* Part 1, Task 1: Convert the supplied main function back to C. */
int main(void) {
    int a = 25;       /* movl $0x19,-0x4(%rbp): 0x19 is 25. */
    int b = 30;       /* movl $0x1e,-0x8(%rbp): 0x1e is 30. */
    long c = 45;      /* movq $0x2d,-0x10(%rbp): 8 byte long, value 45. */

    /* Load a into edx and b into eax. add computes a+b as an int.
       cltq extends the int result from eax to the 64 bit rax register.
       sub subtracts the old c from rax. mov saves the result in c. */
    c = a + b - c;
    return 0;         /* mov $0x0,%eax puts the return value in eax. */
}
