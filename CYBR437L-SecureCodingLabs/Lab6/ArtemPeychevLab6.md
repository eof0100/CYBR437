Author: Artem Peychev  
Class: CYBR437 Secure Coding  
Due: November 4th, 2026  


## Lab 6 - Buffer Overflow 



### Task 1


After reviewing Task1.c code I compiled it using the following command:

```bash
cc Task1.c -o Task1 -fno-stack-protector -no-pie -m32 -Wno-error=implicit-function-declaration
```

![GCC Compilation](images/task1_compiled.png)

After compilation, I used the file command to determine the executable's endianess. The output shows that the file is LSB or Least Signifiant Byte first meaning it uses little endian ordering.

![Endianess of Executable](images/task1_endianess.png)


Next I used checksec to make sure stack canary protection is disabled before moving on to diassembling using pwndbg. 

![GCC Compilation](images/task1_checksec.png)


Then I ran pwndbg, using break man command then use ni stands for next instruction to execute the next set of instructions. The instruction `mov ptr [ebp-0xc], 0` indicates a 4 byte local variable is located at that offset 0xc (12 bytes) below the EBP address. I calculated the address using EBP = 0xffffcb78 and subtracted the 12 bytes which gave me a memory address of 0xffffcb6c and that memory address was given value of zero stored in 4 bytes or 00 00 00 00.

![EBP Located at offset 0xc (12 bytes)](images/task1_1.png)


The lea stands for load effective address instruction calculates the memop address located 0x16 (22 bytes) below the EBP and stores that address 0xffffcb62 in EAX register. This address is then passed to gets() function which tells me that is where the buffer starts. After push eax line the call gets which calls the get() function which takes the address of character buffer where it stores user input. So the compuler passses memory address of 0xffffcb62 to gets() which is where the buffer starts.


![22 byte offset from EBP](images/task1_2_lea_eax.png)


In summary I found `lea eax, [ebp-0x16]` which is 22 bytes offset from EBP before that I found `move dword ptr [ebp-0xc, 0]` which is 12 bytes offset from EBP comparing the two offsets 0x16-0x0c = 0x0a (10).

To verify this finding, I set a breakpoint immediately after the gets() function returned and supplied a cyclic pattern as input. Inspecting memory revealed that the cyclic pattern started at address which was 0xffffcb62, confirming the beginning of the buffer. The pattern continued past address 0xffffcb6c, which is the location of the target variable. This confirmed that input exceeding the buffer's capacity overwrites the adjacent variable stored at [ebp-0xc].

Providing more than 10 bytes of input, the buffer overflow allows user-controlled data to modify the target variable and influence the program's execution flow.


![22 byte offset from EBP](images/task1_3.png)

After finding that the 10-byte buffer could be overflowed to reach the target variable, I changed the payload in exploit.py to send 11 bytes of input. The script sent this input to the vulnerable gets() function. The extra byte went past the buffer and changed the nearby variable at ebp-0xc on the stack. After saving the file, I ran python3 exploit.py. The overflow worked, and the program revealed the secret message and flag.

![Python Exploit Code](images/python_script.png)




### Task 2

First, I compiled the C code for Task 2, checked the file properties, and checked what security was enabled using checksec.


![Task 2 Compiled Code](images/task2_gcc.png)
![file and checksec output](images/task2_2.png)


After launching the program in pwndbg and setting a breakpoint at the beginning of the verifyLaunchKey() function, I stepped through the instructions using the ni (next instruction) command. During the disassembly, I identified the instruction:

```bash
mov dword ptr [ebp-0xc], 0x87654321
```

This instruction initializes the launchKey variable with the value 0x87654321. Since EBP was 0xffffcb78, the variable was located at memory address 0xffffcb6c (0xffffcb78 - 0x0c).

![launchKey var location](images/task2_3.png)


Then going through the assembly I found the instructions
lea eax, ebp-0x25, push eax, call gets@plt.These instructions pass the address of the input buffer to get. Using the value of EBP, the buffer address was calculated as 0xffffcb53. Comparing the buffer offset 0x25 and the launchkey offset 0x0c shows a difference of 25 bytes. So the input longer than 25 bytes will begin overwriting the launchKey variable.


![Buffer location gets() call](images/task2_4.png)

![file and checksec output](images/task2_5.png)

To verify the overflow, I set a breakpoint immediately after the gets() function returned and supplied a cyclic pattern as input. I then examined memory beginning at the buffer address 0xffffcb53. The output showed the cyclic pattern stored in memory starting at the buffer location. Since the pattern extended beyond 25 bytes  and into the memory  containing the launchKey variable, I confirmed that input longer than 25 bytes can overwrite launchKey and modify its value.

![Cyclic pattern verified](images/task2_6.png)

After determining that the launchKey variable could be overwritten through the buffer overflow, I modified the payload in exploit.py to send 25 bytes of padding followed by value required by the programs validation check. What the payload does it fill the vuffer then overwrite launchKey var stored at ebp-0xc. Saved the exploit script and ran it. Success.

![Exploit code and exploitation](images/task2_7.png)


## Task 3

Same as before I compiled the program, used file, and checksec to verify security controls. The screenshot shows this below.


![Compile, File, and Checksec](images/task3_1.png)


After setting a breakpoint at the beginning of the findArtifact func I stepped through the program using the ni (next instruction) command. This allowed me to analyze the stack layout and determine how the buffer was stored in memory before user input was processed.

![Breakpoint findArtifact](images/task3_breakpoint.png)

Continuing dissassembly I identifed lea eax, [ebp-0x1c], push eax, call scanf@plt. These instructions pass address of the input buffer to scanf() function. Using the current value of EBP which is at 0xffffcb78 the buffer was calculated as 0xffffcb5c confirming the location of the input buffer that will later I will use to overwrite the return address.


![EIP crash](images/task3_eip_crash.png)

To determine the offset required to overwrite the return address, I supplied a cyclic pattern as input and allowed the program to crash. After the crash, I inspected the registers using:


![Registers](images/task3_registers.png)


The output showed that the value of EIP had been overwritten with 0x61616169, confirming that the buffer overflow successfully reached and overwrote the return address. This demonstrated that execution flow could be redirected by controlling the value placed into EIP.

![Registers](images/task3_registers_offset.png)

![Artifact](images/task3_artifact.png)


After determining that 32 bytes were required to reach the return address, I used the info functions command to locate the unveilArtifact function. The output showed that unveilArtifact was located at address 0x080491c6. Since this function reveals the artifact flag, the goal was to overwrite the return address with this functions address.

The modified payload successfully overwrote the return address and redirected execution to unveilArtifact. Then the program executed the artifactfunction and revealed the flag. Success.


![Exploit Code and Exploitation](images/task3_success.png)


## Task 4

First I begin by compiling Task4.c into executable.

![GCC Compilation](images/4_gcc.png)

Using the file command I checked the executables properties and it appears to be an ELF 32 bit LSB little endianess byte ordering.

![File Output](images/4_file.png)

Using checksec command to verify the enabled/disabled protections before I begin analyzing task4 using pwndbg. 


![Security Controls](images/4_checksec.png)


After setting a breakpoint at the beginning of the initiate_hack() function, stepping through the program using the ni (next instruction) command. This allowed me to observe the function stack setup performed by the compiler before local variables and user input were even processed. By the way I discovered the function using the info functions command (screenshot below).

![Info Functions - initiate_hack](images/4_buffer.png)
![Breakpoint InitiateHack()](images/4_breakpoint_initiate_hack.png)



Continuing through the disasembling, I identified the instruction `lea eax, [ebp-0x28]`. This instruction calculates the address of the input buffer and then it will store it inside eax register. The current EBP which is 0xffffcb78, I calculated the buffer address as 0xffffcb50. The instructions pass the address to scanf() function which confirms the location of the 32 byte input buffer used by the program (char buffer[32] according to the C code). 

To determine if the return address can be overwritten, what I did is supply a cyclic pattern as input and had the program crash. After the crash I inspected the registers using `info registers` command.

![Cyclic Pattern](images/4_cyclic_pattern.png)
![Registers after cyclic pattern](images/4_registers_after_cyclic.png)

The output showed that the EIP register with value of 0x6161616c which confirms that the buffer overflow did successfully reach the return address. This demonstrated that the execution flow could be controled through this overflow.

To determine the exact offset required overwrite the return address, I used the command `cyclic -l 0x6161616c`. The output shows that cyclic value was found at offset 44 (see screenshot below). This confirms that 44 bytes of input is required before the buffer overflow beings to overwrite the return address stored at EIP. This offset was used when I edited exploit.py.


![Offset 4 bytes](images/4_cyclic_offset.png)


After determining the offset required to overwrite the return address, again I used `info functions`command with `p access_vault` and `info address access_vault` to locate the address of the access_fault function. Source code of the function shows it is responsible for validating parameters that are supplied and then reveals the flag when the correct values are present. So the goal of the exploit here is to redirect the execution to this function after the return address is overwritten.

![Offset 4 bytes](images/4_access_vault.png)

So then I verified access_vault located at address 0x80491c6 which is responsible for validating the supplied params and then revealing the flag.

Knowing the buffer location, return address offset which is 44 bytes, and address of access_vault, I modified the exploit.py payload to overflow the buffer then overwrite the return address redirecting the execution to the function that I want which is access_vault() placing the required values on the stack os the validation checks inside the function succeed.
ss
Screenshot below shows the correct payload and successful exploitation of the overflow granting access and reading the flag.


![Exploit Code and Exploitation](images/4_exploit_success.png)

