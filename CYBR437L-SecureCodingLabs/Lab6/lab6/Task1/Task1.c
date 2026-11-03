#include <stdio.h>

void revealSecret() {
    FILE *fp;
    int c;

    // Attempt to open the secret file
    fp = fopen("secret.txt", "r");
    if (fp) {
        printf("Revealing the secret message:\n");
        while ((c = getc(fp)) != EOF) {
            putchar(c);
        }
        fclose(fp);
    } else {
        printf("Failed to reveal the secret. File not found.\n");
    }
}

int main(void) {
    char buffer[10];
    int secretAccess = 0;

    printf("Enter your message: \n");

    // Vulnerable function
    gets(buffer);

    if (secretAccess != 0) {
        revealSecret();
    } else {
        printf("No secrets revealed.\n");
    }

    return 0;
}
