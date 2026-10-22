import re
import sys

# unsafe functions to search for and description of why they're not safe
functions = {
    "gets": "No limit how much input is read.",
    "strcpy": "Does not check if the copied text fits.",
    "strncpy": "Might not put a null character at the end.",
    "strcat": "Doesn't check if there is room to add more text.",
    "sprintf": "Doesn't limit how much text it writes.",
    "vsprintf": "Does not limit how much text it writes.",
    "scanf": "Can read too much text if %s has no size limit.",
    "sscanf": "Can read too much text if %s has no size limit.",
    "fscanf": "Can read too much text if %s has no size limit.",
    "itoa": "Nonstandard in C and does not take size of buffer.",
}

# regex matches unsafe func names gets OR strcpy OR strcat etc etc
pattern = r"\b(gets|strcpy|strncpy|strcat|sprintf|vsprintf|scanf|sscanf|fscanf|itoa)\s*\("


def scan_file(filename):
    # keeps track of line numbers so it is easier to find unsafe code
    count = 0

    # open filename in read mode
    with open(filename, "r", encoding="utf-8") as file:

        # read each line + count line nums
        for line_number, line in enumerate(file, start=1):

            # find every single match, this works even if there is
            # more than one atch on a single line 
            for match in re.finditer(pattern, line):
                name = match.group(1)

                # print out the func, reaosn flagged, and source code
                print(f"Line {line_number}: {name}")
                print(f"Warning: {functions[name]}")
                print(f"Code: {line.strip()}")
                print()

                count = count + 1

    print(f"Total occurrences found: {count}")


# only runs if its not imported as module
if __name__ == "__main__":

    # make sure a filename was entered right after script name arg position 1
    if len(sys.argv) != 2:
        print("Usage: python3 regex_scanner.py task2.c")
        sys.exit(1)

    # show error cant open, read, or file doesnt exist
    try:
        scan_file(sys.argv[1])
    except (OSError, UnicodeError) as error:
        print(f"Could not open or read the file: {error}")
        sys.exit(1)