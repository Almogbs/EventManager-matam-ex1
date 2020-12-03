#include "member_list.h"
#include "priority_queue.h"


/**
 *   Functions for operating on Member elements & Int priorety element
 *   in the generic ADT priorety queue
 */
static PQElement copyMember(PQElement member)
{
    return (PQElement)memberCopy((Member)member);
}

static void freeMember(PQElement member)
{
    memberDestroy((Member)member);
}

static bool equalMember(PQElement member1, PQElement member2)
{
    return memberEqual((Member)member1, (Member)member2);
}

static PQElementPriority copyInt(PQElementPriority event_num)
{
    if (!event_num) {
        return NULL;
    }
    int *copy = malloc(sizeof(*copy));
    if (!copy) {
        return NULL;
    }
    *copy = *(int *) event_num;
    return copy;
}

static void freeInt(PQElementPriority event_num)
{
    if(!event_num)
    {
        return;
    }
    free(event_num);
}

static int compareInt(PQElementPriority event_num1, PQElementPriority event_num2)
{
    return *(int*)event_num1 - *(int*)event_num2;
}


// Struct for Member Manager
struct MemberList_t{
    PriorityQueue member_queue;
};


MemberList memberListCreate()
{
    MemberList member_list = malloc(sizeof(*member_list));
    if(!member_list)
    {
        return NULL;
    }
    PriorityQueue member_queue = pqCreate(copyMember, freeMember, equalMember, copyInt, freeInt, compareInt);
    if(!member_queue)
    {
        free(member_list);
        return NULL;
    }
    member_list->member_queue = member_queue;
    return member_list;
}


void memberListDestroy(MemberList member_list)
{
    if(!member_list)
    {
        return;
    }
    pqDestroy(member_list->member_queue);
    free(member_list);
}



MemberList memberListCopy(MemberList member_list)
{
    if(!member_list)
    {
        return NULL;
    }
    MemberList copy_member_list = malloc(sizeof(*copy_member_list));
    if(!copy_member_list)
    {
        return NULL;
    }
    copy_member_list->member_queue = pqCopy(member_list->member_queue);
    if(!copy_member_list->member_queue)
    {
        return NULL;
    }

    return copy_member_list;
}


bool memberListInsert(MemberList member_list, Member to_add)
{
    if(!member_list || !to_add)
    {
        return false;
    }
    Member new_member = memberCopy(to_add);
    if(!new_member)
    {
        return false;
    }
    int event_num = NO_EVENTS;
    if(pqInsert(member_list->member_queue, new_member, &event_num) == PQ_SUCCESS)
    {
        memberDestroy(new_member);
        return true;
    }
    memberDestroy(new_member);
    return false;
}


bool memberListContain(MemberList member_list, int id)
{
    if(!member_list)
    {
        return false;
    }
    PQ_FOREACH(Member, iter, member_list->member_queue)
    {
        if(memberGetId(iter) == id)
        {
            return true;
        }
    }
    return false;
}


void memberListRemove(MemberList member_list, int id)
{
    if(!member_list)
    {
        return;
    }
    PQ_FOREACH(Member, iter, member_list->member_queue)
    {
        if(memberGetId(iter) == id && pqRemoveElement(member_list->member_queue, (PQElement)iter) == PQ_SUCCESS)
        {
            return;
        }
    }
    return ;
}


const Member getMember(MemberList member_list, int member_id)
{
    if(!member_list)
    {
        return NULL;
    }
    PQ_FOREACH(Member, iter, member_list->member_queue)
    {
        if(memberGetId(iter) == member_id)
        {
            return iter;
        }
    }
    return NULL;
}

void memberListAddToEventNum(MemberList member_list, int member_id, int n)
{
    if(!member_list)
    {
        return;
    }
    Member temp_member = getMember(member_list, member_id);
    if(!temp_member)
    {
        return;
    }
    int old_num = memberGetEventNum(temp_member);
    int new_num = memberGetEventNum(temp_member) + n;
    pqChangePriority(   member_list->member_queue, 
                        (PQElement)temp_member, 
                        (PQElementPriority)(&old_num),
                        (PQElementPriority)(&new_num));
    return;
    }


void update(MemberList member_list1, MemberList member_list2)
{
    if(!member_list1 || !member_list2)
    {
        return;
    }
    PQ_FOREACH(Member, iter, member_list2->member_queue)
    {
        memberListAddToEventNum(member_list1, memberGetId(iter), -1);
    }
}

void printMemberList(MemberList member_list, FILE* fd)
{
    if(!member_list || fd)
    {
        return;
    }
    PQ_FOREACH(Member, iter, member_list->member_queue)
    {
        fprintf(fd, ",%s", memberGetName(iter));
    }
    return;
}