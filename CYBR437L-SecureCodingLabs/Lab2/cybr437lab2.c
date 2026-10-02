#include "cybr437lab2.h"

const int MAX = 9;

int * fillArray(int *size)
{
    int x=0, num = 0; 
    int * array = (int*)calloc(MAX , sizeof(int));

    printf("Please enter up to %d positive (>0) integers. To quit early enter -99\n\n", MAX);

    do
    {
        printf("enter pos value (-99 to quit) ");
        scanf("%d", &num);
    }while(num < 1 && num != -99);

    if(num == -99)
    {
      *size =  0;
      return array;
    }

    array[x] = num;
    x++;

    while(x < MAX && num != -99) 
    {

        printf("enter pos value (-99 to quit) ");
        scanf("%d", &num);

        while(num < 1 && num != -99)
        {
            printf("try again\n");
            printf("enter a pos int (-99 to quit) ");
            scanf("%d", &num);
        }

        if(num != -99)
           array[x] = num;

        x++;  
    }

    if(num == -99)// && x != MAX)
	x --; 

    *size = x;
    printf("%d\n", *size);

    return array;
}



void printArray(int size, int * myArray)
{
    int x;
    if(size != 0)
    {
        printf("[ ");
        for(x = 0; x < size - 1; x++)
           printf("%d, ", myArray[x]);

        printf("%d ]\n", myArray[size - 1]);
    }

    else
        printf("Empty Array!\n");
}


int cleanUp(int ** myArray)
{
	free(*myArray);
        *myArray = NULL;
	return 0;

}
