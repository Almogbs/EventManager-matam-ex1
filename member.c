#include "member.h"
#include <assert.h>

struct Member_t {
    char* name;
    int id;
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
    Member new_member = memberCreate(member->name, member->id);
    if(!new_member)
    {
        return NULL;
    }
    return new_member;
}

bool memberIdGet(Member member, int* id)
{
    if(!member || !id)
    {
        return false;
    }
    *id = member->id;
    return true;
}

bool memberIdCompare(Member member1, Member member2)
{
    if(!member1 || !member2)
    {
        return false;
    }
    if(member1->id == member2->id)
    {
        return true;
    }
    return false;
}

void printMember(Member member)
{
    printf("name: %s, id: %d\n", member->name, member->id);
}
