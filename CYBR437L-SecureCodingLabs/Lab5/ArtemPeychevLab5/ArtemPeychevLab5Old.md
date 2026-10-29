Artem Peychev  
CYBR437 Secure Coding  
Lab 5 Assembly Code 



# Part 1: Assembly to C

I compiled these three programs with no optimization so I could compare the instructions. The build for Part 1 matches the handout without a stack canary. All Part 3 programs are compiled separately with stack protection enabled.

## Task 1

### C code

```c
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
```

### How the assembly matches my code

The first `movl` stores `0x19`, which is 25, in `a` at `rbp-4`. The next `movl` stores `0x1e`, which is 30, in `b` at `rbp-8`. The `movq` stores `0x2d`, which is 45, in `c` at `rbp-16`. The `l` instructions store 4 bytes, while the `q` instruction stores 8 bytes. On this Linux x86 64 system, `int` is 4 bytes and `long` is 8 bytes.

The next two `mov` instructions load `a` into `edx` and `b` into `eax`. The `add %edx,%eax` adds them, giving 55. The `cltq` changes the signed 32 bit result into a signed 64 bit result. The `sub -0x10(%rbp),%rax` subtracts the old value of `c`, giving 10. Then `mov %rax,-0x10(%rbp)` stores 10 back in `c`. This matches `c = a + b - c;`.

The final `mov $0x0,%eax` matches `return 0;`. The `push`, frame setup, `pop`, and `ret` instructions handle entering and leaving the function. The `endbr64` instruction is part of control flow protection.

### Assembly comparison

![Task 1 compiled main function.](images/1.png)

My comparison with the handout: **[Write what matched in your actual output. Addresses can differ.]**

## Task 2

### C code

```c
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
```

### How the assembly matches my code

The two `movl` instructions store 25 and 30 in the first two local variables. The next `mov` loads the first number into `eax`. The `cmp -0x8(%rbp),%eax` compares that first number with the second number.

The `jle` jumps to the branch that stores 0 when the first number is less than or equal to the second number. If the jump does not happen, the next instruction stores 1 in the third local variable at `rbp-12`. The `jmp` then skips the branch that stores 0. This matches my `if (a > b)` and `else` statements. With 25 and 30, `result` becomes 0.

The final `mov $0x0,%eax` is the return value of `main`. The function setup and ending instructions handle its stack frame, and `endbr64` is for control flow protection.

### Assembly comparison

![Task 2 compiled main function.](images/2.png)

My comparison with the handout: **[Write what matched in your actual output.]**

## Task 3

### C code

```c
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
```

### How the assembly matches my code

In `main`, `sub $0x10,%rsp` makes room on the stack. The two `movl` instructions store 5 and 6 in the local variables. The next instructions load 6 into `edx` and 5 into `eax`. They then copy these values into `esi` and `edi`. On this system, `edi` holds the first integer argument and `esi` holds the second one. This prepares the call `fun(a, b)`.

In `fun`, the first two `mov` instructions save those arguments in local stack positions. The next two load the first argument into `edx` and the second into `eax`. The `add` adds them together, and `sub $0x1,%eax` subtracts 1. This matches `return a + b - 1;`, giving 10. The return value is left in `eax`.

Back in `main`, `mov %eax,-0x4(%rbp)` stores that returned value in `a`. The next `mov` sets the return value of `main` to 0. The setup and ending instructions manage the stack frame. The `leave` instruction restores the frame before `ret` returns.

### Assembly comparison

![Task 3 compiled main and fun functions.](images/3.png)

My comparison with the handout: **[Write what matched in your actual output.]**

# Part 2: Memory management



## Task 1

The function uses malloc to allocate 32 bytes but never frees that memory.
When the function ends it loses the pointer to that memory causing a memory leak.

To fix I would add a line to free the text vuffer before it returns a 0.

## Task 2

The second printf uses a pointer after its memory has been already freed.
It is using the memory hafter it has been freed even if it prints 10 that memory no longer is safe to access.

To fix this ussie I would remove the second printf or move it right before
the free(ptr) line and would check if malloc returned NULL before using a pointer.



# Part 3: Buffer overflows

I compiled the Part 3 programs with `-fstack-protector-all`. I also used AddressSanitizer for the array copying task to detect the write outside the array. I did not disable the stack canary.

## Task 1: City input

### Vulnerable code

```c
/* Part 3, Task 1: Original vulnerable code from the assignment. */
#include <stdio.h>
int main(void) {
    char city[10];
    printf("Enter city: ");
    /* No width limit: a long word writes beyond the 10 byte array. */
    scanf("%s", city);
    printf("City: %s\n", city);
    return 0;
}
```

### Problem and test

`city` has room for 10 bytes. A C string also needs a final null byte, so only 9 characters fit. The `%s` conversion has no length limit, so a longer word can write outside the array.

I used `normal_city.txt`, containing `Spokane`, and `long_city.txt`, containing an 80 character word. Both files end with a newline.

![Original city program with short and long input.](images/4.png)

The short city printed correctly but the long inputt caused stack smashing detected error and the program stopped. The input was too long for 10 byte city array because scanf did not limit the chars it read.

### Corrected code

```c
/* Part 3, Task 1: Read the full city instead of rejecting long names. */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int main(void) {
    char *city = NULL;
    size_t capacity = 0;
    printf("Enter city: ");
    /* On Linux, getline allocates space and grows it when needed.
       The return value includes the newline but not the final null byte. */
    ssize_t length = getline(&city, &capacity, stdin);
    if (length < 0) {
        fprintf(stderr, "Could not read city.\n");
        free(city);
        return 1;
    }
    if (length > 0 && city[length - 1] == '\n') {
        city[length - 1] = '\0';
    }
    printf("City: %s\n", city);
    free(city); /* Release the memory that getline allocated. */
    return 0;
}
```

### Why the fix works

I replaced the fixed array and unbounded scan with `getline`. It allocates memory and grows the buffer when more space is needed. This lets the program read the whole city, including long names, instead of rejecting the input or cutting it short. I also remove the input newline and free the allocated memory when finished.

I tested the corrected program with the same two input files.

![Corrected city program with the same short and long input.](images/5.png)

Both short and long city names printed without crashing this time. The fixed program uses the getline which allows the growing of the buffer when more space is needed so the entire output will fit.

## Task 2: Copying an array

### Vulnerable code and test main

```c
/* Part 3, Task 2: Original function with a main that tests its bounds. */
#include <stdio.h>

int fun(double *to, double *from, int pos, int n) {
    int k;
    for (k = 0; k < n; k++) {
        to[pos] = from[k]; /* No check of the destination capacity. */
        pos++;
    }
    return pos;
}

int main(void) {
    double to[5] = {0};
    double from[6] = {10, 20, 30, 40, 50, 60};
    printf("Destination capacity: 5; start: 3; number to copy: 4\n");
    printf("This tries to write destination indexes 3, 4, 5, and 6.\n");
    fflush(stdout);
    int end = fun(to, from, 3, 4);
    printf("Returned position: %d\n", end);
    for (int k = 0; k < 5; k++) printf("%.0f ", to[k]);
    printf("\n");
    return 0;
}
```

### Problem and test

The function never checks how much space the destination has. My test destination has 5 elements, with valid indexes 0 through 4. Starting at position 3 and copying 4 values tries to write indexes 3, 4, 5, and 6. Indexes 5 and 6 are outside the destination. The source has 6 elements, which is more than the 4 being copied, as required.

![Original copying function and its detected overflow.](images/6.png)

The address sanitizer detected a write outside the destination array and only indexes 0 to 4 but it is actually copying four values starting with index 3 trying to write at index 5 and 6 so the program stopped when the overflow was detected.

### Corrected code and test main

```c
/* Part 3, Task 2: Grow the destination so all requested items fit. */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

/* to points to the caller's array pointer, so realloc can update it.
   capacity records the number of doubles currently allocated.
   from contains at least n elements, as the assignment assumes. */
int fun(double **to, const double *from, int pos, int n, size_t *capacity) {
    /* Prevent invalid indexes and overflow while calculating the size.
       A valid request that exceeds capacity is handled by growing the array. */
    if (pos < 0 || n < 0 || n > INT_MAX - pos) return -1;
    size_t needed = (size_t)pos + (size_t)n;
    if (needed > SIZE_MAX / sizeof(double)) return -1;

    if (needed > *capacity) {
        /* Use a temporary pointer so an allocation failure does not
           lose the original array. */
        double *larger = realloc(*to, needed * sizeof(double));
        if (larger == NULL) return -1;
        for (size_t k = *capacity; k < needed; k++) larger[k] = 0;
        *to = larger;
        *capacity = needed;
    }
    for (int k = 0; k < n; k++) {
        (*to)[pos] = from[k];
        pos++;
    }
    return pos;
}

int main(void) {
    size_t capacity = 5;
    double *to = calloc(capacity, sizeof(double));
    double from[6] = {10, 20, 30, 40, 50, 60};
    if (to == NULL) return 1;
    printf("Original capacity: %zu; start: 3; number to copy: 4\n", capacity);
    int end = fun(&to, from, 3, 4, &capacity);
    if (end < 0) {
        fprintf(stderr, "Could not complete the copy.\n");
        free(to);
        return 1;
    }
    printf("New capacity: %zu; returned position: %d\n", capacity, end);
    printf("Destination: ");
    for (size_t k = 0; k < capacity; k++) printf("%.0f ", to[k]);
    printf("\n");
    free(to);
    return 0;
}
```

### Why the fix works

I changed the function so it knows the destination capacity and can grow the destination with `realloc`. I also changed the destination in `main` to allocated memory, because a stack array cannot be resized with `realloc`.

The destination pointer is passed by address because `realloc` can move the array. The function checks the size calculation before allocating memory. When the request needs more space, it grows the destination instead of just rejecting the copy. It uses a temporary pointer so the old allocation is not lost if `realloc` fails.

I used the same source, starting position 3, copy count 4, and original destination capacity 5. The corrected function should grow the destination to 7 elements and return position 7. The destination should contain `0 0 0 10 20 30 40`.

![Corrected copying function using the same test values.](images/7.png)

Fixed program increases the array size from 5 to correct size of 7 and copied all four values returning to position number 7.

# Part 4: Integer overflows

I used `INT_MIN` and `INT_MAX` from `limits.h` instead of writing system specific limits as constants. I used UndefinedBehaviorSanitizer to report signed overflow in the original functions. Signed overflow is undefined behavior in C, so a wrapped result is not guaranteed.

## Task 1: Addition

### Source code

```c
/* Part 4, Task 1: Run with "unsafe" to demonstrate the original bug.
   Run with no argument to test the corrected function. */
#include <stdio.h>
#include <limits.h>
#include <string.h>

void fun1(int a, int b) {
    int sum;
    sum = a + b; /* Original code: signed overflow is undefined behavior. */
    printf("Result = %d\n", sum);
}

void fun1_fixed(int a, int b) {
    printf("a = %d, b = %d: ", a, b);
    /* For positive b, check the upper limit before adding.
       For negative b, check the lower limit before adding.
       The checks themselves stay within the int range. */
    if ((b > 0 && a > INT_MAX - b) ||
        (b < 0 && a < INT_MIN - b)) {
        printf("Error: integer overflow.\n");
        return;
    }
    printf("Result = %d\n", a + b);
}

int main(int argc, char **argv) {
    printf("INT_MIN = %d; INT_MAX = %d\n", INT_MIN, INT_MAX);
    if (argc > 1 && strcmp(argv[1], "unsafe") == 0) {
        printf("Original function: a = INT_MAX, b = 1\n");
        fflush(stdout);
        fun1(INT_MAX, 1);
    } else {
        printf("Corrected function:\n");
        fun1_fixed(INT_MAX, 1);   /* Same test that caused overflow. */
        fun1_fixed(INT_MIN, -1);  /* Also test the lower limit. */
        fun1_fixed(20, 30);       /* Ordinary safe addition: 50. */
        fun1_fixed(-20, -30);     /* Another safe addition: -50. */
    }
    return 0;
}
```

### Original function and chosen values

I chose `a = INT_MAX` and `b = 1`. Both arguments fit in an `int`, but their sum is one larger than the biggest value an `int` can hold. The original function adds them without checking first.

![Original addition function with overflow detected.](images/8.png)

What the original test actually reported: **[Describe the sanitizer message.]**

### Fix and repeated tests

When `b` is positive, I check whether `a > INT_MAX - b`. When `b` is negative, I check whether `a < INT_MIN - b`. These checks happen before the addition. If a check fails, the function prints an error instead of doing the overflowing addition.

I repeated the original test with `INT_MAX` and 1. I also tested `INT_MIN` and negative 1 to check the lower limit. For normal inputs, I used 20 and 30, which should give 50, and negative 20 and negative 30, which should give negative 50.

![Corrected addition function with overflow and normal inputs.](images/9.png)

What the corrected tests actually printed: **[Record the error messages and the two valid results.]**

## Task 2: Subtraction

### Source code

```c
/* Part 4, Task 2: Run with "unsafe" to demonstrate the original bug.
   Run with no argument to test the corrected function. */
#include <stdio.h>
#include <limits.h>
#include <string.h>

void fun2(int x, int y) {
    int diff;
    diff = x - y; /* Original code: signed overflow is undefined behavior. */
    printf("Result = %d\n", diff);
}

void fun2_fixed(int x, int y) {
    printf("x = %d, y = %d: ", x, y);
    /* Subtracting positive y can go below INT_MIN.
       Subtracting negative y can go above INT_MAX.
       Check before carrying out the subtraction. */
    if ((y > 0 && x < INT_MIN + y) ||
        (y < 0 && x > INT_MAX + y)) {
        printf("Error: integer overflow.\n");
        return;
    }
    printf("Result = %d\n", x - y);
}

int main(int argc, char **argv) {
    printf("INT_MIN = %d; INT_MAX = %d\n", INT_MIN, INT_MAX);
    if (argc > 1 && strcmp(argv[1], "unsafe") == 0) {
        printf("Original function: x = INT_MIN, y = 1\n");
        fflush(stdout);
        fun2(INT_MIN, 1);
    } else {
        printf("Corrected function:\n");
        fun2_fixed(INT_MIN, 1);   /* Same test that caused overflow. */
        fun2_fixed(INT_MAX, -1);  /* Also test the upper limit. */
        fun2_fixed(30, 20);       /* Ordinary safe subtraction: 10. */
        fun2_fixed(-20, 30);      /* Another safe subtraction: -50. */
    }
    return 0;
}
```

### Original function and chosen values

I chose `x = INT_MIN` and `y = 1`. Both inputs fit in an `int`, but subtracting 1 from the smallest `int` would produce a value below the allowed range. The original function performs this subtraction without checking first.

![Original subtraction function with overflow detected.](images/10.png)

Original function tried subtracting 1 from int_min the result would be below smallest allowed integer value. The sanitizer detected the overflow and stopped the program

### Fix and repeated tests

When `y` is positive, I check whether `x < INT_MIN + y`. When `y` is negative, I check whether `x > INT_MAX + y`. These checks happen before subtracting. The function prints an error if the result would not fit.

I repeated the original test with `INT_MIN` and 1. I also tested `INT_MAX` and negative 1 to check the upper limit. For normal inputs, I used 30 and 20, which should give 10, and negative 20 and 30, which should give negative 50.

![Corrected subtraction function with overflow and normal inputs.](images/11.png)

The fixed function prints an error for both inputs that would otherwise cause an overflow to occur. The tests worked correctly with 30 - 20 = 10, -20 - 30 = -50. Program finished without any errors.