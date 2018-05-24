#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct node {
    time_t data;
    struct node * next;
} node_t;

node_t* create(time_t data,node_t* next)
{
    node_t* new_node = (node_t*)malloc(sizeof(node_t));
    if(new_node == NULL)
    {
        printf("Error creating a new node.\n");
        exit(0);
    }
    new_node->data = data;
    new_node->next = next;

    return new_node;
}

int count_node(node_t *head)
{
    node_t *cursor = head;
    int c = 0;
    while(cursor != NULL)
    {
        c++;
        cursor = cursor->next;
    }
    return c;
}
void append(node_t* head, time_t data)
{
    /* go to the last node */
    node_t *cursor = head;
    while(cursor->next != NULL)
        cursor = cursor->next;

    /* create a new node */
    node_t* new_node =  create(data,NULL);
    cursor->next = new_node;

}
void remove_front(node_t* head)
{
    if(head != NULL){
    node_t *front = head;
    head = head->next;
    front->next = NULL;
    /* is this the last node in the list */
    if(front == head)
        head = NULL;
    free(front);
    }
    else{
    	printf("list is null/n");
    }
}
/*
    add a new node at the beginning of the list
*/
void prepend(node_t* head,time_t data)
{
    node_t* new_node = create(data,head);
    head = new_node;
}

int main(){
	node_t *head=NULL;
	prepend(head,time(0));
	printf("end");
	return(0);
}
