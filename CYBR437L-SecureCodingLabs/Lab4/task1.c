//CYBR437 - Lab4 - Task1: Static Analysis

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE1    512
#define BUFSIZE2    ((BUFSIZE1/2) - 8)

int main(int argc, char *argv[]) {
  char *buf1R1;
  char *buf2R1;
  char *buf1R2;
  volatile int modified;
  char buffer[64];
  int arr[3] = {1, 2, 3};
  
  if (argc > 1) {
        snprintf(buffer, sizeof(buffer), "%s", argv[1]);
    }

  printf(buffer);

  buf1R1 = (char *) malloc(BUFSIZE2);
  buf2R1 = (char *) malloc(BUFSIZE2);

  free(buf1R1);
  free(buf2R1);

  printf("Out-of-bounds read: %d\n", arr[5]); 

  int *ptr = NULL;
  modified = 0;
  gets(buffer);

  if(modified != 0) {
      printf("you have changed the 'modified' variable\n");
  } else {
      printf("Try again?\n");
  }	

  buf1R2 = (char *) malloc(BUFSIZE1);
  strncpy(buf1R2, argv[1], BUFSIZE1-1);

  free(buf2R1);
  free(buf1R2);

  char mem_buffer[10];
  strcpy(mem_buffer, "Welcome, how many errors are there?"); 
  printf("%s\n", mem_buffer);

  printf("Pointer: %d\n", *ptr);

  *ptr = malloc(sizeof(int));
  *ptr = 10;
  free(ptr);


  printf("Use after free: %d\n", *ptr);



  return 0;
		
}
