#include "event.h"


struct Event_t{
    char* name;
    int event_id;
};


Event eventCreate(char* name, int event_id)
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
    strcpy(event_name, name);
    event->name = event_name;
    event->event_id = event_id;
    return event;
}


void eventDestroy(Event event)
{
    if(!event)
    {
        return;
    }
    free(event->name);
    free(event);
}



Event eventCopy(Event event)
{
    if(!event)
    {
        return NULL;
    }
    return eventCreate(event->name, event->event_id);
}



bool eventGet(Event event, char* name, int* event_id)
{
    if(!event || !name || !event_id)
    {
        return false;
    }
    strcpy(name, event->name);
    *event_id = event->event_id;
    return true;
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
