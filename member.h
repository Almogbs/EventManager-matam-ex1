#ifndef MEMBER_H_
#define MEMBER_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "priority_queue.h"

/** Constants to Valideting members information */
#define MIN_MEMBER_ID 0


/** Type for defining the member */
typedef struct Member_t *Member;

/**
* memberCreate: Allocates a new member.
*
* @param name - the name of the member.
* @param member_id - the id of the member.
* @return
* 	NULL - if allocation failed or member is illegal.
* 	A new Member in case of success.
*/
Member memberCreate(char* name, int member_id);


/**
* memberDestroy: Deallocates an existing member.
*
* @param member - Target member to be deallocated. If member is NULL nothing will be done
*/
void memberDestroy(Member member);


/**
* memberCopy: Creates a copy of target member.
*
* @param member - Target member.
* @return
* 	NULL if a NULL was sent or a memory allocation failed.
* 	A member containing the same elements as member otherwise.
*/
Member memberCopy(Member member);


/**
* memberGet: Returns the name, the id and the date of the member.
*
* @param member - Target member
* @param name - the pointer to assign to name of the member into.
* @param member_id - the pointer to assign to id of the member into.
* @param date - the pointer to assign to date of the member into.
* @return
* 	false if one of pointers is NULL.
* 	Otherwise true and the member is assigned to the pointers.
*/
bool memberGet(Member member, char* name, int* member_id);


/**
* memberGetName: Returns the name of the member.
*
* @param member - Target member.
* @return
* 	NULL if a NULL was sent.
* 	Otherwise pointer to the name of the member.
*/
char* memberGetName(Member member);


/**
* memberGetId: Returns the id of the member.
*
* @param member - Target member.
* @return
* 	-1 if a NULL was sent.
* 	Otherwise const pointer to the id of the member.
*/
const int memberGetId(Member member);



/**
* memberEqual: Checks if the members have same id.
*
* @return
* 		True if same id.
* 		Otherwise, false.
*/
bool memberEqual(Member member1, Member member2);


/**
* memberCompare: Checks if the members have same id and name.
*
* @return
* 		True if same id and name.
* 		Otherwise, false.
*/
bool memberCompare(Member member1, Member member2);

int memberGetEventNum(Member member);
void memberSetNumEvent(Member member, int n);

#endif //MEMBER_H_
