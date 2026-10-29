Artem Peychev  
CYBR437 Secure Coding  
Lab 5

# Part 1: Assembly to C

I used no optimization when compiling so I could compare the assembly with the handout. Part 1 uses the same setup without a stack canary as the examples. Part 3 was compiled separately with stack protection turned on.

## Task 1

### Code

```c
int main(void) {
    // First movl puts 25 in a.
    int a = 25;
    // Next movl puts 30 in b.
    int b = 30;
    // movq puts 45 in c. Long uses 8 bytes on this system.
    long c = 45;

    // add adds a and b. cltq makes the result 64 bits.
    // sub takes away the old c, then mov saves the result in c.
    c = a + b - c;
    // The last mov puts 0 in eax for the return value.
    return 0;
}
```

### What it does

First movl stores 25 in a. In the assembly this number is 0x19. It goes at rbp minus 4. The next movl stores 30 in b which is 0x1e, at rbp minus 8. Then movq stores 45 in c which is 0x2d, at rbp minus 16.

I used int for a and b because movl stores 4 bytes. I used long for c because movq stores 8 bytes and long has that size on this system.

Then mov loads a into edx and b into eax. The add instruction adds them so now the result is 55. Next cltq takes the signed 32 bit result in eax and makes it a signed 64 bit result in rax. This lets it work with the long variable.

The sub instruction takes away the old c value of 45. So 55 minus 45 gives 10. The next mov saves 10 back in c. This is what the line c equals a plus b minus c does.

The last mov puts 0 in eax for return 0. Push saves the old rbp and mov sets up the new stack frame. Pop restores rbp and ret returns from the function. Endbr64 is a protection instruction for jumps into the function.

### Screenshot

![Assembly for task 1](images/1.png)

The assembly has the same numbers, stack positions, add, cltq and sub as the handout. It also saves the result back in c.

## Task 2

### Code

```c
int main(void) {
    // The first two movl instructions store these numbers.
    int a = 25;
    int b = 30;
    int result;

    // mov loads a, then cmp compares a with b.
    // jle goes to the else part if a is less than or equal to b.
    if (a > b) {
        // movl stores 1 here, then jmp skips the else part.
        result = 1;
    } else {
        // This is where jle goes. movl stores 0.
        result = 0;
    }
    // Put 0 in eax and finish main.
    return 0;
}
```

### What it does

The first two movl instructions store 25 and 30 in a and b. They use the stack positions rbp minus 4 and rbp minus 8. Then mov loads a into eax and cmp compares it with b.

Jle means jump if less than or equal. So if a is less than or equal to b it goes to the part that stores 0 in result. Result is at rbp minus 12.

If a is bigger than b it does not take that jump. It stores 1 in result instead. Then jmp skips the part that stores 0. So this is an if and else in C. For these numbers 25 is smaller than 30, so result becomes 0.

The last mov puts 0 in eax because main returns 0. Push, mov, pop and ret handle starting and ending the function. Endbr64 is there for control flow protection.

### Screenshot

![Assembly for task 2](images/2.png)

The assembly has the same cmp, jle and jmp instructions as the handout. It also has the two choices that store either 1 or 0.

## Task 3

### Code

```c
int fun(int a, int b) {

    // edi and esi bring the two numbers into this function.
    // mov saves them on the stack, then loads a into edx and b into eax.
    // add adds them, then sub takes away 1.
    // eax holds the number that goes back to main.
    return a + b - 1;
}

int main(void) {
    // The two movl instructions store 5 and 6.
    int a = 5;
    int b = 6;

    // mov puts a in edi and b in esi, then call runs fun.
    // After fun returns, mov saves the number from eax in a.
    a = fun(a, b);
    // Main returns 0.
    return 0;
}
```

### What it does

In main, sub takes 16 bytes from rsp to make room on the stack. The two movl instructions store 5 in a and 6 in b. Then mov loads b into edx and a into eax. The next two mov instructions put b in esi and a in edi. These registers hold the numbers being passed to fun.

The call instruction runs fun. Inside fun, mov saves the first number from edi at rbp minus 4 and the second number from esi at rbp minus 8. It loads a into edx and b into eax. Then add adds the numbers and sub takes away 1.

So fun does 5 plus 6 minus 1 and returns 10. It leaves this result in eax. Back in main, mov saves that result in a at rbp minus 4.

The next mov puts 0 in eax because main returns 0. Leave restores the stack frame and ret goes back to the caller. The push and mov at the start set up the frame, and endbr64 is for control flow protection.

### Screenshot

![Assembly for main and fun](images/3.png)

The screenshot shows main passing the numbers to fun, then storing its result. Fun has the same add and subtract 1 instructions as the handout.

# Part 2: Memory management

I looked at these two tasks without running the code.

## Task 1

The function uses malloc to get 32 bytes but never frees that memory. When the function ends it loses the pointer, so this causes a memory leak.

To fix it I would free text_buffer before return 0. There is already a NULL check, so if malloc fails it returns negative 1 without using the pointer.

## Task 2

The second printf uses ptr after the memory was already freed. This is called use after free. Even if it prints 10 the memory is no longer safe to use.

To fix this I would remove the second printf or move it before free. I would also check if malloc returned NULL before putting 10 into the memory that ptr points to.

I could set ptr to NULL after freeing it so it does not keep the old address. But setting it to NULL still does not make it safe to read through that pointer.

# Part 3: Buffer overflows

I kept stack protection on for these programs. I also used AddressSanitizer for the array copy so it could show where the program writes outside the array.

## Task 1: City input

### Original code

```c
#include <stdio.h>
int main(void) {
    // Only 10 bytes are available here.
    char city[10];
    printf("Enter city: ");

    // This keeps reading even when the input will not fit.
    scanf("%s", city);
    printf("City: %s\n", city);
    return 0;
}
```

### What is wrong

City only has 10 bytes. It needs one of those bytes for the zero at the end of the string, so only 9 characters fit. Scanf has no limit on how many characters it reads here. So a long name goes past the end of the array.

I used normal_city.txt with Spokane and long_city.txt with a much longer name. I used these same files for both versions.

### Before the fix

![City before the fix](images/4.png)

The short city printed correctly. The long input caused a stack smashing detected error and the program stopped. It was too long for the city array.

### Fixed code

```c
// Makes getline available on this Linux system.
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int main(void) {
    char *city = NULL;
    size_t capacity = 0;
    printf("Enter city: ");

    // getline makes space and adds more when needed.
    ssize_t length = getline(&city, &capacity, stdin);
    // Stop if the input could not be read.
    if (length < 0) {
        fprintf(stderr, "Could not read city.\n");
        free(city);
        return 1;
    }
    // Remove the Enter key newline at the end of the input.
    if (length > 0 && city[length - 1] == '\n') {
        city[length - 1] = '\0';
    }
    printf("City: %s\n", city);
    // Free the memory after printing the city.
    free(city);
    return 0;
}
```

### What I changed

I used getline instead of the small city array and scanf. Getline makes space for the input and adds more space if it needs it. So the whole name fits even if it is long, and the program does not have to reject it.

I also remove the newline at the end and free the memory after printing. If getline cannot read the input the program frees any allocated memory and stops.

### After the fix

![City after the fix](images/5.png)

Both short and long city names printed this time without crashing. The buffer could grow so the whole input fit.

## Task 2: Copying an array

### Original code

```c
#include <stdio.h>

int fun(double *to, double *from, int pos, int n) {
    int k;
    for (k = 0; k < n; k++) {
        // This does not check if the next value will fit.
        to[pos] = from[k];
        pos++;
    }
    return pos;
}

int main(void) {
    // Destination has 5 spaces and source has 6 numbers.
    double to[5] = {0};
    double from[6] = {10, 20, 30, 40, 50, 60};
    printf("Destination capacity: 5; start: 3; number to copy: 4\n");
    printf("This tries to write destination indexes 3, 4, 5, and 6.\n");
    fflush(stdout);
    // Copy 4 numbers starting at index 3. This goes past the end.
    int end = fun(to, from, 3, 4);
    printf("Returned position: %d\n", end);
    for (int k = 0; k < 5; k++) printf("%.0f ", to[k]);
    printf("\n");
    return 0;
}
```

### What is wrong

The function does not know how much space the destination has. My destination has 5 elements, so its indexes go from 0 to 4. I tried copying 4 values starting at index 3. That tries to write at indexes 3, 4, 5 and 6, but 5 and 6 are outside the array.

The source has 6 elements which is more than the 4 being copied, so the source is large enough for this test.

### Before the fix

![Array copy before the fix](images/6.png)

AddressSanitizer found a write outside the destination array. It points to line 7 in fun where the value is copied. The program stopped when the overflow was found.

### Fixed code

```c
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

// Passing the pointer by address lets this function update it.
// Capacity tells us how many numbers fit in the destination.
int fun(double **to, const double *from, int pos, int n, size_t *capacity) {

    // Check the positions and make sure adding the sizes will fit.
    if (pos < 0 || n < 0 || n > INT_MAX - pos) return -1;
    size_t needed = (size_t)pos + (size_t)n;
    // Make sure the size in bytes will fit too.
    if (needed > SIZE_MAX / sizeof(double)) return -1;

    // Make more room if the current array is too small.
    if (needed > *capacity) {

        // Keep the old pointer in case realloc fails.
        double *larger = realloc(*to, needed * sizeof(double));
        if (larger == NULL) return -1;
        // Set the new spaces to zero.
        for (size_t k = *capacity; k < needed; k++) larger[k] = 0;
        // Give the updated pointer and size back to main.
        *to = larger;
        *capacity = needed;
    }
    // Now all the numbers fit, so copy them.
    for (int k = 0; k < n; k++) {
        (*to)[pos] = from[k];
        pos++;
    }
    return pos;
}

int main(void) {
    size_t capacity = 5;
    // Use allocated memory so realloc can grow it later.
    double *to = calloc(capacity, sizeof(double));
    double from[6] = {10, 20, 30, 40, 50, 60};
    if (to == NULL) return 1;
    printf("Original capacity: %zu; start: 3; number to copy: 4\n", capacity);
    // Use the same starting position and copy count as before.
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
    // Free the array when finished.
    free(to);
    return 0;
}
```

### What I changed

I changed the function so it knows how many elements fit in the destination. If there is not enough room it uses realloc to grow the array. So it can copy all the values instead of rejecting the input.

I used calloc in main because realloc needs allocated memory. It cannot grow the original stack array. I pass the pointer by address so the function can update it if realloc moves the array.

Before making more room I check that the positions are not negative and that the size calculation will fit. I also check the size in bytes. I use a temporary pointer for realloc so I do not lose the old memory if it fails. If there is no memory available the function returns an error. At the end main frees the array.

I used the same 6 source values, starting index 3, copy count 4 and original destination size 5 for this test.

### After the fix

![Array copy after the fix](images/7.png)

The fixed program increased the array size from 5 to 7. It copied all four values and returned position 7. The array had 0 0 0 10 20 30 40 and there was no overflow error.

# Part 4: Integer overflows

I used INT_MIN and INT_MAX from limits.h to get the smallest and biggest int values for this system. I did not write those limit values into the code myself.

I used UndefinedBehaviorSanitizer to check for overflow in the original functions. Signed overflow is undefined in C, so I cannot rely on it just wrapping around to another number.

## Task 1: Addition

### Code

```c
#include <stdio.h>
#include <limits.h>
#include <string.h>

void fun1(int a, int b) {
    int sum;
    // Original function adds the numbers without checking.
    sum = a + b;
    printf("Result = %d\n", sum);
}

void fun1_fixed(int a, int b) {
    printf("a = %d, b = %d: ", a, b);

    // Positive b can make the sum too big.
    // Negative b can make it too small. Check before adding.
    if ((b > 0 && a > INT_MAX - b) ||
        (b < 0 && a < INT_MIN - b)) {
        printf("Error: integer overflow.\n");
        return;
    }
    // The result fits, so it is safe to add now.
    printf("Result = %d\n", a + b);
}

int main(int argc, char **argv) {
    printf("INT_MIN = %d; INT_MAX = %d\n", INT_MIN, INT_MAX);
    // The unsafe argument runs the original function.
    if (argc > 1 && strcmp(argv[1], "unsafe") == 0) {
        printf("Original function: a = INT_MAX, b = 1\n");
        fflush(stdout);
        // Both numbers fit on their own, but their sum does not.
        fun1(INT_MAX, 1);
    } else {
        printf("Corrected function:\n");
        // Try the same overflow test with the fixed function.
        fun1_fixed(INT_MAX, 1);
        // Check what happens below the smallest int.
        fun1_fixed(INT_MIN, -1);
        // These two additions should work normally.
        fun1_fixed(20, 30);
        fun1_fixed(-20, -30);
    }
    return 0;
}
```

### Original function

I chose a as INT_MAX and b as 1. Both numbers fit in an int by themselves. But INT_MAX is already the biggest int, so adding 1 makes the result too big.

The original function just adds without checking. Running it with the unsafe argument tests that addition. The sanitizer reports signed integer overflow and stops the program.

![Addition test](images/8.png)

### What I changed

If b is positive I check whether a is bigger than INT_MAX minus b. If b is negative I check whether a is smaller than INT_MIN minus b. I do this before adding so the check does not wait until after the overflow happened.

If the result would not fit it prints an error and returns. Otherwise it adds the numbers and prints the answer.

### After the fix

![Addition after the fix](images/9.png)

The fixed function printed errors for INT_MAX plus 1 and INT_MIN plus negative 1. The normal tests worked. Adding 20 and 30 gave 50, and adding negative 20 and negative 30 gave negative 50.

## Task 2: Subtraction

### Code

```c
#include <stdio.h>
#include <limits.h>
#include <string.h>

void fun2(int x, int y) {
    int diff;
    // Original function subtracts without checking first.
    diff = x - y;
    printf("Result = %d\n", diff);
}

void fun2_fixed(int x, int y) {
    printf("x = %d, y = %d: ", x, y);

    // Subtracting a positive number can go below INT_MIN.
    // Subtracting a negative number can go above INT_MAX.
    if ((y > 0 && x < INT_MIN + y) ||
        (y < 0 && x > INT_MAX + y)) {
        printf("Error: integer overflow.\n");
        return;
    }
    // The result fits, so subtract the numbers now.
    printf("Result = %d\n", x - y);
}

int main(int argc, char **argv) {
    printf("INT_MIN = %d; INT_MAX = %d\n", INT_MIN, INT_MAX);
    // The unsafe argument runs the original function.
    if (argc > 1 && strcmp(argv[1], "unsafe") == 0) {
        printf("Original function: x = INT_MIN, y = 1\n");
        fflush(stdout);
        // Taking 1 away from INT_MIN goes below the limit.
        fun2(INT_MIN, 1);
    } else {
        printf("Corrected function:\n");
        // Try the same overflow test with the fixed function.
        fun2_fixed(INT_MIN, 1);
        // Taking away negative 1 also checks the upper limit.
        fun2_fixed(INT_MAX, -1);
        // These two subtractions should work normally.
        fun2_fixed(30, 20);
        fun2_fixed(-20, 30);
    }
    return 0;
}
```

### Original function

I chose x as INT_MIN and y as 1. Both fit in an int by themselves. But INT_MIN is already the smallest int, so subtracting 1 would go below the limit.

![Subtraction before the fix](images/10.png)

The original function tried subtracting 1 from INT_MIN. The sanitizer found signed integer overflow and stopped the program.

### What I changed

If y is positive I check whether x is smaller than INT_MIN plus y. If y is negative I check whether x is bigger than INT_MAX plus y. These checks happen before subtracting anything.

If the result would not fit the function prints an error. Otherwise it subtracts and prints the answer.

### After the fix

![Subtraction after the fix](images/11.png)

The fixed function printed errors for INT_MIN minus 1 and INT_MAX minus negative 1. The normal tests worked. Taking 20 away from 30 gave 10. Taking 30 away from negative 20 gave negative 50. The program finished without a sanitizer error.
