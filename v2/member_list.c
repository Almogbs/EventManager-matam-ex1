#include "member_list.h"
#include "priority_queue.h"


typedef struct MemberPriority_t{
    int member_id;
    int num_event;
} *MemberPriority;

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

static PQElementPriority copyInfo(PQElementPriority member_priority)
{
    if (!member_priority) 
    {
        return NULL;
    }
    MemberPriority temp = (MemberPriority)member_priority;
    MemberPriority copy = malloc(sizeof(*copy));
    if (!copy)
    {
        return NULL;
    }
    copy->member_id = temp->member_id;
    copy->num_event = temp->num_event;
    return (PQElementPriority)copy;
}

static void freeInfo(PQElementPriority member_priority)
{
    if(!member_priority)
    {
        return;
    }
    free(member_priority);
}

static int compareInfo(PQElementPriority member_info1, PQElementPriority member_info2)
{
    if(!member_info1 || !member_info2) 
    {                                           
        return 0;
    }
    MemberPriority member_priority1 = (MemberPriority)member_info1;
    MemberPriority member_priority2 = (MemberPriority)member_info2;
    if(member_priority1->num_event == member_priority2->num_event)
    {
        return (member_priority2->member_id - member_priority1->member_id);
    }
    return (member_priority1->num_event - member_priority2->num_event);
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
    PriorityQueue member_queue = pqCreate(copyMember, freeMember, equalMember, copyInfo, freeInfo, compareInfo);
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
    MemberPriority member_priority = malloc(sizeof(*member_priority));
    if(!member_priority)
    {
        return false;
    } 
    member_priority->member_id = memberGetId(to_add);
    member_priority->num_event = 0;
    if(pqInsert(member_list->member_queue, (PQElement)new_member, (PQElementPriority)member_priority) == PQ_SUCCESS)
    {
        free(member_priority);
        memberDestroy(new_member);
        return true;
    }
    free(member_priority);
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
    return;
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
    Member temp_member = memberCopy(getMember(member_list, member_id));
    if(!temp_member)
    {
        return;
    }
    int old_num = memberGetEventNum(getMember(member_list, member_id));
    int new_num = memberGetEventNum(getMember(member_list, member_id)) + n;
    MemberPriority member_priority_new = malloc(sizeof(*member_priority_new));
    if(!member_priority_new)
    {
        memberDestroy(temp_member);
        return;
    }    
    MemberPriority member_priority_old = malloc(sizeof(*member_priority_old));
    if(!member_priority_old)
    {
        memberDestroy(temp_member);
        free(member_priority_new);
        return;
    }
    member_priority_new->member_id = member_id;
    member_priority_new->num_event = new_num;
    member_priority_old->member_id = member_id;
    member_priority_old->num_event = old_num;
    pqChangePriority(   member_list->member_queue, 
                        (PQElement)temp_member, 
                        (PQElementPriority)member_priority_old,
                        (PQElementPriority)member_priority_new);
    memberSetNumEvent(getMember(member_list, member_id), new_num);
    memberDestroy(temp_member);
    free(member_priority_new);
    free(member_priority_old);
    return;
}


void memberListUpdatePassedEvent(MemberList member_list1, MemberList member_list2)
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
    if(!member_list || !fd)
    {
        return;
    }
    PQ_FOREACH(Member, iterator, member_list->member_queue)
    {
        fprintf(fd, ",%s", memberGetName(iterator));
    }
    return;
}

void printMembersAndEventNum(MemberList member_list, FILE* fd)
{
    if(!member_list || !fd)
    {
        return;
    }
    PQ_FOREACH(Member, iter, member_list->member_queue)
    {
        if(memberGetEventNum(iter) == 0)
        {
            break;
        }
        fprintf(fd, "%s,%d\n", memberGetName(iter), memberGetEventNum(iter));
    }
    return;
}