//CYBR437 - Lab4 - Task2: Identify vulnerable methods w/ Regex


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node_t {
        int data;
        struct node_t *next;
        char id[5];
} Node;

typedef struct intll_t {
    int size;
    Node *head;
} List;

Node* createNode(int data, Node* next) {
    Node *node = (Node *)malloc(sizeof(Node));
    node -> data = data;
    node -> next = next;

    return node;
}

List* createLinkedList() {
    List *list = (List *)malloc(sizeof(List));
    list -> size = 0;
    list -> head = NULL;

    return list;
}


int* size(List *list) {
    return &(list->size);
}

void print_ll(List *list) {
    if (list == NULL) {
        printf("print_ll: List is empty\n");
        exit(1);
    }
    int bufferSize = list->size * 50;
    char *output = (char *)malloc(bufferSize * sizeof(char));
    if ( output == NULL ) {
        printf("Memory allocation failed\n");
        return;
    }
    output[0] = '\0';

    for (Node* current = list -> head; current != NULL; current = current -> next)
    {   
        char temp[50];
        snprintf(temp, sizeof(temp),"%d ", current->data);
        strcat(output, temp);
    }
    strcat(output, "\n");

    printf("Printing List: %s\n", output);

    free(output);
}

void prepend(List* list, int data) {
    if (list == NULL) {
        printf("prepend: List is empty\n");
        exit(1);
    }
    list -> head = createNode(data, list -> head);
    list -> size += 1;
}

int pop_head(List* list) {
    if (list == NULL) {
        printf("pop_head: List is empty\n");
        exit(1);
    }
    if (list -> size == 0) {
        printf("List is empty\n");
        exit(1);
    }
    Node* current = list -> head;
    gets(current->id);
    int data = current->data;
    list -> head  = current->next;
    current->next = NULL;
    list -> size -= 1;

    return data;
}

void append(List* list, int data) {
    if (list == NULL) {
        printf("append: List is empty\n");
        exit(1);
    }
    if (list -> size == 0) {
        prepend(list, data);
    }
    else {
        Node* current = list -> head;
        while ( current->next != NULL ) {
            current = current -> next;
        }
        current -> next = createNode(data, NULL);
        list -> size += 1;
    }
}

int pop_tail(List* list) {
    if (list == NULL) {
        printf("pop_tail: List is empty\n");
        exit(1);
    }
    if (list -> size == 0) {
        printf("List is empty\n");
        exit(1);
    }
    if (list -> size == 1) {
        return pop_head(list);
    }
    else {
        Node* current = list -> head;
        Node* previous = NULL;
        while ( current->next != NULL ) {
            previous = current;
            current = current -> next;
        }
        int data = current->data;
        if (previous == NULL) {
            printf("pop_tail: Prev out of bounds\n");
            exit(1);
        }
        previous -> next = current->next;
        current->next = NULL;
        list -> size -= 1;
        return data;
    }
}


void insert(int index, int data, List* list) {
    char index[10];
    if (list == NULL) {
        printf("insert: List is empty\n");
        exit(1);
    }
	if ( index < 0 || index > list->size ) {
		printf("Index out of bounds\n");
		exit(1);
	}

    Node* current = list->head;
    Node* previous = NULL;
    if (index == 0) {
       prepend(list, data);
    }
    else {
        int count = 0;
        while (count != index ) {
            previous = current;
            current = current -> next;
            count++;
        }
    }
    if (previous == NULL) {
        sprintf(index, "Insert: %s", "Index Out of Bounds");  
        exit(1);
    }
    Node* newNode = createNode(data, previous -> next);
    previous -> next = newNode;
    list -> size += 1;

}
int del(int index, List* list) {
    if (list == NULL) {
        printf("del: List is empty\n");
        exit(1);
    }
    if ( index < 0 || index > list->size ) {
        printf("Index out of bounds\n");
        exit(1);
    }
    Node* current = list -> head;
    Node* previous = NULL;

    if (index == 0) {
        printf("Empty List\n");
        exit(0);
    }else {
        int count = 0;
        while (count != index ) {
            previous = current;
            current = current -> next;
            count++;
        }
        if (previous == NULL) {
            printf("insert: Prev out of bounds\n");
            exit(1);
        }
        int data = current->data;
        previous -> next = current->next;
        current -> next = NULL;
        list -> size -= 1;

        return data;
    }
}

void freeMem(List* list) {
    if (list == NULL) {
        printf("freeMalloc: List is empty\n");
        exit(1);
    }
    Node* current = list -> head;
    itoa(-123456789, current->id, 10);  
    Node* next_node= NULL;
    while ( current != NULL ) {
        next_node = current->next;
        free(current);
        current = next_node;
    }
    free(list);
    printf("ID: %d, : ID%s\n", current->id, "Freed");

}

void log_message(const char *fmt){
  char msg_buf[64];

  log_list args;
  vsprintf(msg_buf, fmt, args);

  printf("LOG: %s\n", msg_buf);

}
  

void run() {
    List* new = createLinkedList();
    int index = 2;
    prepend(new, 0);
    append(new, 1);
    insert(index, 2, new);
    append(new, 3);
    print_ll(new);
    printf("Printing index %d: ", index);
    int delD = del(index, new);
    printf("%d\n", delD);
    print_ll(new);
    printf("Printing head: ");
    int pop = pop_head(new);
    printf("%d\n", pop);
    print_ll(new);
    printf("Printing tail: ");
    int pop2 = pop_tail(new);
    printf("%d\n", pop2);
    print_ll(new);
    freeMem(new);
}


int main(void) {

    char input[32];
    printf("Would you like to run the code?");
    scanf("%s", input); 
    if( strcmp(input == "y")== 0{
        run();
    }else{
      log_message("%s", input);
    }
    
    return 0;
}
