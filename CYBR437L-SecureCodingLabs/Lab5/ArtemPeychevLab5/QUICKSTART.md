# Quick guide for Artem

Use your Ubuntu VM or another Linux x86 64 environment. The assembly examples use the Linux x86 64 calling convention. Run the commands below inside this extracted folder. The writeup already includes the source listings and explanation drafts. Read them, check them against your outputs, and fill every bold bracketed observation before submitting.

Do not put screenshots of someone else's results in your report. Capture your own terminal after running each group.

## Suggested hour

* First 5 minutes: extract, open the Markdown file, and build.
* Next 10 minutes: Part 1 comparisons and screenshots 1 through 3.
* Next 5 minutes: read and check the Part 2 explanations. Do not run those programs.
* Next 15 minutes: Part 3, screenshots 4 through 7.
* Next 10 minutes: Part 4, screenshots 8 through 11.
* Last 15 minutes: fill observations, convert to PDF, check images, and submit the source files too.

## 1. Open the folder and build

Extract the downloaded ZIP. Open the extracted `ArtemPeychevLab5` folder in your file manager, right click in the folder, and choose Open in Terminal. Open `ArtemPeychevLab5.md` in your editor.

```bash
pwd
ls
bash build.sh
```

`pwd` shows your current directory. `ls` should show `build.sh` and the C files. The script compiles all nine programs into `bin`. Warnings about an unused variable in Part 1 are expected because the handout stores values without printing them. Errors are different: stop and share any compiler error.

If `gcc` or `objdump` is missing:

```bash
sudo apt update
sudo apt install build-essential binutils
bash build.sh
```

The script uses `-O0` so the compiler keeps the simple instructions. It uses `-g` for source line information. Part 1 alone uses `-fno-stack-protector` to match the supplied assembly, which has no canary. The requirement to keep stack protection applies to Part 3, and every Part 3 build explicitly uses `-fstack-protector-all`.

## 2. Part 1: Compare the compiled assembly

The programs deliberately print nothing, like the assembly in the handout. Do not add printf to these three programs just to get output, because that changes their disassembly. Instead inspect their assembly.

### Screenshot 1

```bash
objdump -d --disassemble=main bin/p1_task1
```

Capture the command and main's assembly. Save the image as `images/1.png`. Look for 0x19, 0x1e, 0x2d, add, cltq, sub, and the store back to the long variable. The calculation is 25 + 30 - 45 = 10.

### Screenshot 2

```bash
objdump -d --disassemble=main bin/p1_task2
```

Save `images/2.png`. Include cmp, jle, the store of 1, jmp, and the store of 0. Because 25 is not greater than 30, the third variable becomes 0.

### Screenshot 3

```bash
objdump -d --disassemble=main bin/p1_task3
objdump -d --disassemble=fun bin/p1_task3
```

Save `images/3.png`. Show both main and fun if possible by making the terminal taller or reducing its font size. Main passes 5 and 6. Fun adds them and subtracts 1, returning 10, and main saves that result.

Fill the three assembly comparison placeholders in your Markdown file using what you actually see. Addresses differ between binaries. Compiler versions can also change instruction layout, so do not claim an exact match unless you checked it. The source comments and report explain the supplied instructions individually. I checked these conversions with GCC 13: with the provided Part 1 flags, the instructions and local variable offsets match the handout; your compiler version can still differ.

## 3. Part 2: Written explanations only

Read the two Part 2 explanations in the template. Task 1 is a memory leak. Task 2 is a use after free and also needs a malloc failure check. No screenshots or extra C files are required for Part 2. Do not run the supplied Part 2 programs.






## 4. Part 3, Task 1: City overflow

### Screenshot 4: Original version

```bash
wc -L normal_city.txt long_city.txt
./bin/p3_city_vulnerable < normal_city.txt
./bin/p3_city_vulnerable < long_city.txt
```

Save `images/4.png`, showing the short input working and the long input causing a stack protection error. The files contain a 7 character city and an 80 character word. A message like `stack smashing detected` is an expected demonstration, not a reason to turn off the canary. The exact crash wording can differ.

If a desktop crash reporting dialog appears, close the dialog and capture the terminal output.

### Screenshot 5: Fixed version, SAME input files

```bash
./bin/p3_city_fixed < normal_city.txt
./bin/p3_city_fixed < long_city.txt
```

Save `images/5.png`, showing both full outputs with no stack protection error. Fill the two Task 1 observation placeholders.

## 5. Part 3, Task 2: Array overflow

### Screenshot 6: Original version

```bash
./bin/p3_copy_vulnerable
```

Save `images/6.png`. Capture the test description and the first part of the AddressSanitizer report showing `stack-buffer-overflow`, the invalid write, and the function/source line. You do not need every line of the long diagnostic. Stack protection is still enabled.

### Screenshot 7: Fixed version, SAME test values

```bash
./bin/p3_copy_fixed
```

Save `images/7.png`. Expected successful output includes:

```text
New capacity: 7; returned position: 7
Destination: 0 0 0 10 20 30 40
```

The fix grows the destination rather than discarding the source values. Fill the two Task 2 observation placeholders.

If the sanitizer complains specifically about LeakSanitizer or ptrace, run the test from a normal terminal outside GDB. If that environment limitation persists, use `ASAN_OPTIONS=detect_leaks=0 ./bin/p3_copy_fixed`. This only skips the leak scan; array bounds detection and the stack canary stay enabled.

## 6. Part 4, Task 1: Addition

### Screenshot 8: Original version

```bash
./bin/p4_addition unsafe
```

Save `images/8.png`. Show INT_MIN, INT_MAX, the chosen inputs, and the sanitizer's signed integer overflow message. The program intentionally stops at the invalid addition. Both input values individually fit in int.

### Screenshot 9: Fixed version

```bash
./bin/p4_addition
```

Save `images/9.png`. The two limit tests should print errors. The two normal tests should print 50 and negative 50. Fill the two addition observation placeholders.

## 7. Part 4, Task 2: Subtraction

### Screenshot 10: Original version

```bash
./bin/p4_subtraction unsafe
```

Save `images/10.png`. Capture the selected arguments and the signed integer overflow message. The program intentionally stops before printing an invalid result.

### Screenshot 11: Fixed version

```bash
./bin/p4_subtraction
```

Save `images/11.png`. The limit tests should print errors. The normal tests should print 10 and negative 50. Fill the two subtraction observation placeholders.

## 8. Final checks and PDF

Your `images` folder must contain `1.png` through `11.png`. Use these exact lowercase names. Check that each image is readable and matches its caption.

```bash
ls images
rg '\[Write|\[Describe|\[Confirm|\[Record' ArtemPeychevLab5.md
```

The second command should print nothing once every observation is filled. If rg is unavailable, use your editor to search for `[Write`, `[Describe`, `[Confirm`, and `[Record`.

Use your usual Markdown to PDF method. If Pandoc and a LaTeX engine are already installed, this command includes the images and gives long code lines a little more room:

```bash
pandoc ArtemPeychevLab5.md -o ArtemPeychevLab5.pdf --listings -H codewrap.tex --pdf-engine=xelatex -V geometry:margin=0.7in -V fontsize=10pt -V monofont='DejaVu Sans Mono'
```

If xelatex is missing but pdflatex is installed:

```bash
pandoc ArtemPeychevLab5.md -o ArtemPeychevLab5.pdf --listings -H codewrap.tex --pdf-engine=pdflatex -V geometry:margin=0.7in -V fontsize=10pt
```

If neither PDF engine is ready, use your existing editor's Markdown PDF export or print its rendered Markdown preview to PDF. Verify that all images appear and code is not cut off. Do not spend the last minutes installing a large TeX distribution.

## 9. What to submit

The provided instructions require the writeup and your written source files for Parts 1, 3, and 4. The writeup alone is not enough. They do not specify ZIP or tar.gz, so follow the Canvas upload slots. Upload the PDF and the nine C files directly if separate files are accepted. If Canvas accepts an archive, this command creates a package containing the report, Markdown source, C source, screenshot images, test inputs, and build script:

```bash
tar -czf ArtemPeychevLab5.tar.gz ArtemPeychevLab5.pdf ArtemPeychevLab5.md p1_task1.c p1_task2.c p1_task3.c p3_city_vulnerable.c p3_city_fixed.c p3_copy_vulnerable.c p3_copy_fixed.c p4_addition.c p4_subtraction.c images normal_city.txt long_city.txt build.sh codewrap.tex
```

Do not submit `QUICKSTART.md` as part of your report. It is your working guide. Your final report must include actual observations and your own screenshots, not the bracketed placeholders.
