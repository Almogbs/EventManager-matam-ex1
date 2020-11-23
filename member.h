#ifndef MEMBER_H_
#define MEMBER_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN_ID 0


/** Type for defining the member */
typedef struct Member_t *Member;

/**
* memberCreate: Allocates a new member.
*
* @param name - the name of the member.
* @param id - the id of the member.
* @return
* 	NULL - if allocation failed.
* 	A new Member in case of success.
*/
Member memberCreate(char* name, int id);


/**
* memberDestroy: Deallocates an existing Member.
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
* memberIdGet: Returns the name and id of a member
*
* @param member - Target member
* @param id - the pointer to assign to id of the member into.
*
* @return
* 	false if one of pointers is NULL.
* 	Otherwise true and the member is assigned to the pointers.
*/
bool memberIdGet(Member member, int* id);

/**
* memberIdCompare: compare the member's id
*
* @return
* 		True if members have same id;
* 		Otherwise, false (include NULL members).
*/
bool memberIdCompare(Member member1, Member member2);

//DELETE ME!!!!!
void printMember(Member member);

#endif /**  MEMBER_H_  */

