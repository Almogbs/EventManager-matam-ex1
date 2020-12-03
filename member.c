#include "member.h"
#include <assert.h>


/** Struct representing the member(list) */
struct Member_t {
    char* name;
    int id;
    struct Member_t* next;
};


Member memberCreate(char* name, int id)
{
    if(!name)
    {
        return NULL;
    }
    Member member = malloc(sizeof(*member));
    if(!member)
    {
        return NULL;
    }
    char* new_name = malloc((int)strlen(name)+1);
    if(!new_name)
    {
        free(member);
        return NULL;
    }
    strcpy(new_name, name);
    member->name = new_name;
    member->id = id;
    member->next = NULL;
    return member;
}


void memberDestroy(Member member)
{
    if(!member)
    {
        return;
    }
    while(member)
    {
        Member to_delete = member;
        member = member->next;
        free(to_delete->name);
        free(to_delete);
    }
}


bool memberInsert(Member member, Member to_add)
{
    if(!to_add)
    {
        return false;
    }
    Member new_member = memberCopy(to_add);
    if(!new_member)
    {
        return false;
    }
    if(!member)
    {
        member = new_member;
        return true;
    }
    new_member->next = member;
    member = new_member;
    return true;
}


const Member getMember(Member member, int member_id)
{
    if(!member)
    {
        return NULL;
    }
    Member temp_member = member;
    while(temp_member)
    {
        if(temp_member->id == member_id)
        {
            return temp_member;
        }
        temp_member = temp_member->next;
    }
    return NULL;
}


Member memberCopy(Member member)
{
    if(!member)
    {
        return NULL;
    }
    Member new_member = memberCreate(member->name, member->id);
    if(!new_member)
    {
        return NULL;
    }
    return new_member;
}


bool memberContain(Member member, int id)
{
    if(!member)
    {
        return false;
    }
    Member temp_member = member;
    while(temp_member)
    {
        if(temp_member->id == id)
        {
            return true;
        }
        temp_member = temp_member->next;
    }
    return false;
}


void memberRemove(Member member, int id)
{
    if(!member)
    {
        return;
    }
    if(member->id == id)
    {
        Member to_remove = member;
        member = member->next;
        free(to_remove->name);
        free(to_remove);
        return;
    }
    Member temp_member = member;
    while(temp_member->next)
    {
        if(temp_member->next->id == id)
        {
            Member to_delete = temp_member->next;
            temp_member->next = to_delete->next;
            free(to_delete->name);
            free(to_delete);
            return;
        }
        temp_member = temp_member->next;
    }
    return;
}


void printMembers(Member member, FILE* fd)
{
    if(!member || !fd)
    {
        return;
    }
    Member temp_member = member;
    while(temp_member)
    {
        fprintf(fd, ",%s", temp_member->name);
        temp_member = temp_member->next;    
    }
}
