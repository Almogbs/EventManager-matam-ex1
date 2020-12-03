#ifndef MEMBER_H_
#define MEMBER_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/** Constants to Valideting members id */
#define MIN_ID 0

/*
typedef enum MemberResult_t {
    MEMBER_SUCCESS,
    MEMBER_OUT_OF_MEMORY,
    MEMBER_INVALID_ID,
    MEMBER_ALREADY_EXISTS,
    MEMBER_NOT_EXISTS
} MemberResult;
*/

/** Type for defining the member */
typedef struct Member_t *Member;

/**
* memberCreate: Allocates a new member list with the member that specified at the head.
*
* @param name - the name of the member.
* @param id - the id of the member.
* @return
* 	NULL - if allocation failed.
* 	A new Member list with the Member in case of success.
*/
Member memberCreate(char* name, int id);


/**
* memberDestroy: Deallocates an existing Member list.
*
* @param member - Target member list to be deallocated. If member is NULL nothing will be done
*/
void memberDestroy(Member member);



/**
* memberCopy: Creates a copy of the first member in list.
*
* @param member - Target member list.
* @return
* 	NULL if a NULL was sent or a memory allocation failed.
* 	otherwise, a member containing the same elements as the first member in member list, with NULL as next member.
*/
Member memberCopy(Member member);


/**
* memberInsert: insert to_add member to the member list.
*
* @param member - Target member list.
* @param to_add - The member which to be inserted.
* @return
* 	false - if allocation failed or NULL argument.
*   Otherwise true and the member was added.
*/
bool memberInsert(Member member, Member to_add);


/**
* memberContain: Checks if member with specified id is on the member list.
*
* @param member - Target member list.
* @param id - the id of the member.
* @return
* 	false if one of pointers is NULL or if there is no member in the list with the id.
* 	Otherwise true.
*/
bool memberContain(Member member, int id);


/**
* memberRemove: Removes the member with the specified id from the member list.
                If there is no member with the if, nothing will happen.
*
* @param member - Target member list.
* @param id - the id to of the member to remove.
*
*/
void memberRemove(Member member, int id);


/**
* getMember: Returns a member within the member list, who have the specifeid member_id .
*
* @param member - Target member list.
* @param member_id - the id to of the member to get.
*
* @return
*   NULL if a NULL was sent or there is no member with the specifeid id in the member list.
*   Otherwise return const pointer to the member
*/
const Member getMember(Member member, int member_id);


/**
* printMembers: prints the name of the members in list to the open fd file.
*               If NULL was sent or file is not open in read mode - nothing will happen.
*
* @param member - Target member list.
* @param fd - the open file to print.
*/
void printMembers(Member member, FILE* fd);

#endif /**  MEMBER_H_  */

