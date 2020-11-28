
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "event_manager.h"
#include "priority_queue.h"
#include "date.h"
#include "member.h"
#include "event.h"

/**
 *   Functions for operating on Event elements & Date priorety element
 *   in the generic ADT priorety queue
 */

static PQElement copyEvent(PQElement event)
{
    return eventCopy((Event*)event);
}

static void freeEvent(PQElement event)
{
    eventDestroy((Event*)event);
}

static bool equalEvent(PQElement event1, PQElement event2)
{
    return eventEqual((Event*)event1, (Event*)event2);
}

static PQElementPriority copyDate(PQElementPriority date)
{
    return dateCopy((Date*)date);
}

static void freeDate(PQElementPriority date)
{
    dateDestroy((Date*)date);
}

static int compareDate(PQElementPriority date1, PQElementPriority date2)
{
    return dateCompare((Date*)date1, (Date*)date2);
}

// Struct for Event Manager
struct EventManager_t{
    Date init_date;
    PriorityQueue event_list;
    Member member_list;
};

EventManager createEventManager(Date date)
{
    if(!date || isValidDate(date) == false)
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
    em->event_list = pq;
    em->init_date = init_date;
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
    memberDestroy(em->member_list);
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
    Event new_event = eventCreate(event_name, event_id);
    if(!new_event)
    {
        return EM_OUT_OF_MEMORY;
    }


}