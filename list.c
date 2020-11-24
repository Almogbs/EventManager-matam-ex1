#include "list.h"

struct List_t {
    Node head;
    FreeListElement free_element;
    CopyListElement copy_element;
    EqualListElements equal_element;
};

struct Node_t {
    ListElement element;
    struct Node_t* next;
};

Node nodeCreate(ListElement element)
{
    if(!element)
    {
        return NULL;
    }
    Node node = malloc(sizeof(*node));
    node->element = element;
    node->next = NULL;
    return node;
}



List listCreate(ListElement element,
                CopyListElement copy_element, 
                FreeListElement free_element, 
                EqualListElements equal_elements)
{
    if(!element || !copy_element || !free_element || !equal_elements)
    {
        return NULL;
    }
    List list = malloc(sizeof(*list));
    if(!list)
    {
        return NULL;
    }
    Node node = nodeCreate(element);
    if(!node)
    {
        free(list);
        return NULL;
    }
    list->head = node;
    list->copy_element = copy_element;
    list->free_element = free_element;
    list->equal_element = equal_elements;
    return list;
}

void listDestroy(List list)
{
    if(!list)
    {
        return;
    }
    while(list->head)
    {
        Node tmp_head = list->head;
        list->head = tmp_head->next;
        list->free_element(tmp_head->element);
        free(tmp_head);
    }
    free(list);
}

int listGetSize(List list)
{
    if(!list)
    {
        return 0;
    }
    int counter = 0;
    while(list->head)
    {
        list->head = list->head->next;
        counter++;
    }
    return counter;
}

bool listContains(List list, ListElement element)
{
    if(!list || !element)
    {
        return false;
    }
    while(list->head)
    {
        if(list->equal_element(list->head->element, element))
        {
            return true;
        }
        list->head = list->head->next;
    }
    return false;
}


ListResult listInsert(List list, ListElement element)
{
    if(!list || !element)
    {
        return LIST_NULL_ARGUMENT;
    }
    Node new_node = nodeCreate(element);
    if(!new_node)
    {
        return LIST_OUT_OF_MEMORY;
    }
    new_node->next = list->head;
    list->head = new_node;
    return LIST_SUCCESS;
}


ListResult listRemoveElement(List list, ListElement element)
{
    if(!list || !element)
    {
        return LIST_NULL_ARGUMENT;
    }
    while(list->head)
    {
        if(list->equal_element(list->head->element, element))
        {
            Node tmp_node = list->head;
            list->head = list->head->next;
            list->free_element(tmp_node->element);
            free(tmp_node);
            
            return LIST_SUCCESS;

        }
    }
    return LIST_ELEMENT_DOES_NOT_EXISTS;
}

