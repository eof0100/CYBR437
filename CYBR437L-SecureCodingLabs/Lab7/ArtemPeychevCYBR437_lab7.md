Artem Peychev  
CYBR437: Secure Coding  
Due: 11/11/2026


## Lab 7: BufferOverflows using ROP 



### Task 1 

I began by using the `file` command allowing me to view task1's binary format information. The task1 executable is a 32 bit ELF executable compiled for x86 systems. It uses LSB or Least Significant Byte first also known as little-endian which lets me know to use little-endian when performing a bufferoverflow. 

Next, I checked what security mechanisms are in place for task1 executable using the `checksec` command. The Stack Canary and PIE are disabled, NX is enabled, and RELRO is partially enabled. No FORTIFY protected was detected.

![File and Checksec Output](images/file_checksec_task1.png)


After running pwndbg, I wanted to what functions are inside task1. I discovered some interesting functions.

```bash
0x08049196  breach_firewall
0x080491ca  access_mainframe
0x08049214  login
0x0804927c  main
```

![Command: info functions](images/1_info_functions.png)

First started to look inside main(). I can see right away that the caller main is calling the callee function login(). But I also noticed before login is called there is a puts@plt call that is used to print a string. To find out what is it printing, I set a breakpoint `break *0x0804929f`, the instruction just before `call puts`. 

```
break *0x0804929f
run
```

![Command: disass main](images/2_disass_main.png)

Nothing too interesting just a message being printed that said, `Welcome to Task 1`.

![Command: disass main](images/3_puts_msg.png)


Since main is calling login(), I decided to `disass login` whch revealed the following flow: after printing "Enter credentials" with puts, the program calls scanf at <+50> where the buffer address is computed using `lea eax, [ebp-0x48]` at <+39>. The %s which has no field width checking, a classic buffer overflow. Since the buffer begins at [ebp-0x48] or 72 bytes and the EIP sits at [ebp+4] that means the payload for the overflow would be 76 bytes (0x48 for buffer plus 4 bytes for saved EBP) which is enough to overwrite the return address and gain control of the EIP when login finishes executing.

Inide login, the scanf call uses a buffer located at `[ebp-0x48]` and since %s has no limit width this allows it to be vulnerable to a buffer overflow. The buffer starts exactly 72 bytes `[ebp-0x48]` below the EBP register (see screenshot below). EBP takes 4 bytes at `[ebp]` and the return address EIP sits right above it at `[ebp+4]. In order to take advantage of the buffer overflow and highjack the control flow of the program I need to overwrite the EIP which needs exactly 76 bytes (72 bytes for the buffer and 4 bytes for the EBP). So whatever address I write after those 76 bytes will overwrite the EIP and will be executed after login hits ret.

![Command: disass login](images/4_disass_login.png)

![Reminder: info functions](images/5_info_functions.png)

function so I can overwrite the EIP with access_mainframe's memory address (see screenshot above) which is `0x080491ca`.

Using access_mainframe's memory address (see screenshot above), I wrote a python script using pwntools to send 76 bytes of padding followed by p32(0x080491ca) to the program in order to overwrite the EIP. When login() returns it will jump to access_mainframe function.

Note: p32(0x0491ca) sends the address in little-endian as raw bytes


Overwriting the EIP worked but unfortunately, I got a message saying access denied and that the firewall is still active. Next I decided to `disass access_mainframe`. and see what is going on inside.


![Command: disass access_mainframe](images/6_disass_access_mainframe.png)

The most important logic here is the firewall check from <+17> to <+25>. The instruction `mov edx, DWORD PTR [eax+0x28]` reads the global variable perhaps called firewall_active or something similiar and puts it into EDX register. The next line `test edx, edx` tests if contents inside EDX == 0 which means firewall is off jumps to <+27> to <+47> otherwise the failure path is <+49> to <+66>. 

If EDX is zero the following instrctions will execute:

```bash
lea edx,[eax-0x1fe4]           
push edx
call puts@plt                
jmp 0x804920f                 
```

Load a message most likely a flag or access granted or something similiar then it will print it out and finally jumps to the end of the function at address 0x804920f.

Now that I know what access_mainframe needs in order to print out the success message, I am going to `disass breach_filewall` and see if I can turn off the global variable for the firewall.

![Command: disass breach_firewall](images/6_disass_access_mainframe.png)


After analyzing breach_firewall, at <+36> it executes `mov DWORD PTR [ebx+0x28], 0x1` which writes the value on to the global variable that controls the firewall and that same check is inside access_mainframe at <+17>.

The interesting part is calling breach_firewall will actually turn on the global variable so calling it will not do me any good. But jumping directly to access_mainframe it still fails the check and prints "Access Denied" because the global variable that controls the firewall has its flag already set to 1. I would guess that when the global variable was created it was initialized to 1.

So overwriting the EIP with access_mainframe's memory address will not work so to get this to work what I need to do is use ROP chain to jump directly to the middle of access_mainframe at <+27> after the check condition granting access.