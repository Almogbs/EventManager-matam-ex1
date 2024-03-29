#ifndef EVENT_H_
#define EVENT_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "date.h"
#include "member.h"
#include "member_list.h"

/** Constants to Valideting events information */
#define MIN_EVENT_ID 0


/** Type for defining the event */
typedef struct Event_t *Event;

/**
* eventCreate: Allocates a new event.
*
* @param name - the name of the event.
* @param event_id - the id of the event.
* @param date - the date of the event.
* @return
* 	NULL - if allocation failed or event is illegal.
* 	A new Event in case of success.
*/
Event eventCreate(char* name, int event_id, Date date);


/**
* eventDestroy: Deallocates an existing event.
*
* @param event - Target event to be deallocated. If event is NULL nothing will be done
*/
void eventDestroy(Event event);


/**
* eventCopy: Creates a copy of target event.
*
* @param event - Target event.
* @return
* 	NULL if a NULL was sent or a memory allocation failed.
* 	A event containing the same elements as event otherwise.
*/
Event eventCopy(Event event);


/**
* eventGet: Returns the name, the id and the date of the event.
*
* @param event - Target event
* @param name - the pointer to assign to name of the event into.
* @param event_id - the pointer to assign to id of the event into.
* @param date - the pointer to assign to date of the event into.
* @return
* 	false if one of pointers is NULL.
* 	Otherwise true and the event is assigned to the pointers.
*/
bool eventGet(Event event, char* name, int* event_id, Date date);


/**
* eventGetName: Returns the name of the event.
*
* @param event - Target event.
* @return
* 	NULL if a NULL was sent.
* 	Otherwise pointer to the name of the event.
*/
char* eventGetName(Event event);


/**
* eventGetDate: Returns the date of the event.
*
* @param event - Target event.
* @return
* 	NULL if a NULL was sent.
* 	Otherwise pointer to the date of the event.
*/
const Date eventGetDate(Event event);


/**
* eventGetId: Returns the id of the event.
*
* @param event - Target event.
* @return
* 	-1 if a NULL was sent.
* 	Otherwise const pointer to the id of the event.
*/
const int eventGetId(Event event);


/**
* eventGetMember: Returns the Member list of the event.
*
* @param event - Target event.
* @return
* 	NULL if a NULL was sent.
* 	Otherwise const pointer to the head the Member list of the event.
*/
const MemberList eventGetMemberList(Event event);


/**
* eventEqual: Checks if the events have same id.
*
* @param event1 - First event.
* @param event2 - Second event.
* @return
* 		True if same id.
* 		Otherwise, false.
*/
bool eventEqual(Event event1, Event event2);


/**
* eventCompare: Checks if the events have same date and name.
*
* @return
* 		True if same id and name.
* 		Otherwise, false.
*/
bool eventCompare(Event event1, Event event2);

/**
* eventChangeDate: Change the date of the event to new_date.
*
* @param event - Target event.
* @param new_date - The new date.
*/
void eventChangeDate(Event event, Date new_date);


#endif //EVENT_H_
