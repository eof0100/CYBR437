# Lab 3 command guide

This guide is for doing the work. Submit the completed report PDF, not this guide. The report template deliberately leaves `<answer here>` placeholders. Replace every placeholder with your own observation and save your actual debugger screenshots. The supplied Part 2 diagram is already in images/2_1.png.

## Setup

Extract the ZIP. Put the instructor's lab3_p1 binary beside CYBR437_ArtemPeychev_Lab3.md. Open a terminal in that folder and run:

```bash
mkdir -p images
chmod u+x lab3_p1
pwndbg ./lab3_p1
```

If pwndbg is already configured to load when GDB starts, use `gdb ./lab3_p1` instead. Confirm the prompt says pwndbg. Inside the debugger run:

```gdb
set pagination off
set disassembly-flavor intel
```

Run the report's commands in question order in ONE debugger session. Do not restart between questions: the breakpoints and $base value are reused. The binary is PIE, so its runtime addresses can change. `$base` is calculated after `run` and makes the provided address commands work for this exact uploaded binary without guessing runtime addresses.

Take a screenshot after each question and save it as images/1.png through images/15.png. For long output, take multiple screenshots and add Markdown links such as `![Additional output](images/3_extra.png)` below that question. Do not shrink screenshots until text is unreadable.

## What each result should establish

1. `main` comes from **cybr437lab2main.c**. The other file, cybr437lab2.c, contains simpleCounter and simpleIf. Despite the filenames, this is the supplied Lab 3 binary.
2. Capture `checksec` inside pwndbg. Report the protections it actually lists. Static inspection shows PIE, a stack canary reference, a non-executable GNU_STACK, and GNU_RELRO with immediate binding; verify the displayed labels on your machine.
3. Calls in normal execution order: `puts` (first message), `simpleCounter(3)`, `printf` (input prompt), `__isoc99_scanf` (reads num), `simpleIf(num)`, `simpleCounter(num)`. The two user functions are simpleCounter and simpleIf. `__stack_chk_fail` is a compiler security failure path, not a normal user function call. Capture the full main disassembly, using extra screenshots if necessary.
4. Breakpoints are main, simpleCounter, and simpleIf. `run` should stop at main. The following `set $base ...` command calculates the binary's load base for later questions. It does not change the program.
5. Read the exact line shown by `info line *$pc`. Debug line records suggest line 9 at the main breakpoint. The first string is `This is a pwndbg program`.
6. In a fresh session, main is breakpoint 1. Verify before deleting: if its number differs, use the actual number. The guide uses `break cybr437lab2main.c:23` because you are stopped in main. Both source files contain line 23. If your instructor intends the counter's line 23 instead, use `break cybr437lab2.c:23`; that adds an extra stop inside the counter, so continue past it before expecting simpleIf in Question 11.
7. One `continue` should stop in simpleCounter with argument `num = 3`. `info args` supplies the parameter name and value.
8. `backtrace` shows frame #0 simpleCounter and frame #1 main. It displays the call chain, not stack memory contents.
9. There are **four output call sites: three printf calls and one putchar call**. There are two kinds of output function. One printf is inside a loop, so the number of times it executes is different from the number of call sites. Do not count scanf as output.
10. The constant referenced by simpleCounter is **VAR = 100**. Its address is load base + 0x2068, in **.rodata**, in a read-only mapping. `p/x &VAR` and `x/wd` show its address/value; `vmmap` shows mapping permissions and `info files` shows section boundaries. You do not need a source file for this.
11. Run `continue`, then type `15` when the PROGRAM asks `Enter a number`. It is program input, not a pwndbg command. You should stop in **simpleIf(num = 15)**. If the prompt is hidden in context output, enter 15 once after the debugger has resumed and is waiting.
12. The first literal in simpleIf is `Congrats you won`; with input 15, the other branch prints `Sorry you didn't win\n`. Show both literals to remove ambiguity. The global used in this function is **MAX**, declared `extern double` in this compilation unit and defined `const double` in the main compilation unit. Because the debug records disagree about const qualification, `whatis MAX` may show double or const double; report its actual output and explain the definition if needed. Its address is load base + 0x4020, in **.bss**, a writable mapping in this binary, despite the const-qualified definition. Read its actual value with `p MAX` rather than assuming a number.
13. There is a wording error: simpleIf compares **num** against **0xc, decimal 12**. It does not compare s. Say this directly and show the cmp instruction. With input 15, num differs from 12.
14. **s is a file-scope static int**, stored in initialized **.data**, with static storage duration and internal linkage. It begins at 1. The first simpleCounter(3) increments it three times, leaving 4. At simpleIf, `p s` should therefore show 4; after `continue`, the second simpleCounter entry also shows 4, proving it persists across calls. Keep this second counter breakpoint stopped for Question 15. A file-scope static differs from an automatic local variable and from a static local variable.
15. `telescope $rsp 64` displays 64 pointer-sized stack entries with pwndbg annotations. `x/64gx $rsp` provides a raw dump of 64 eight-byte values from the stack. Capture enough screenshots to include all 64 entries. These are memory displays, not backtraces.

The commands were checked against the uploaded binary's assembly, symbols, sections, and debug line records. They have not been executed here in pwndbg; use your live output as the evidence and send any unexpected stop/error immediately.

## Part 2

The included images/2_1.png is the stack/heap diagram based on the handout's printed addresses. It includes both frames, all array elements, all pointer variables, and all three heap allocations, ordered with high memory first. Types and initial values come from the code image. Unprinted frame bookkeeping addresses are not invented.

The pointer variable `word` itself is on the stack at 0x7fffdb770210; its allocated characters are on the heap at 0x55e8969572a0. The same distinction applies to b and c in foo. The diagram shows the state when addresses were printed, before c and b are reassigned later in foo. Add a short explanation in the Part 2 placeholder. You do not need to type, compile, or execute the C code in Part 2.

## Convert the completed report to PDF

Run these commands from the folder containing the Markdown and images. Complete all answers and screenshots first.

If Pandoc and your LaTeX engine already work:

```bash
pandoc CYBR437_ArtemPeychev_Lab3.md -o CYBR437_ArtemPeychev_Lab3.pdf -V geometry:margin=0.65in
```

If you get a missing LaTeX/package error, avoid spending the remaining time installing packages. Generate an HTML file with embedded screenshots:

```bash
pandoc CYBR437_ArtemPeychev_Lab3.md --standalone --embed-resources -o CYBR437_ArtemPeychev_Lab3.html
```

Open the HTML in Firefox or Chrome, press Ctrl+P, choose Save to PDF, and save as CYBR437_ArtemPeychev_Lab3.pdf. This path uses the browser's PDF printer and does not need LaTeX. Alternatively, convert to DOCX with Pandoc, open in Word or LibreOffice, and export to PDF.

## Before submission

- Replace every `<answer here>`.
- Verify every actual command shown matches what you ran. Remove any instruction comments you do not want in the report.
- Open the FINAL PDF and check every screenshot and the diagram is present and readable. A screenshot working in Markdown does not guarantee it appears in the PDF.
- Check Question 3 includes all main instructions and Question 15 includes all 64 entries, using extra screenshots when needed.
- Submit **one PDF file in Canvas**. The handout does not ask for a ZIP, tar.gz, Markdown file, source code, or the binary.
