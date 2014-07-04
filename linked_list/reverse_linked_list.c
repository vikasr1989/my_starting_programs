#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOGD //printf

#include "linked_list_interface.h"


my_node *reverse_linked_list(my_node *head);
int main()
{
    my_node *head = my_create_linked_list(0);
    my_print();
    my_node * tmp = reverse_linked_list(head);
    printf("count is %d\n",my_count_linked_list(tmp));
    printf("reversed linked list is\n");
    my_print_linked_list(tmp);
    my_cleanup();
    return 0;
}

my_node *reverse_linked_list(my_node *head)
{
    my_node *p = NULL,*q = NULL,*r = NULL;
    p = head;
    /* Linked list with 0 nodes */
    if(p == NULL)
        return NULL;
    q= head->next;
    /* Linked list with 1 node */
    if(q == NULL)
        return head;
    while(q->next != NULL)
    {
        r = q->next;
        q->next = p;
        p = q;
        q = r;
        r = r->next;
    }
    q->next = p;
    head->next = NULL;
    return q;
}
