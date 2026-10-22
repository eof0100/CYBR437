# CYBR 437 Secure Coding

## Lab 4: Regular Expressions and Static Analysis

**Name:** Artem Peychev  
**Class:** CYBR437 Secure Coding 
**Assignment:** Lab 4 - Static Analysis with Regular Expressions
**Due date:** October 21, 2026

---


## Part 1: Regular Expressions


### Problem 1: Social Security Number (SSN) (20 points)

Create a pattern for an SSN with exactly nine digits. Optional dashes or whitespace may occur only between digits 3 and 4 and/or digits 5 and 6. Mixed separators and omitted separators are allowed by the examples.

The stated rules are:

1. None of the three segments may consist entirely of zeros.
2. Reject nine identical digits and the consecutive sequence 123456789, including separated forms.
3. The handout states that SSA issues digits 1, 8, and 9 only in position 1.
4. An SSN starting with 9 must have 9 in position 4.
5. Reject 666 in positions 1 through 3.
6. There must be exactly nine digits.



#### Social Security 

I started with a pattern that first matched the overall syntax of the SSN's. First I started with a pattern that matched three digits followed by two digits followed by four digits. Between each group of digits I added a dash or whitespace which is optional and not required. Using regex101.com, I came up with the following regular expression that worked.

![SSN Overall Pattern](images/1.png)

Then I added what is called negative lookaheads to reject rule 1 and 5 which will reject 000 or 666 in the first grouping, 00 in the middle grouping, and 0000 in the last grouping.

![Reject 000, 666, 00, 0000](images/2.png)


Then I added check to reject consecutive sequence of numbers 1234567789 and nine any identical numbers which includes with and without seperators.


![Reject Consecutive & Identical Numbers](images/3.png)

Then I moved on to rule #4 which rejects SSN's that start with a 9 but the fourth digit is anything from 0-8 (9 is allowed). This means that a number starting with a 9 must have the fourth digit be also a 9.

![Reject SSN Starting with a 9 with fourth digit 0-8](images/4.png)

The final regular expression ended up being below. Then I added the regular expression to the python script and used the regex testing script to make sure it passed all the tests for valid SSN's and made sure to fail for all invalid SSN's.


#### Final Regular expression:

```python
regex = r"^(?!000|666)(?!9[0-9]{2}[-\s]?[0-8])(?!([0-9])\1{2}[-\s]?\1{2}[-\s]?\1{4}$)(?!123[-\s]?45[-\s]?6789$)[0-9]{3}[-\s]?(?!00)[0-9]{2}[-\s]?(?!0000)[0-9]{4}$"
```

![SSN Test Results](images/ssn_pass.png)


![SSN Test Code](images/ssn_test_code.png)



#### Shortcomings

Rule 3 states that the digits 1,8,9 are issued only in position 1. It I take this rule literally this causes conflicts with valid SSN examples such as 132-45-6789 which contain digits 8 and 9 in the last two positions. Also, this conflics with rule #4 which says if the first digit is a 9 the fourth digit must also be a 9. So what I did as a compromise, I implemented the rule #4 but did not implement rule #3.


--- 


### Problem 2: Email Address (20 points)

Create a pattern with a local part and a domain separated by one @ symbol.

1. The local part must contain at least one character.
2. The domain must contain at least four characters.
3. The allowed special characters are hyphen (-), underscore (_), and dot (.).
4. A dot cannot be the first character or immediately precede @.
5. Consecutive dots are not allowed anywhere in the email address.
6. The dot restrictions do not apply to hyphens or underscores.

For the e-mail pattern using regex101.com, I created a pattern that checks for local part which contains at minimum one character, domain separated by @ must contain at least four characters. Both parts do allow letters, numbers, hyphens, underscores, and dots according to the 6 rules.

I first came up with a pattern that simply rejects addresses that start with a period, contain a period immediately before the @ sign, and any consecutive dots anywhere in the e-mail address. Here is a screenshot of the regular expression that pass initial test cases:


#### Regular expression

```python
regex = r"^(?!\.)(?!.*\.\.)[A-Za-z0-9_.-]+(?<!\.)@[A-Za-z0-9_.-]{4,}$"
```

![Regex Email Pattern](images/5.png)

Once I added the regular expression to cybr437regex.py, then I added valid and invalid test cases to cybr437regextesting.py and passed all 11 tests.  

![Regex Email Tests](images/email_pass.png)

![Email Test Code](images/email_test_code.png)


#### Shortcomings

My pattern accepts domains without a dot, for example a@abcd, because the instructions require at least four characters after @ but do not require a domain extension. It also allows dots at the beginning or end of the domain because those positions are not prohibited. Consecutive dots are rejected anywhere in the address.
The pattern follows the lab’s limited character rules rather than a real email standard from the real world. But all five valid and six invalid Python tests produced the expected results.

Note: I added the sixth test myself to test .@ewu.edu which tests both a dot in the beginning and a dot before the @ symbol.



---

### Problem 3: Password (20 points)

Create a password pattern that meets these requirements:

1. At least ten characters.
2. At least one uppercase letter.
3. At least one lowercase letter.
4. At least one digit.
5. At least one punctuation mark.
6. No more than three consecutive lowercase letters.
7. The handout states that the password must not contain "two digits that repeat."
8. The first character must be a lowercase letter or a punctuation mark.

<!-- Explain your interpretation of "two digits that repeat." The supplied valid example t3sTingM33! contains repeated 3 digits. Define what you count as punctuation. -->

I created a regular expression pattern that checks for at least ten characters, including at least one uppercase letter, one lowercase letter, one digit, and one punctuation mark. It also checks that the password starts with a lowercase letter or punctuation and does not contain more than three consecutive lowercase letters. Rule #7, I interpreted the rule "two digits that repeat" as pair of two digits such as 8888 or 1313 and not 33 because one of the valud examples that is suppose to pass "t3sTingM33!" contains 33 which are two consecutive digit.
I tested the five valid and five invalid examples from the instructions in regex101.com and as the screenshot shows the first five valid examples all matched and the other five did not.

![Password Regex on regex101.com](images/6.png)

After adding valid and invalid tests to cybr437regextesting.py, running the password tests all passed.

![Password Test Results](images/password_pass.png)

Below shows the valid and invalid testing code that I added to cybr437regextesting.py

![Password Testing Code](images/password_code.png)

H


#### Regular expression

```python
regex = r"<answer here>"
```






## Part 2: Static Analysis and Regex Scanner (40 points)

### Task 1: Analyze task1.c with Four Tools (15 points)

Analyze `task1.c` with cppcheck, flawfinder, splint, and Clang Static Analyzer. Explain each tool's output and compare the results.

#### cppcheck

**Command used:**

```bash
<command here>
```

**Output explanation:**

<!-- Explain the actual findings, affected lines, why they matter, and any parsing/compilation errors that limited the analysis. Add rows as needed. -->

| Source line | Tool finding | Explanation of the issue |
| --- | --- | --- |
| <answer here> | <answer here> | <answer here> |
| <answer here> | <answer here> | <answer here> |

<answer here>

![Part 2, Task 1: cppcheck output](images/2_1.png)

#### flawfinder

**Command used:**

```bash
<command here>
```

**Output explanation:**

<!-- Explain the actual findings, affected lines, why they matter, and any parsing/compilation errors that limited the analysis. Add rows as needed. -->

| Source line | Tool finding | Explanation of the issue |
| --- | --- | --- |
| <answer here> | <answer here> | <answer here> |
| <answer here> | <answer here> | <answer here> |

<answer here>

![Part 2, Task 1: flawfinder output](images/2_2.png)

#### splint

**Command used:**

```bash
<command here>
```

**Output explanation:**

<!-- Explain the actual findings, affected lines, why they matter, and any parsing/compilation errors that limited the analysis. Add rows as needed. -->

| Source line | Tool finding | Explanation of the issue |
| --- | --- | --- |
| <answer here> | <answer here> | <answer here> |
| <answer here> | <answer here> | <answer here> |

<answer here>

![Part 2, Task 1: splint output](images/2_3.png)

#### Clang Static Analyzer

**Command used:**

```bash
<command here>
```

**Output explanation:**

<!-- Explain the actual findings, affected lines, why they matter, and any parsing/compilation errors that limited the analysis. Add rows as needed. -->

| Source line | Tool finding | Explanation of the issue |
| --- | --- | --- |
| <answer here> | <answer here> | <answer here> |
| <answer here> | <answer here> | <answer here> |

<answer here>

![Part 2, Task 1: Clang Static Analyzer output](images/2_4.png)

#### Comparative analysis of the four tools

| Tool | Main findings | Findings missed or limitations | How its results differed from the other tools |
| --- | --- | --- | --- |
| cppcheck | <answer here> | <answer here> | <answer here> |
| flawfinder | <answer here> | <answer here> | <answer here> |
| splint | <answer here> | <answer here> | <answer here> |
| Clang Static Analyzer | <answer here> | <answer here> | <answer here> |

<!-- Discuss overlapping findings, differences in detection, and whether an error prevented a tool from completing. A tool stopping early does not prove the code has no further issues. -->

<answer here>

---

### Task 2: Python Regex Scanner for Vulnerable Functions (25 points)

Write a Python program that uses regex to detect vulnerable or deprecated function uses in C source files. Run it against `task2.c`. For every occurrence, output a message, its line number, and the line of C code.

**Scanner filename:** `<scanner_filename>.py`

#### Targeted functions and reasons

<!-- List the actual functions your scanner targets. Add rows as needed. Distinguish inherently unsafe/deprecated functions from functions whose safety depends on their arguments and usage. -->

| Function | Why the scanner flags it |
| --- | --- |
| <answer here> | <answer here> |
| <answer here> | <answer here> |
| <answer here> | <answer here> |

#### Regex pattern

```python
<answer here>
```

#### Scanner source code

<!-- Paste your completed scanner as text. Also submit the actual Python file separately. -->

```python
<answer here>
```

#### Detailed explanation of the code

<!-- Explain how the program reads the input file, applies the regex, detects every occurrence, tracks line numbers, and reports the matching code. Explain handling of multiple matches on one line if applicable. -->

<answer here>

#### Run the scanner against task2.c

**Command used:**

```bash
<command here>
```

**Results:**

<!-- Record your actual results. Add one row for every reported occurrence, including repeated uses of the same function on different lines. -->

| Line number | Function | Reported message | Matching source line |
| --- | --- | --- | --- |
| <answer here> | <answer here> | <answer here> | `<source line here>` |
| <answer here> | <answer here> | <answer here> | `<source line here>` |
| <answer here> | <answer here> | <answer here> | `<source line here>` |

<answer here>

![Part 2, Task 2: Regex scanner command and output](images/2_5.png)

<!-- Add images/2_6.png and more screenshots if required to show the complete output. -->

#### Scanner limitations

<!-- Explain any actual limitations, such as matching text inside comments or strings, missing calls split across lines, or flagging functions without checking argument sizes. Do not claim it is a complete C vulnerability analyzer. -->

<answer here>

---

<!-- BEFORE SUBMISSION: Remove this comment after checking the following.
1. All three Part 1 patterns are included and explained component by component.
2. Each Part 1 problem has five valid and five invalid inputs, with reasons and actual results.
3. Regex101 testing, Python test cases, and test output are shown.
4. Part 1 shortcomings, failures, and interpretations are documented.
5. All four static analysis tools have commands, actual output screenshots, and explanations.
6. The four tools are compared.
7. The Part 2 scanner is explained and its output includes every detected occurrence, line number, and source line.
8. Replace all placeholders and confirm every image appears in the exported report.
9. Submit cybr437regex.py, cybr437regextesting.py, your scanner Python file, and the write-up. Follow any additional Canvas upload/format instructions.
-->
