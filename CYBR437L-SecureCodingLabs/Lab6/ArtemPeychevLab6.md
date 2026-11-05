Author: Artem Peychev
Class: CYBR437 Secure Coding
Due: November 4th, 2026


## Lab 6 - Buffer Overflow 



After reviewing Task1.c code I compiled it using the following command:

```bash
cc Task1.c -o Task1 -fno-stack-protector -no-pie -m32 -Wno-error=implicit-function-declaration
```

![GCC Compilation](images/task1_compiled.png)

After compilation, I used the file command to determine the executable's endianess. The output shows that the file is LSB or Least Signifiant Byte first meaning it uses little endian ordering.

![Endianess of Executable](images/task1_endianess.png)


Next I used checksec to make sure stack canary protection is disabled before moving on to diassembling using pwndbg. 

![GCC Compilation](images/task1_checksec.png)





