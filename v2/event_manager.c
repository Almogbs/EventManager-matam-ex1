
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "event_manager.h"
#include "priority_queue.h"
#include "date.h"
#include "member_list.h"
#include "member.h"
#include "event.h"

/**
 *   Functions for operating on Event elements & Date priorety element
 *   in the generic ADT priorety queue
 */
static PQElement copyEvent(PQElement event)
{
    return (PQElement)eventCopy((Event)event);
}

static void freeEvent(PQElement event)
{
    eventDestroy((Event)event);
}

static bool equalEvent(PQElement event1, PQElement event2)
{
    return eventEqual((Event)event1, (Event)event2);
}

static PQElementPriority copyDate(PQElementPriority date)
{
    return (PQElementPriority)dateCopy((Date)date);
}

static void freeDate(PQElementPriority date)
{
    dateDestroy((Date)date);
}

static int compareDate(PQElementPriority date1, PQElementPriority date2)
{
    return -dateCompare((Date)date1, (Date)date2);
}


// Struct for Event Manager
struct EventManager_t{
    Date init_date;
    PriorityQueue event_list;
    MemberList member_list;
};

EventManager createEventManager(Date date)
{
    if(!date)
    {
        return NULL;
    }
    EventManager em = malloc(sizeof(*em));
    if(!em)
    {
        return NULL;
    }
    PriorityQueue pq = pqCreate(copyEvent, freeEvent, equalEvent, copyDate, freeDate, compareDate);
    if(!pq)
    {
        free(em);
        return NULL;
    }
    Date init_date = dateCopy(date);
    if(!init_date)
    {
        free(em);
        pqDestroy(pq);
        return NULL;
    }
    MemberList member_list = memberListCreate();
    if(!member_list)
    {
        free(em);
        pqDestroy(pq);
        dateDestroy(date);
        return NULL;
    }
    em->event_list = pq;
    em->init_date = init_date;
    em->member_list = member_list;
    return em;
}


void destroyEventManager(EventManager em)
{
    if(!em)
    {
        return;
    }
    dateDestroy(em->init_date);
    pqDestroy(em->event_list);
    memberListDestroy(em->member_list);
    free(em);
}

EventManagerResult emAddEventByDate(EventManager em, char* event_name, Date date, int event_id)
{
    if(!em || !event_name || !date)
    {
        return EM_NULL_ARGUMENT;
    }
    if(dateCompare(date, em->init_date) < 0)
    {
        return EM_INVALID_DATE;
    }
    if(event_id < MIN_EVENT_ID)
    {
        return EM_INVALID_EVENT_ID;
    }
    Event new_event = eventCreate(event_name, event_id, date);
    if(!new_event)
    {
        return EM_OUT_OF_MEMORY;
    }
    Date new_date = dateCopy(date);
    {
        if(!date)
        {
            eventDestroy(new_event);
            return EM_OUT_OF_MEMORY;
        }
    }

    PQ_FOREACH(Event, iter, em->event_list)
    {
        if(eventCompare(iter, new_event))
        {
            eventDestroy(new_event);
            dateDestroy(new_date);
            return EM_EVENT_ALREADY_EXISTS;
        }
        else if(eventEqual(iter, new_event))
        {
            eventDestroy(new_event);
            dateDestroy(new_date);
            return EM_EVENT_ID_ALREADY_EXISTS;
        }
    }
    PriorityQueueResult result = pqInsert(em->event_list, (PQElement)new_event, (PQElementPriority)new_date);
    switch (result)
    {
    case PQ_NULL_ARGUMENT:
            eventDestroy(new_event);
            dateDestroy(new_date);
            return EM_NULL_ARGUMENT;
    case PQ_OUT_OF_MEMORY:
            eventDestroy(new_event);
            dateDestroy(new_date);
            return EM_OUT_OF_MEMORY;
    case PQ_SUCCESS:
            eventDestroy(new_event);
            dateDestroy(new_date);
            return EM_SUCCESS;
    default:
            eventDestroy(new_event);
            dateDestroy(new_date);
            return EM_ERROR;
    }
    return EM_ERROR;
}

EventManagerResult emAddEventByDiff(EventManager em, char* event_name, int days, int event_id)
{
    if(!em || !event_name)
    {
        return EM_NULL_ARGUMENT;
    }
    Date new_date = dateCopy(em->init_date);
    if(!new_date)
    {
        return EM_OUT_OF_MEMORY;
    }
    for(int i=0; i<days; i++)
    {
        dateTick(new_date);
    }
    EventManagerResult result = emAddEventByDate(em, event_name, new_date, event_id);
    dateDestroy(new_date);
    return result;
}

EventManagerResult emRemoveEvent(EventManager em, int event_id)
{
    if(!em)
    {
        return EM_NULL_ARGUMENT;
    }
    if(event_id < MIN_EVENT_ID)
    {
        return EM_INVALID_EVENT_ID;
    }
    Event temp_event = NULL;
    PQ_FOREACH(Event, iter, em->event_list)
    {
        if(eventGetId(iter) == event_id)
        {
            temp_event = iter;
            break;
        }
    }
    memberListUpdatePassedEvent(em->member_list, eventGetMemberList(temp_event));
    PriorityQueueResult result = pqRemoveElement(em->event_list, (PQElement)temp_event);
    switch(result)
    {
    case PQ_NULL_ARGUMENT:
        return EM_EVENT_NOT_EXISTS;
    case PQ_ELEMENT_DOES_NOT_EXISTS:
        return EM_EVENT_NOT_EXISTS;
    case PQ_SUCCESS:
        return EM_SUCCESS;
    default:
        return EM_ERROR;
    }
    return EM_ERROR;
}

EventManagerResult emChangeEventDate(EventManager em, int event_id, Date new_date)
{
    if(!em || !new_date)
    {
        return EM_NULL_ARGUMENT;
    }
    if(event_id < MIN_EVENT_ID)
    {
        return EM_INVALID_EVENT_ID;
    }
    Event temp_event = NULL;
    PQ_FOREACH(Event, iter, em->event_list)
    {
        if(eventGetId(iter) == event_id)
        {
            temp_event = iter;
            break;
        }
    }
    if(!temp_event)
    {
        return EM_EVENT_ID_NOT_EXISTS;
    }
    if(dateCompare(new_date, em->init_date) > 0)
    {
        return EM_INVALID_DATE;
    }
    
    Event iterate_event = eventCreate(eventGetName(temp_event), eventGetId(temp_event), eventGetDate(temp_event));
    if(!iterate_event)
    {
        return EM_OUT_OF_MEMORY;
    }
    PQ_FOREACH(Event, iter, em->event_list)
    {
        if(eventCompare(iter, iterate_event))
        {
            eventDestroy(iterate_event);
            return EM_EVENT_ALREADY_EXISTS;
        }
    }
    PriorityQueueResult result = pqChangePriority(  em->event_list, 
                                                    (PQElement)temp_event, 
                                                    (PQElementPriority)eventGetDate(temp_event), 
                                                    (PQElementPriority)new_date);
    eventDestroy(iterate_event);                                                
    switch(result)
    {
    case PQ_NULL_ARGUMENT:
        return EM_NULL_ARGUMENT;
    case PQ_OUT_OF_MEMORY:
        return EM_OUT_OF_MEMORY;
    case PQ_SUCCESS:
        return EM_SUCCESS;
    default:
        return EM_ERROR;
    }
    return EM_ERROR;
}


EventManagerResult emAddMember(EventManager em, char* member_name, int member_id)
{
    if(!em || !member_name)
    {
        return EM_NULL_ARGUMENT;
    }
    if(member_id < MIN_MEMBER_ID)
    {
        return EM_INVALID_MEMBER_ID;
    }
    if(memberListContain(em->member_list, member_id))
    {
        return EM_MEMBER_ID_ALREADY_EXISTS;
    }
    Member to_add = memberCreate(member_name, member_id);
    if(!to_add)
    {
        return EM_OUT_OF_MEMORY;
    }
    if(!memberListInsert(em->member_list, to_add))
    {
        memberDestroy(to_add);
        return EM_OUT_OF_MEMORY;
    }
    memberDestroy(to_add);
    return EM_SUCCESS;
}


EventManagerResult emAddMemberToEvent(EventManager em, int member_id, int event_id)
{
    if(!em)
    {
        return EM_NULL_ARGUMENT;
    }
    if(member_id < MIN_MEMBER_ID)
    {
        return EM_INVALID_MEMBER_ID;
    }
    if(event_id < MIN_EVENT_ID)
    {
        return EM_INVALID_EVENT_ID;
    }
    Event event_ptr = NULL;
    PQ_FOREACH(Event, iter, em->event_list)
    {
        if(eventGetId(iter) == event_id)
        {
            event_ptr = iter;
            break;
        }
    }
    if(!event_ptr)
    {
        return EM_EVENT_ID_NOT_EXISTS;
    }
    if(!memberListContain(em->member_list, member_id))
    {
        return EM_MEMBER_ID_NOT_EXISTS;
    }
    if(memberListContain(eventGetMemberList(event_ptr), member_id))
    {
        return EM_EVENT_AND_MEMBER_ALREADY_LINKED;
    }
    Member temp_member = getMember(em->member_list, member_id);
    if(!temp_member)
    {
        return EM_OUT_OF_MEMORY;
    }
    char* temp_name = memberGetName(temp_member);
    Member new_member = memberCreate(temp_name, member_id);
    if(!new_member)
    {
        return EM_OUT_OF_MEMORY;
    }
    if(memberListInsert(eventGetMemberList(event_ptr), new_member))
    {
        memberListAddToEventNum(em->member_list, member_id, 1);
        memberDestroy(new_member);
        return EM_SUCCESS;
    }
    memberDestroy(new_member);
    return EM_OUT_OF_MEMORY;
}


EventManagerResult emRemoveMemberFromEvent(EventManager em, int member_id, int event_id)
{
    if(!em)
    {
        return EM_NULL_ARGUMENT;
    }
    if(member_id < MIN_MEMBER_ID)
    {
        return EM_INVALID_MEMBER_ID;
    }
    if(event_id < MIN_EVENT_ID)
    {
        return EM_INVALID_EVENT_ID;
    }
    Event event_ptr = NULL;
    PQ_FOREACH(Event, iter, em->event_list)
    {
        if(eventGetId(iter) == event_id)
        {
            event_ptr = iter;
            break;
        }
    }
    if(!event_ptr)
    {
        return EM_EVENT_ID_NOT_EXISTS;
    }
    if(!memberListContain(em->member_list, member_id))
    {
        return EM_MEMBER_ID_NOT_EXISTS;
    }
    if(!memberListContain(eventGetMemberList(event_ptr), member_id))
    {
        return EM_EVENT_AND_MEMBER_NOT_LINKED;
    }
    memberListRemove(eventGetMemberList(event_ptr), member_id);
    memberListAddToEventNum(em->member_list, member_id, -1);
    return EM_SUCCESS;
}


EventManagerResult emTick(EventManager em, int days)
{
    if(!em)
    {
        return EM_NULL_ARGUMENT;
    }
    if(days <= 0)
    {
        return EM_INVALID_DATE;
    }
    for(int i=0; i<days; i++)
    {
        dateTick(em->init_date);
    }
    PQ_FOREACH(Event, iter, em->event_list)
    {
        if(dateCompare(em->init_date, eventGetDate(iter)) > 0)
        {
            MemberList member_list = eventGetMemberList(iter);
            memberListUpdatePassedEvent(em->member_list, member_list);
            pqRemoveElement(em->event_list, (PQElement)iter);
        }
    }
    return EM_SUCCESS;
}


int emGetEventsAmount(EventManager em)
{
    return pqGetSize(em->event_list);
}


char* emGetNextEvent(EventManager em)
{
    if(!em)
    {
        return NULL;
    }
    Event event_ptr = (Event)pqGetFirst(em->event_list);
    return eventGetName(event_ptr);
}

void emPrintAllEvents(EventManager em, const char* file_name)
{
    if(!em || !file_name)
    {
        return;
    }
    FILE* fd = fopen(file_name, "w");
    if(!fd)
    {
        return;
    }
    PQ_FOREACH(Event, iter, em->event_list)
    {
        int day, month, year;
        int *day_ptr = &day, *month_ptr = &month, *year_ptr = &year;
        Date date = dateCopy(eventGetDate(iter));
        if(!date)
        {
            break;
        }
        if(!dateGet(date, day_ptr, month_ptr, year_ptr))
        {
            dateDestroy(date);
            break;
        }
        dateDestroy(date);
        fprintf(fd, "%s,%d.%d.%d", eventGetName(iter), day, month, year);
        printMemberList(eventGetMemberList(iter), fd);
        fprintf(fd, "\n");
    }
    fclose(fd);
}


void emPrintAllResponsibleMembers(EventManager em, const char* file_name)
{
    if(!em || !file_name)
    {
        return;
    }
    FILE* fd = fopen(file_name, "w");
    if(!fd)
    {
        return;
    }
    printMembersAndEventNum(em->member_list, fd);
    fclose(fd);
}


