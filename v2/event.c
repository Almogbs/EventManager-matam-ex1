#include "event.h"

/** Struct representing the event */
struct Event_t{
    char* name;
    int event_id;
    Date date;
    MemberList member_list;
};


Event eventCreate(char* name, int event_id, Date date)
{
    if(!name || event_id < 0)
    {
        return NULL;
    }
    Event event = malloc(sizeof(*event));
    if(!event)
    {
        return NULL;
    }
    char* event_name = malloc(strlen(name)+1);
    if(!event_name)
    {
        free(event);
        return NULL;
    }
    Date event_date = dateCopy(date);
    if(!event_date)
    {
        free(event);
        free(event_name);
        return NULL;
    }
    
    strcpy(event_name, name);
    event->name = event_name;
    event->event_id = event_id;
    event->date = event_date;
    event->member_list = NULL;
    return event;
}


void eventDestroy(Event event)
{
    if(!event)
    {
        return;
    }
    free(event->name);
    dateDestroy(event->date);
    memberListDestroy(event->member_list);
    free(event);
}


Event eventCopy(Event event)
{
    if(!event)
    {
        return NULL;
    }
    return eventCreate(event->name, event->event_id, event->date);
}


bool eventGet(Event event, char* name, int* event_id, Date date)
{
    if(!event || !name || !event_id || !date)
    {
        return false;
    }
    strcpy(name, event->name);
    *event_id = event->event_id;
    date = event->date;
    return true;
}


char* eventGetName(Event event)
{
    if(!event)
    {
        return NULL;
    }
    return event->name;
}


const int eventGetId(Event event)
{
    if(!event)
    {
        return -1;
    }
    return event->event_id;
}


const Date eventGetDate(Event event)
{
    if(!event)
    {
        return NULL;
    }
    return event->date;
}


const MemberList eventGetMemberList(Event event)
{
    if(!event)
    {
        return NULL;
    }
    return event->member_list;
}


bool eventEqual(Event event1, Event event2)
{
    if(!event1 || !event2)
    {
        return false;
    }
    if(event1->event_id == event2->event_id)
    {
        return true;
    }
    return false;
}


bool eventCompare(Event event1, Event event2)
{
    if(!event1 || !event2)
    {
        return false;
    }
    if(strcmp(eventGetName(event1), eventGetName(event2)) == 0 && dateCompare(event1->date, event2->date) == 0)
    {
        return true;
    }
    return false;
}

