#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void showMissionDetails() {
    FILE *fp;
    int c;

    // Attempt to open the mission's secret file
    fp = fopen("mission_flag.txt", "r");
    if (fp) {
        printf("Mission launched! Revealing mission details:\n");
        while ((c = getc(fp)) != EOF) {
            putchar(c);
        }
        fclose(fp);
    } else {
        printf("Mission launch a success, but the Mission_Details_File is missing.\n");
    }
}

void verifyLaunchKey(){
    int launchKey = 0x87654321;
    char buffer[25];
    printf("Enter launch key: ");
    fflush(stdout);
    gets(buffer);
    if(launchKey == 0xd3adfa11){
        printf("Launch key accepted!\n");
        showMissionDetails();
    }
    else{
        printf("Incorrect launch code: %04x\n", launchKey);
        printf("Mission aborted...\n");
        fflush(stdout);
    }
}

int main(int argc, char* argv[]){
    verifyLaunchKey();
    return 0;
}
