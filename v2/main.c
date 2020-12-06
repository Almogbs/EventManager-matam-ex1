#include "event_manager.h"

int main()
{
    Date date = dateCreate(1,1,1999);
    Date date1 = dateCreate(1,1,1999);
    Date date2 = dateCreate(1,2,1999);
    Date date3 = dateCreate(12,3,1999);
    Date date4 = dateCreate(4,1,1999);
    EventManager em = createEventManager(date);
    emAddEventByDate(em, "bla", date1, 1234);
    emAddEventByDate(em, "bon", date2, 2311);
    emAddEventByDate(em, "bak", date3, 4321);
    emAddEventByDate(em, "bor", date4, 3124);
    emAddEventByDiff(em, "paka", 22, 4444);
    emAddEventByDiff(em, "poke", 366, 1111);
    emAddMember(em, "Almog", 22);
    emAddMember(em, "Ben", 11);
    emAddMember(em, "Shaul", 1);
    emAddMember(em, "Bobby", 33);
    emAddMember(em, "Fischer", 3);
    emAddMember(em, "Paul", 2);
    emAddMember(em, "Morphy", 4);
    emAddMember(em, "Garry", 7);
    emAddMember(em, "Kasparov", 5);
    emAddMemberToEvent(em, 2, 1111);
    emAddMemberToEvent(em, 1, 1111);
    emAddMemberToEvent(em, 3, 1111);
    emAddMemberToEvent(em, 4, 1111);
    emAddMemberToEvent(em, 5, 1111);
    emAddMemberToEvent(em, 7, 1111);
    emAddMemberToEvent(em, 22, 1111);
    emAddMemberToEvent(em, 11, 1111);
    emAddMemberToEvent(em, 33, 1111);

    emPrintAllEvents(em, "file1");
    emPrintAllResponsibleMembers(em, "file2");

    dateDestroy(date);
    dateDestroy(date1);
    dateDestroy(date2);
    dateDestroy(date3);
    dateDestroy(date4);
    destroyEventManager(em);


    return 0;
}