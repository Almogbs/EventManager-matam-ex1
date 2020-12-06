#ifndef MEMBER_LIST_H_
#define MEMBER_LIST_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "member.h"

#define NO_EVENTS 0


/** Type for defining the member list*/
typedef struct MemberList_t *MemberList;

/**
* memberListCreate: Allocates a new empty member list.
*
* @return
* 	NULL - if allocation failed.
* 	A new Member list in case of success.
*/
MemberList memberListCreate();


/**
* memberListDestroy: Deallocates an existing Member list.
*
* @param member_list - Target member list to be deallocated. If member list is NULL nothing will be done
*/
void memberListDestroy(MemberList member_list);



/**
* memberListCopy: Creates a copy of the member list.
*
* @param member_list - Target member list.
* @return
* 	NULL if a NULL was sent or a memory allocation failed.
* 	otherwise, a copy of the member list.
*/
MemberList memberListCopy(MemberList member_list);


/**
* memberListInsert: insert to_add member to the member list.
*
* @param member_list - Target member list.
* @param to_add - The member which to be inserted.
* @return
* 	false - if allocation failed or NULL argument.
*   Otherwise true and the member was added.
*/
bool memberListInsert(MemberList member_list, Member to_add);


/**
* memberListContain: Checks if member with specified id is on the member list.
*
* @param member_list - Target member list.
* @param id - the id of the member.
* @return
* 	false if one of pointers is NULL or if there is no member in the list with the id.
* 	Otherwise true.
*/
bool memberListContain(MemberList member_list, int id);


/**
* memberListRemove: Removes the member with the specified id from the member list.
                If there is no member with the if, nothing will happen.
*
* @param member_list - Target member list.
* @param id - the id to of the member to remove.
*
*/
void memberListRemove(MemberList member_list, int id);


/**
* getMember: Returns a member within the member list, who have the specifeid member_id .
*
* @param member_list - Target member list.
* @param member_id - the id to of the member to get.
*
* @return
*   NULL if a NULL was sent or there is no member with the specifeid id in the member list.
*   Otherwise return const pointer to the member
*/
const Member getMember(MemberList member_list, int member_id);


/**
* printMemberList: prints the name of the members in list to the open fd file.
*               If NULL was sent or file is not open in read mode - nothing will happen.
*
* @param member_list - Target member list.
* @param fd - the open file to print.
*/
void printMemberList(MemberList member_list, FILE* fd);

void printMembersAndEventNum(MemberList member_list, FILE* fd);


void memberListAddToEventNum(MemberList member_list, int member_id, int n);
void memberListUpdatePassedEvent(MemberList member_list1, MemberList member_list2);

#endif /**  MEMBER_LIST_H_  */

