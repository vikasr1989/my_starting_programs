#ifndef _LINKED_LIST_INTERFACE_H_
#define _LINKED_LIST_INTERFACE_H_
typedef struct node
{
    int data;
    struct node *next;
}my_node;

my_node *my_create_linked_list(int NumberOfElements);
void my_insert(int data);
void my_delete(int data);
void my_print(void);
int my_count(void);
int my_count_linked_list(my_node *head);
void my_cleanup(void);
void my_print_linked_list(my_node *head);

#endif
