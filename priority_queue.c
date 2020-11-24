#include "priority_queue.h"

struct Node_t {
    PQElement element;
    PQElementPriority priority;
    struct Node_t* next;
};

struct PriorityQueue_t {
    Node head;
    CopyPQElement copy_element;
    FreePQElement free_element;
    EqualPQElements equal_elements;
    CopyPQElementPriority copy_priority;
    FreePQElementPriority free_priority;
    ComparePQElementPriorities compare_priorities;
    Node iterator;
};

Node nodeCreate(PQElement element, PQElementPriority priority)
{
    if(!element || !priority)
    {
        return NULL;
    }
    Node node = malloc(sizeof(*node));
    if(!node)
    {
        return NULL;
    }
    node->element = element;
    node->priority = priority;
    node->next = NULL;
    return node;
}

PriorityQueue pqCreate(CopyPQElement copy_element,
                       FreePQElement free_element,
                       EqualPQElements equal_elements,
                       CopyPQElementPriority copy_priority,
                       FreePQElementPriority free_priority,
                       ComparePQElementPriorities compare_priorities)
{
    if(!copy_element || !free_element || !equal_elements || !copy_priority || !free_priority || !compare_priorities)
    {
        return NULL;
    }
    PriorityQueue queue = malloc(sizeof(*queue));
    if(!queue)
    {
        return NULL;
    }    
    /*
    Node iterator = malloc(sizeof(*iterator));
    if(!iterator)
    {
        free(queue);
        return NULL;
    }
    */
    queue->head = NULL;
    queue->copy_element = copy_element;
    queue->free_element = free_element;
    queue->equal_elements = equal_elements;
    queue->copy_priority = copy_priority;
    queue->free_priority = free_priority;
    queue->compare_priorities = compare_priorities;
    return queue;
}

void pqDestroy(PriorityQueue queue)
{
    if(!queue)
    {
        return;
    }
    assert(queue!=NULL);
    while(queue->head)
    {
        Node to_delete = queue->head;
        queue->free_element(to_delete->element);
        queue->free_priority(to_delete->priority);
        queue->head = to_delete->next;
        free(to_delete);
    }
    free(queue);
}


PriorityQueue pqCopy(PriorityQueue queue)
{
    if(!queue)
    {
        return NULL;
    }
    PriorityQueue new_queue = pqCreate( queue->copy_element,
                                        queue->free_element,
                                        queue->equal_elements,
                                        queue->copy_priority,
                                        queue->free_priority,
                                        queue->compare_priorities);
    if(!new_queue)
    {
        return NULL;
    }
    Node new_head = nodeCreate(queue->head->element, queue->head->priority);
    if(!new_head)
    {
        pqDestroy(new_queue);
        return NULL;
    }
    new_queue->head = new_head;
    Node head_ptr = queue->head->next;
    while(head_ptr)
    {
        Node node = nodeCreate(head_ptr->element, head_ptr->priority);
        if(!node)
        {
            pqDestroy(new_queue);
            return NULL;
        }
        new_queue->head->next=node;
        head_ptr = head_ptr->next;
    }
    return new_queue;
}

int pqGetSize(PriorityQueue queue)
{
    if(!queue)
    {
        return -1;
    }
    Node head_ptr = queue->head;
    int counter = 0;
    while(head_ptr)
    {
        head_ptr = head_ptr->next;
        counter++;
    }
    return counter;
}

bool pqContains(PriorityQueue queue, PQElement element)
{
    if(!queue || !element)
    {
        return false;
    }
    Node head_ptr = queue->head;
    while(head_ptr)
    {
        if(queue->equal_elements(head_ptr, element))
        {
            return true;
        }
        head_ptr = head_ptr->next;
    }
    return false;
}

PriorityQueueResult pqInsert(PriorityQueue queue, PQElement element, PQElementPriority priority)
{
    if(!queue || !element || !priority)
    {
        return PQ_NULL_ARGUMENT;
    }
    PQElement new_element = queue->copy_element(element);
    if(!new_element)
    {
        return PQ_OUT_OF_MEMORY;
    }    
    PQElementPriority new_priority = queue->copy_priority(priority);
    if(!new_element)
    {
        queue->free_element(new_element);
        return PQ_OUT_OF_MEMORY;
    }
    Node to_add = nodeCreate(new_element, new_priority);
    if(!to_add)
    {
        queue->free_element(new_element);
        queue->free_priority(new_priority);
        return PQ_OUT_OF_MEMORY;
    }
    if(!(queue->head))
    {
        queue->head = to_add;
        return PQ_SUCCESS;
    }
    if(queue->compare_priorities(queue->head->priority, to_add->priority) < 0)
    {
        to_add->next = queue->head;
        queue->head = to_add;
        return PQ_SUCCESS;
    }
    Node temp_head = (queue->head);
    assert(queue->compare_priorities(queue->head->priority, to_add->priority) >= 0);
    while(!(temp_head->next) && queue->compare_priorities(temp_head->next->priority, to_add->priority) > 0)
    {
        temp_head = temp_head->next;
    }
    to_add->next = temp_head->next;
    temp_head->next = to_add;
    return PQ_SUCCESS;
}

PriorityQueueResult pqChangePriority(PriorityQueue queue, PQElement element,
                                     PQElementPriority old_priority, PQElementPriority new_priority)
{
    if(!queue || !element || !old_priority || !new_priority)
    {
        return PQ_NULL_ARGUMENT;
    }
    Node head_ptr = queue->head;;
    if(queue->compare_priorities(old_priority, queue->head->priority) == 0)
    {
        queue->free_element(queue->head->element);
        queue->free_priority(queue->head->priority);
        free(queue->head);
        queue->head = head_ptr;
    }
    else
    {
        assert(queue->compare_priorities(queue->head->priority, old_priority));
        while(head_ptr->next && queue->compare_priorities(old_priority, head_ptr->next->priority) != 0)
        {
            head_ptr = head_ptr->next;
        }
        if(!head_ptr->next)
        {
            return PQ_ELEMENT_DOES_NOT_EXISTS;
        }
        Node to_delete = head_ptr->next;
        queue->free_element(to_delete->element);
        queue->free_priority(to_delete->priority);
        head_ptr->next = to_delete->next;
        free(to_delete);
    }
    return (pqInsert(queue, element, new_priority) == PQ_SUCCESS) ? PQ_SUCCESS : PQ_OUT_OF_MEMORY;
}

PriorityQueueResult pqRemove(PriorityQueue queue)
{
    if(!queue)
    {
        return PQ_NULL_ARGUMENT;
    }
    Node to_remove = queue->head;
    queue->head = to_remove->next;
    queue->free_element(to_remove->element);
    queue->free_priority(to_remove->priority);
    free(to_remove);
    return PQ_SUCCESS;
}

PriorityQueueResult pqRemoveElement(PriorityQueue queue, PQElement element)
{
    if(!queue || !element)
    {
        return PQ_NULL_ARGUMENT;
    }
    Node temp_head = queue->head;

    if(queue->equal_elements(queue->head, element))
    {
        return pqRemove(queue);
    }
    while(temp_head->next && queue->equal_elements(element, temp_head->next) != 0)
    {
        temp_head = temp_head->next;
    }
    if(!temp_head->next)
    {
        return PQ_ELEMENT_DOES_NOT_EXISTS;
    }
    Node to_delete = temp_head->next;
    queue->free_element(to_delete->element);
    queue->free_priority(to_delete->priority);
    temp_head->next = to_delete->next;
    free(to_delete);
    return PQ_SUCCESS;
}


PQElement pqGetFirst(PriorityQueue queue)
{
    if(!queue || pqGetSize(queue)==0)
    {
        return NULL;
    }
    queue->iterator = queue->head;
    return queue->head->element;

}


PQElement pqGetNext(PriorityQueue queue)
{
    if(!queue || pqGetSize(queue)==0 || !(queue->iterator->next))
    {
        return NULL;
    }
    queue->iterator = queue->iterator->next;
    return queue->iterator->element;
}

PriorityQueueResult pqClear(PriorityQueue queue)
{
    if(!queue)
    {
        return PQ_NULL_ARGUMENT;
    }
    while(queue->head)
    {
        Node temp_head = queue->head->next;
        queue->free_element(queue->head->element);
        queue->free_priority(queue->head->priority);
        free(queue->head);
        queue->head = temp_head;
    }
    return PQ_SUCCESS;
}