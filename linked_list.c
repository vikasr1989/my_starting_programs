#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOGD //printf

typedef struct node
{
    int data;
    struct node *next;
}my_node;

void my_insert(int data);
void my_delete(int data);
void my_print(void);
int my_count(void);
void my_cleanup(void);
my_node *head = NULL;
void *handle = NULL;

int main()
{
    head = NULL;
    LOGD("head = %p\n",head);
    my_insert(0);
    LOGD("head = %p\n",head);
    my_insert(1);
    my_insert(2);
    my_insert(3);
    my_insert(4);
    my_insert(5);
    my_insert(6);
    my_insert(7);
    my_insert(8);
    my_delete(8);
    printf("count %d\n",my_count());
    my_print();
    my_cleanup();
    return 0;
}

my_node *my_add_node_function(void *handle);
void my_delete_node_function(my_node *p,void *handle);
void my_add_element(my_node *leaf,int data,void *handle);
my_node *my_delete_element(my_node *head,int data,void *handle);
void my_delete_linked_list(my_node *head,void *handle);
void my_print_linked_list(my_node *head);

void my_insert(int data)
{
    LOGD("%s\n",__func__);
    my_node *tmp = head;
    if(head == NULL)
    {
        head = my_add_node_function(handle);
    }
    else
    {
        LOGD("tmp = %p\n",tmp);
        while(tmp->next != NULL)
        {
            LOGD("tmp = %p\n",tmp);
            tmp = tmp->next;
        }
        tmp->next = my_add_node_function(handle);
        tmp = tmp->next;
    }
    LOGD("tmp = %p\n",tmp);
    LOGD("head = %p\n",head);
    my_add_element(tmp,data,handle);
}
void my_delete(int data)
{
    LOGD("%s\n",__func__);
    head = my_delete_element(head,data,handle);
}
void my_print(void)
{
    LOGD("%s\n",__func__);
    my_print_linked_list(head);
}
int my_count(void)
{
    LOGD("%s\n",__func__);
    my_node *next = NULL;
    int count = 0;
    LOGD("ptr = %p\n",head);
    for(next = head;next != NULL;next = next->next)
    {
        LOGD("ptr = %p\n",next);
        count++;
    }
    return count;
}
void my_cleanup(void)
{
    LOGD("%s\n",__func__);
    my_delete_linked_list(head,handle);
}

my_node *my_add_node_function(void *handle)
{
    my_node *p = malloc(sizeof(my_node));
    LOGD("%s\n",__func__);
    LOGD("ptr = %p\n",p);
    return p;
}
void my_delete_node_function(my_node *p,void *handle)
{
    LOGD("%s\n",__func__);
    LOGD("ptr = %p\n",p);
    free(p);
}

void my_add_element(my_node *leaf,int data,void *handle)
{
    LOGD("%s\n",__func__);
    if(leaf)
    {
        leaf->data = data;
        leaf->next = NULL;
    }
    return ;
}

my_node *my_delete_element(my_node *head,int data,void *handle)
{
    LOGD("%s\n",__func__);
    my_node *temp = NULL;
    my_node *prev = NULL;
    if(head)
    {
        for (temp = head;temp != NULL;temp = temp->next)
        {
            LOGD("for loop temp = %p head = %p\n",temp,head);
            if(temp->data == data)
                break;
            prev=temp;
        }
        /* Element not found */
        if(temp == NULL)
        {
            prev = head;
            LOGD("Element not found\n");
        }
        else
        {
            LOGD("b4 if prev = %p\n",prev);
            if(prev != NULL)
            {
            /* Element found in the middle */
                LOGD("if ptr = %p\n",temp);
                prev->next = temp->next;
                my_delete_node_function(temp,handle);
                prev = head;
                LOGD("prev = %p\n",prev);
            }
            else
            {
            /* Element found in the head node */
                prev = temp->next;
                LOGD("else prev = %p\n",prev);
                my_delete_node_function(temp,handle);
            }
        }
    }
    return prev;
}

void my_print_linked_list(my_node *head)
{
    LOGD("%s\n",__func__);
    my_node *next = NULL;
    LOGD("ptr = %p\n",head);
    for(next = head;next != NULL;next = next->next)
    {
        LOGD("ptr = %p\n",next);
        printf("%d\n",next->data);
    }
    return;
}

void my_delete_linked_list(my_node *head,void *handle)
{
    LOGD("%s\n",__func__);
    my_node *next = head;
    my_node *temp = NULL;
    LOGD("ptr = %p\n",head);
    while(next != NULL)
    {
        LOGD("ptr = %p\n",next);
        temp = next->next;
        my_delete_node_function(next,handle);
        next = temp;
    }
    return;    
}
