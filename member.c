#include "member.h"

/** Struct representing the member */
struct Member_t{
    char* name;
    int member_id;
    int event_num;
};


Member memberCreate(char* name, int member_id)
{
    if(!name || member_id < 0)
    {
        return NULL;
    }
    Member member = malloc(sizeof(*member));
    if(!member)
    {
        return NULL;
    }
    char* member_name = malloc(strlen(name)+1);
    if(!member_name)
    {
        free(member);
        return NULL;
    }    
    strcpy(member_name, name);
    member->name = member_name;
    member->member_id = member_id;
    member->event_num = 0;
    return member;
}


void memberDestroy(Member member)
{
    if(!member)
    {
        return;
    }
    free(member->name);
    free(member);
}



Member memberCopy(Member member)
{
    if(!member)
    {
        return NULL;
    }
    Member new_member = memberCreate(member->name, member->member_id);
    if(!new_member)
    {
        return NULL;
    }
    new_member->event_num = member->event_num;
    return new_member;
}



bool memberGet(Member member, char* name, int* member_id)
{
    if(!member || !name || !member_id)
    {
        return false;
    }
    strcpy(name, member->name);
    *member_id = member->member_id;
    return true;
}

char* memberGetName(Member member)
{
    if(!member)
    {
        return NULL;
    }
    return member->name;
}

const int memberGetId(Member member)
{
    if(!member)
    {
        return -1;
    }
    return member->member_id;
}


bool memberEqual(Member member1, Member member2)
{
    if(!member1 || !member2)
    {
        return false;
    }
    if(member1->member_id == member2->member_id)
    {
        return true;
    }
    return false;
}

bool memberCompare(Member member1, Member member2)
{
    if(!member1 || !member2)
    {
        return false;
    }
    if(strcmp(memberGetName(member1), memberGetName(member2)) == 0 && memberEqual(member1, member2))
    {
        return true;
    }
    return false;
}


void memberSetNumEvent(Member member, int n)
{
    if(!member)
    {
        return;
    }    
    member->event_num =  n;
    //printf("^^^%d %d %d\n",n , memberGetId(member),memberGetEventNum(member));

    return;
}


int memberGetEventNum(Member member)
{
    if(!member)
    {
        return -1;
    }
    return member->event_num;
}
