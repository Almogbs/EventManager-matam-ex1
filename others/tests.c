#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "event_manager.h"
#include "date.h"

#define OK_EVENT_NAME "event_name"
#define OK_EVENT_ID 50

/**
 * Evaluates expr and continues if expr is true.
 * If expr is false, ends the test by returning false and prints a detailed
 * message about the failure.
 */
#define ASSERT_TEST(expr, goto_label)                                                         \
     do {                                                                          \
         if (!(expr)) {                                                            \
             printf("\nAssertion failed at %s:%d %s ", __FILE__, __LINE__, #expr); \
             result = false;                                                       \
             goto goto_label;                                                         \
         }                                                                         \
     } while (0)

/**
 * Macro used for running a test from the main function
 */
#define RUN_TEST(test, name)                  \
    do {                                 \
      printf("+ Running <b>%s</b> ... ", name);   \
        if (test()) {                    \
            printf("[OK]\n");            \
        } else {                         \
            printf("[Failed]\n\n <span>To see what the test does and why it failed, please check the link at the top of the page to the test file</span>");        \
        }                                \
    } while (0)

#define ASSERT(expr) ASSERT_TEST(expr, destroy)


/* ===== Helper Functions ===== */
EventManager createEM(int day, int month, int year) {
    Date date = dateCreate(day, month, year);
    EventManager em = createEventManager(date);
    dateDestroy(date);
    return em;
}

void writeOutputToFile(char *file_name, char *file_content) {
    char *new_name = malloc(strlen(file_name) + strlen("expected_") + 1);
    strcpy(new_name, "expected_");
    strcat(new_name, file_name);
    FILE *file = fopen(new_name, "w");
    if (!file) {
        return;
    }

    fprintf(file, "%s", file_content);
    free(new_name);
    fclose(file);
}

bool isFilePrintOutputCorrect(char *file_name, char *expected_output) {
    FILE *infile;
    char *buffer;
    long numbytes;

    infile = fopen(file_name, "r");

    if (infile == NULL)
        return false;

    fseek(infile, 0L, SEEK_END);
    numbytes = ftell(infile);
    fseek(infile, 0L, SEEK_SET);

    buffer = (char *) calloc(numbytes + 1, sizeof(char));

    if (buffer == NULL)
        return false;

    fread(buffer, sizeof(char), numbytes, infile);
    fclose(infile);


    writeOutputToFile(file_name, expected_output);
    printf("<br>&nbsp;&nbsp;&nbsp;&nbsp;> Printing output: <a href='/staging/{STAGING_ID}/%s'>%s</a> | Expected output: <a href='/staging/{STAGING_ID}/expected_%s'>expected_%s</a> (Might be correct)",
           file_name, file_name, file_name, file_name);
    bool result = (strncmp(buffer, expected_output, numbytes) == 0);
    free(buffer);
    return result;
}




/* ===== TESTING createEventManager ===== */
bool testEMCreateNullArgument() {
    bool result = true;
    EventManager em = createEventManager(NULL);
    ASSERT(em == NULL);

    destroy:
    destroyEventManager(em);
    return result;
}

bool testEMCreateStandardTest() {
    bool result = true;
    EventManager em = createEM(1, 1, 2000);
    ASSERT(em != NULL);

    destroy:
    destroyEventManager(em);
    return result;
}

bool testEMDestroyNullArgument() {
    destroyEventManager(NULL);
    return true;
}



/* ===== TESTING emAddEventByDate===== */
bool testEMAddEventByDateCopiesDate() {
    bool result = true;
    Date date = dateCreate(2, 2, 2000);
    EventManager em = createEM(1, 1, 2000);
    ASSERT(emAddEventByDate(em, "event1", date, 1) == EM_SUCCESS);
    ASSERT(emAddEventByDate(em, "event2", date, 2) == EM_SUCCESS);

    destroy:
    destroyEventManager(em);
    dateDestroy(date); // If this fails it probably means you didnt copy the date with dateCopy
    return result;
}

bool testEMAddEventByDateCopiesEventName() {
    bool result = true;
    Date date = NULL;
    EventManager em = createEM(1, 1, 2000);

    Date date1 = dateCreate(2, 2, 2020);
    char name[] = "event_name";
    ASSERT(emAddEventByDate(em, name, date1, 5) == EM_SUCCESS);
    strcpy(name, "chngd_name");
    ASSERT(emAddEventByDate(em, "event_name", date1, 5) == EM_EVENT_ALREADY_EXISTS);

    destroy:
    dateDestroy(date);
    dateDestroy(date1);
    destroyEventManager(em);
    return result;
}


bool testEMAddEventByDateBasicUsageTests() {
    bool result = true;
    Date date = NULL;
    EventManager em = createEM(1, 1, 2000);

    // Multiple events in same day
    date = dateCreate(2, 2, 2000);
    ASSERT(emAddEventByDate(em, "event1", date, 1) == EM_SUCCESS);
    ASSERT(emAddEventByDate(em, "event2", date, 2) == EM_SUCCESS);
    dateDestroy(date);

    // More Events
    date = dateCreate(2, 2, 2000);
    ASSERT(emAddEventByDate(em, "event3", date, 3) == EM_SUCCESS);

    destroy:
    dateDestroy(date);
    destroyEventManager(em);
    return result;
}

bool testEMAddEventByDateBasicParameterErrors() {
    bool result = true;
    Date date = NULL;
    EventManager em = createEM(1, 1, 2000);

    ASSERT(emAddEventByDate(em, NULL, NULL, 29) == EM_NULL_ARGUMENT);

    // Past Date
    date = dateCreate(30, 12, 1999);
    ASSERT(emAddEventByDate(em, OK_EVENT_NAME, date, OK_EVENT_ID) == EM_INVALID_DATE);
    dateDestroy(date);

    // Negative event_id
    date = dateCreate(5, 5, 2000);
    ASSERT(emAddEventByDate(em, OK_EVENT_NAME, date, -1) == EM_INVALID_EVENT_ID);
    ASSERT(emAddEventByDate(em, OK_EVENT_NAME, date, 0) == EM_SUCCESS); // 0 is not negative

    // Event already exists in this day
    ASSERT(emAddEventByDate(em, OK_EVENT_NAME, date, OK_EVENT_ID) == EM_EVENT_ALREADY_EXISTS);
    dateDestroy(date);

    // Event id already exists
    date = dateCreate(6, 6, 2000);
    ASSERT(emAddEventByDate(em, OK_EVENT_NAME, date, 0) == EM_EVENT_ID_ALREADY_EXISTS);

    destroy:
    dateDestroy(date);
    destroyEventManager(em);
    return result;
}

bool testEMAddEventByDateBasicErrorsAreInCorrectOrder() {
    bool result = true;
    Date date = NULL;
    date = dateCreate(1, 1, 1999);
    EventManager em = createEM(1, 1, 2000);

    ASSERT(emAddEventByDate(NULL, "event1", date, -1) == EM_NULL_ARGUMENT);
    ASSERT(emAddEventByDate(em, "event1", date, -1) == EM_INVALID_DATE);

    dateDestroy(date);
    date = dateCreate(2, 2, 2000);
    ASSERT(emAddEventByDate(em, "event1", date, 1) == EM_SUCCESS);
    ASSERT(emAddEventByDate(em, "event1", date, -1) == EM_INVALID_EVENT_ID);
    ASSERT(emAddEventByDate(em, "event1", date, 2) == EM_EVENT_ALREADY_EXISTS);

    destroy:
    dateDestroy(date);
    destroyEventManager(em);
    return result;
}



/* ===== TESTING emAddEventByDiff ===== */


bool testEmAddEventByDiffDateTickUsage() {
    bool result = true;
    EventManager em = createEM(30, 12, 1999);

    Date new_date = dateCreate(2, 1, 2000);
    ASSERT(emAddEventByDiff(em, "New year!", 2, 5) == EM_SUCCESS); // This should add the event at 2.1.2020
    ASSERT(emAddEventByDate(em, "New year!", new_date, 6) == EM_EVENT_ALREADY_EXISTS);

    destroy:
    destroyEventManager(em);
    dateDestroy(new_date);
    return result;
}

bool testEMAddEventByDiffBasicUsage() {
    bool result = true;
    EventManager em = createEM(1, 1, 2000);
    ASSERT(emAddEventByDiff(em, "event1", 0, 1) == EM_SUCCESS); // 0 works

    ASSERT(emAddEventByDiff(em, "event1", 1, 2) == EM_SUCCESS); // same name different day works

    ASSERT(emAddEventByDiff(em, "event2", 0, 3) == EM_SUCCESS); // Same day different name and id works

    ASSERT(emTick(em, 1) == EM_SUCCESS);

    ASSERT(emAddEventByDiff(em, "event2", 0, 4) ==
           EM_SUCCESS); // After tick adding event to "current day" with same name as previous event works


    destroy:
    destroyEventManager(em);
    return result;
}

/*bool testEMAddEventByDiffCopiesEventName() {
    bool result = true;
    Date date = NULL;
    EventManager em = createEM(1, 1, 2000);

    // TODO: ADD


    destroy:
    dateDestroy(date);
    destroyEventManager(em);
    return result;
}*/

bool testEMAddEventByDiffBasicParameterErrors() {
    bool result = true;
    EventManager em = createEM(1, 1, 2000);
    ASSERT(emAddEventByDiff(em, NULL, 1, 1) == EM_NULL_ARGUMENT);
    ASSERT(emAddEventByDiff(NULL, "event1", 2, 2) == EM_NULL_ARGUMENT);

    ASSERT(emAddEventByDiff(em, "event2", -1, 3) == EM_INVALID_DATE);

    ASSERT(emAddEventByDiff(em, "event3", 1, -1) == EM_INVALID_EVENT_ID);

    ASSERT(emAddEventByDiff(em, "event4", 1, 4) == EM_SUCCESS);
    ASSERT(emAddEventByDiff(em, "event4", 1, 5) == EM_EVENT_ALREADY_EXISTS);

    ASSERT(emAddEventByDiff(em, "event5", 2, 4) == EM_EVENT_ID_ALREADY_EXISTS);

    destroy:
    destroyEventManager(em);
    return result;
}

/* ========== TESTING emRemoveEvent ========== */
bool testEMRemoveEventBasicErrors() {
    bool result = true;
    Date date = NULL;
    EventManager em = createEM(1, 1, 2000);
    ASSERT(emAddEventByDiff(em, "event1", 0, 1) == EM_SUCCESS);

    ASSERT(emRemoveEvent(NULL, 1) == EM_NULL_ARGUMENT); // NULL Argument

    ASSERT(emRemoveEvent(em, -1) == EM_INVALID_EVENT_ID);

    ASSERT(emRemoveEvent(em, 2) == EM_EVENT_NOT_EXISTS);

    ASSERT(emRemoveEvent(em, 1) == EM_SUCCESS);
    ASSERT(emRemoveEvent(em, 1) == EM_EVENT_NOT_EXISTS); // Event does not exist after removing it

    destroy:
    dateDestroy(date);
    destroyEventManager(em);
    return result;
}

bool testEMRemoveEventDoesntFindByName() {
    bool result = true;
    Date date = NULL;
    EventManager em = createEM(1, 1, 2000);
    emAddEventByDiff(em, "event1", 2, 2);
    emAddEventByDiff(em, "event1", 1, 1);
    emRemoveEvent(em, 1);
    ASSERT(emRemoveEvent(em, 1) == EM_EVENT_NOT_EXISTS);
    ASSERT(emGetEventsAmount(em) == 1);


    destroy:
    dateDestroy(date);
    destroyEventManager(em);
    return result;
}

bool testEMRemoveEventFindsEventFromAddByDiffAndAddByDate() {
    bool result = true;
    Date date = NULL;
    date = dateCreate(2, 1, 2000);
    EventManager em = createEM(1, 1, 2000);

    ASSERT(emAddEventByDiff(em, "event1", 1, 1) == EM_SUCCESS);
    ASSERT(emAddEventByDate(em, "event2", date, 2) == EM_SUCCESS);
    ASSERT(emRemoveEvent(em, 1) == EM_SUCCESS);
    ASSERT(emRemoveEvent(em, 2) == EM_SUCCESS);

    destroy:
    dateDestroy(date);
    destroyEventManager(em);
    return result;
}


/* ========== TESTING emChangeEventDate ========== */
bool testEMChangeEventDateBasicErrors() {
    bool result = true;
    Date date = NULL;
    EventManager em = createEM(1, 1, 2000);

    ASSERT(emChangeEventDate(NULL, 1, NULL) == EM_NULL_ARGUMENT);

    ASSERT(emAddEventByDiff(em, "event1", 5, 1) == EM_SUCCESS);
    date = dateCreate(1, 1, 1999);
    ASSERT(emChangeEventDate(em, 1, date) == EM_INVALID_DATE);
    dateDestroy(date);

    date = dateCreate(9, 9, 3000);
    ASSERT(emChangeEventDate(em, 2, date) == EM_EVENT_ID_NOT_EXISTS);

    ASSERT(emAddEventByDiff(em, "event1", 1, 2) == EM_SUCCESS);
    dateDestroy(date);
    date = dateCreate(2, 1, 2000);
    ASSERT(emAddEventByDiff(em, "event1", 4, 3) == EM_SUCCESS);
    ASSERT(emChangeEventDate(em, 3, date) == EM_EVENT_ALREADY_EXISTS);

    destroy:
    dateDestroy(date);
    destroyEventManager(em);
    return result;
}

bool testEMChangeEventDateCopiesDate() {
    bool result = true;
    Date date = NULL;
    EventManager em = createEM(1, 1, 2000);

    ASSERT(emAddEventByDiff(em, "event1", 1, 1) == EM_SUCCESS);
    date = dateCreate(5, 1, 2000);
    ASSERT(emChangeEventDate(em, 1, date) == EM_SUCCESS);
    ASSERT(emRemoveEvent(em, 1) == EM_SUCCESS);

    destroy:
    dateDestroy(
            date); // If there is an error here it probably means the date was not copied by emChangeEventDate and was copied inside the event by reference
    destroyEventManager(em);
    return result;
}


bool testEMChangeEventDateReinsertsTheEvent() {
    bool result = true;
    Date date = NULL;
    EventManager em = createEM(1, 1, 2000);

    ASSERT(emAddEventByDiff(em, "event1", 1, 1) == EM_SUCCESS);
    ASSERT(emAddEventByDiff(em, "event2", 2, 2) == EM_SUCCESS);
    ASSERT(emAddEventByDiff(em, "event3", 3, 3) == EM_SUCCESS);
    ASSERT(emAddEventByDiff(em, "event4", 4, 4) == EM_SUCCESS);

    date = dateCreate(2, 1, 2000);
    ASSERT(emChangeEventDate(em, 3, date) == EM_SUCCESS);
    ASSERT(strcmp(emGetNextEvent(em), "event1") == 0);

    ASSERT(emAddEventByDiff(em, "event5", 1, 5) == EM_SUCCESS);
    ASSERT(strcmp(emGetNextEvent(em), "event1") == 0);

    ASSERT(emChangeEventDate(em, 5, date) == EM_EVENT_ALREADY_EXISTS); // Change event to same day - might find bugs
    ASSERT(strcmp(emGetNextEvent(em), "event1") == 0);
    dateDestroy(date);

    date = dateCreate(9, 9, 2000);
    ASSERT(emChangeEventDate(em, 1, date) == EM_SUCCESS);
    ASSERT(strcmp(emGetNextEvent(em), "event3") == 0);

    destroy:
    dateDestroy(date);
    destroyEventManager(em);
    return result;
}


bool testEMChangeEventDateReordersTheEvent() {
    bool result = true;
    Date date = NULL;
    EventManager em = createEM(1, 1, 2000);

    ASSERT(emAddEventByDiff(em, "event1", 1, 1) == EM_SUCCESS);
    ASSERT(emAddEventByDiff(em, "event2", 2, 2) == EM_SUCCESS);
    ASSERT(emAddEventByDiff(em, "event3", 3, 3) == EM_SUCCESS);
    ASSERT(emAddEventByDiff(em, "event4", 4, 4) == EM_SUCCESS);

    date = dateCreate(1, 1, 2000);
    ASSERT(emChangeEventDate(em, 3, date) == EM_SUCCESS);
    ASSERT(strcmp(emGetNextEvent(em), "event3") == 0);
/*
    dateDestroy(date);

    date = dateCreate(1, 1, 2000);
    ASSERT(emRemoveEvent(em, 3) == EM_SUCCESS);
    ASSERT(emChangeEventDate(em, 4, date) == EM_SUCCESS);
    ASSERT(strcmp(emGetNextEvent(em), "event4") == 0);
    dateDestroy(date);

    date = dateCreate(5, 1, 2000);
    ASSERT(emChangeEventDate(em, 4, date) == EM_SUCCESS);
    ASSERT(strcmp(emGetNextEvent(em), "event1") == 0);
*/

    destroy:
    dateDestroy(date);
    destroyEventManager(em);
    return result;
}


/* ========== TESTING emAddMember ========== */
bool testEMAddMemberBasicUsage() {
    bool result = true;
    Date date = NULL;
    EventManager em = createEM(1, 1, 2000);

    ASSERT(emAddMember(em, "member1", 1) == EM_SUCCESS); // Basic add
    ASSERT(emAddMember(em, "member2", 2) == EM_SUCCESS); // Multiple adds
    ASSERT(emAddMember(em, "member3", 3) == EM_SUCCESS);

    ASSERT(emAddMember(em, "member1", 4) == EM_SUCCESS); // Same member name allowed

    ASSERT(emAddEventByDiff(em, "event1", 1, 1) == EM_SUCCESS);
    ASSERT(emAddMemberToEvent(em, 1, 1) == EM_SUCCESS);
    ASSERT(emRemoveEvent(em, 1) == EM_SUCCESS);
    ASSERT(emAddMember(em, "member1", 1) ==
           EM_MEMBER_ID_ALREADY_EXISTS); // Removing event with member doesnt delete member from system somehow

    destroy:
    dateDestroy(date);
    destroyEventManager(em);
    return result;
}

/*bool testEMAddMemberCopiesMemberName() {
    bool result = true;
    Date date = NULL;
    EventManager em = createEM(1, 1, 2000);


    //TODO: ADD

    destroy:
    dateDestroy(date);
    destroyEventManager(em);
    return result;
}*/


bool testEMAddMemberBasicErrors() {
    bool result = true;
    Date date = NULL;
    EventManager em = createEM(1, 1, 2000);

    ASSERT(emAddMember(NULL, "member1", 1) == EM_NULL_ARGUMENT);
    ASSERT(emAddMember(em, NULL, 1) == EM_NULL_ARGUMENT);

    ASSERT(emAddMember(em, "member1", -1) == EM_INVALID_MEMBER_ID);

    ASSERT(emAddMember(em, "member1", 1) == EM_SUCCESS);
    ASSERT(emAddMember(em, "member1", 1) == EM_MEMBER_ID_ALREADY_EXISTS);


    destroy:
    dateDestroy(date);
    destroyEventManager(em);
    return result;
}




/* ===== TESTING emAddMemberToEvent ===== */

bool testEMAddMemberToEventBasicParameterErrors() {
    bool result = true;
    EventManager em = createEM(1, 1, 2000);

    ASSERT(emAddMemberToEvent(NULL, 1, 1) == EM_NULL_ARGUMENT); // NULL em

    ASSERT(emAddMember(em, "member1", 1) == EM_SUCCESS);

    ASSERT(emAddMemberToEvent(em, 1, -1) == EM_INVALID_EVENT_ID);

    ASSERT(emAddMemberToEvent(em, -1, 1) == EM_INVALID_MEMBER_ID);

    ASSERT(emAddEventByDiff(em, "event1", 1, 1) == EM_SUCCESS);

    ASSERT(emAddMemberToEvent(em, 1, 1) == EM_SUCCESS);
    ASSERT(emAddMemberToEvent(em, 1, 1) == EM_EVENT_AND_MEMBER_ALREADY_LINKED);

    destroy:
    destroyEventManager(em);
    return result;
}

bool testEMAddMemberToEventBasicUsage() {
    bool result = true;
    Date date = NULL;
    EventManager em = createEM(1, 1, 2000);

    ASSERT(emAddMember(em, "member1", 1) == EM_SUCCESS);
    ASSERT(emAddMember(em, "member2", 2) == EM_SUCCESS);
    ASSERT(emAddEventByDiff(em, "event1", 1, 1) == EM_SUCCESS);

    ASSERT(emAddMemberToEvent(em, 1, 1) == EM_SUCCESS);
    ASSERT(emAddMemberToEvent(em, 2, 1) == EM_SUCCESS);

    date = dateCreate(3, 1, 2000);
    ASSERT(emChangeEventDate(em, 1, date) == EM_SUCCESS);

    ASSERT(emRemoveMemberFromEvent(em, 1, 1) == EM_SUCCESS);
    ASSERT(emAddMemberToEvent(em, 1, 1) == EM_SUCCESS);  // Can add member after removing him

    ASSERT(emRemoveEvent(em, 1) == EM_SUCCESS);
    ASSERT(emAddMemberToEvent(em, 1, 1) == EM_EVENT_ID_NOT_EXISTS); // Cant add member to removed event

    destroy:
    dateDestroy(date);
    destroyEventManager(em);
    return result;
}


/* ========== TESTING emRemoveMemberFromEvent ========== */
bool testEMRemoveMemberFromEventBasicErrors() {
    bool result = true;
    Date date = NULL;
    EventManager em = createEM(1, 1, 2000);
    ASSERT(emAddEventByDiff(em, "event1", 1, 1) == EM_SUCCESS);
    ASSERT(emAddMember(em, "member1", 1) == EM_SUCCESS);

    ASSERT(emRemoveMemberFromEvent(NULL, 1, 1) == EM_NULL_ARGUMENT);

    ASSERT(emRemoveMemberFromEvent(em, 1, -1) == EM_INVALID_EVENT_ID);

    ASSERT(emRemoveMemberFromEvent(em, -1, 1) == EM_INVALID_MEMBER_ID);

    ASSERT(emRemoveMemberFromEvent(em, 1, 2) == EM_EVENT_ID_NOT_EXISTS);

    ASSERT(emRemoveMemberFromEvent(em, 2, 1) == EM_MEMBER_ID_NOT_EXISTS);

    ASSERT(emRemoveMemberFromEvent(em, 1, 1) == EM_EVENT_AND_MEMBER_NOT_LINKED);

    destroy:
    dateDestroy(date);
    destroyEventManager(em);
    return result;
}

bool testEMRemoveMemberFromEventBasicUsage() {
    bool result = true;
    Date date = NULL;
    EventManager em = createEM(1, 1, 2000);

    ASSERT(emAddEventByDiff(em, "event1", 1, 1) == EM_SUCCESS);
    ASSERT(emAddMember(em, "member1", 1) == EM_SUCCESS);
    ASSERT(emAddMemberToEvent(em, 1, 1) == EM_SUCCESS);

    ASSERT(emRemoveMemberFromEvent(em, 1, 1) == EM_SUCCESS);
    ASSERT(emRemoveMemberFromEvent(em, 1, 1) == EM_EVENT_AND_MEMBER_NOT_LINKED); // After removing actually removed

    ASSERT(emAddMember(em, "member2", 2) == EM_SUCCESS);
    ASSERT(emAddMember(em, "member3", 3) == EM_SUCCESS);
    ASSERT(emAddMember(em, "member3", 4) == EM_SUCCESS);
    ASSERT(emAddMemberToEvent(em, 1, 1) == EM_SUCCESS);
    ASSERT(emAddMemberToEvent(em, 2, 1) == EM_SUCCESS);
    ASSERT(emAddMemberToEvent(em, 3, 1) == EM_SUCCESS);
    ASSERT(emAddMemberToEvent(em, 4, 1) == EM_SUCCESS);

    ASSERT(emRemoveMemberFromEvent(em, 3, 1) == EM_SUCCESS);
    ASSERT(emRemoveMemberFromEvent(em, 4, 1) ==
           EM_SUCCESS); // Didnt remove accidently in previous line because of same member name
    ASSERT(emRemoveMemberFromEvent(em, 2, 1) == EM_SUCCESS);
    ASSERT(emRemoveMemberFromEvent(em, 1, 1) == EM_SUCCESS); // Make sure other members are still present
    ASSERT(emRemoveMemberFromEvent(em, 1, 1) ==
           EM_EVENT_AND_MEMBER_NOT_LINKED); // Make sure all members are actually removed
    ASSERT(emRemoveMemberFromEvent(em, 2, 1) == EM_EVENT_AND_MEMBER_NOT_LINKED);
    ASSERT(emRemoveMemberFromEvent(em, 3, 1) == EM_EVENT_AND_MEMBER_NOT_LINKED);
    ASSERT(emRemoveMemberFromEvent(em, 4, 1) == EM_EVENT_AND_MEMBER_NOT_LINKED);

    destroy:
    dateDestroy(date);
    destroyEventManager(em);
    return result;
}



/* ========== TESTING emTick ========== */
bool testEMTickBasicErrors() {
    bool result = true;
    Date date = NULL;
    EventManager em = createEM(1, 1, 2000);

    ASSERT(emTick(NULL, 1) == EM_NULL_ARGUMENT);

    ASSERT(emTick(em, 0) == EM_INVALID_DATE);
    ASSERT(emTick(em, -1) == EM_INVALID_DATE);


    destroy:
    dateDestroy(date);
    destroyEventManager(em);
    return result;
}

bool testEMTickDeletesPastEvents() {
    bool result = true;
    Date date = NULL;
    EventManager em = createEM(1, 1, 2000);

    ASSERT(emAddEventByDiff(em, "event1", 1, 1) == EM_SUCCESS);
    ASSERT(emAddEventByDiff(em, "event1-2", 1, 2) == EM_SUCCESS);
    ASSERT(emAddEventByDiff(em, "event2", 2, 3) == EM_SUCCESS);
    ASSERT(emAddEventByDiff(em, "event3", 3, 4) == EM_SUCCESS);
    ASSERT(emAddMember(em, "member1", 1) == EM_SUCCESS);

    ASSERT(emTick(em, 1) == EM_SUCCESS);
    ASSERT(emGetEventsAmount(em) == 4);

    ASSERT(emTick(em, 1) == EM_SUCCESS);
    ASSERT(emGetEventsAmount(em) == 2); // 2 events were suppose to be deleted
    ASSERT(strcmp(emGetNextEvent(em), "event2") == 0);

    ASSERT(emTick(em, 1) == EM_SUCCESS);
    ASSERT(emGetEventsAmount(em) == 1); // 2 events were suppose to be deleted
    ASSERT(strcmp(emGetNextEvent(em), "event3") == 0);

    ASSERT(emTick(em, 1) == EM_SUCCESS);
    ASSERT(emGetEventsAmount(em) == 0); // 2 events were suppose to be deleted
    ASSERT(emGetNextEvent(em) == NULL); // https://piazza.com/class/kffnb4v77nd2ce?cid=251


    destroy:
    dateDestroy(date);
    destroyEventManager(em);
    return result;
}

// TODO: add test that emTick does -1 to the event counter of members in deleted events


/* ========== TESTING emGetEventsAmount ========== */
bool testEMGetEventsAmountBasicErrors() {
    bool result = true;
    Date date = NULL;
    EventManager em = createEM(1, 1, 2000);

    ASSERT(emGetEventsAmount(NULL) == -1);

    destroy:
    dateDestroy(date);
    destroyEventManager(em);
    return result;
}

bool testEMGetEventsAmountBasicUsage() {
    bool result = true;
    Date date = NULL;
    EventManager em = createEM(1, 1, 2000);
    ASSERT(emGetEventsAmount(em) == 0);

    ASSERT(emAddEventByDiff(em, "event1", 1, 1) == EM_SUCCESS);
    ASSERT(emAddEventByDiff(em, "event1", 2, 2) == EM_SUCCESS);
    ASSERT(emAddEventByDiff(em, "event2", 1, 3) == EM_SUCCESS);
    ASSERT(emGetEventsAmount(em) == 3);

    ASSERT(emRemoveEvent(em, 2) == EM_SUCCESS);
    ASSERT(emGetEventsAmount(em) == 2);

    ASSERT(emAddEventByDiff(em, "event1", 5, 2) == EM_SUCCESS); // Same id as deleted event
    ASSERT(emGetEventsAmount(em) == 3);

    date = dateCreate(2, 4, 20000);
    ASSERT(emAddEventByDate(em, "event3", date, 4) == EM_SUCCESS);
    ASSERT(emGetEventsAmount(em) == 4);
    ASSERT(emAddEventByDate(em, "event4", date, 5) == EM_SUCCESS);
    ASSERT(emGetEventsAmount(em) == 5);

    destroy:
    dateDestroy(date);
    destroyEventManager(em);
    return result;
}



/* ========== TESTING emGetNextEvent ========== */
bool testEMGetNextEventBasicErrors() {
    bool result = true;
    Date date = NULL;
    EventManager em = createEM(1, 1, 2000);

    ASSERT(emGetNextEvent(NULL) == NULL);

    destroy:
    dateDestroy(date);
    destroyEventManager(em);
    return result;
}

bool testEMGetNextEventGetsEventInCorrectOrder() {
    bool result = true;
    Date date = NULL;
    EventManager em = createEM(1, 1, 2000);

    ASSERT(emAddEventByDiff(em, "event3", 4, 3) == EM_SUCCESS);
    ASSERT(emAddEventByDiff(em, "event1", 1, 1) == EM_SUCCESS);
    ASSERT(emAddEventByDiff(em, "event4", 4, 4) == EM_SUCCESS);
    ASSERT(emAddEventByDiff(em, "event2", 1, 2) == EM_SUCCESS);

    ASSERT(strcmp(emGetNextEvent(em), "event1") == 0);

    ASSERT(emRemoveEvent(em, 1) == EM_SUCCESS);
    ASSERT(strcmp(emGetNextEvent(em), "event2") == 0);

    date = dateCreate(1, 1, 2000);
    ASSERT(emAddEventByDate(em, "eventMID", date, 5) == EM_SUCCESS);
    ASSERT(strcmp(emGetNextEvent(em), "eventMID") == 0);
    ASSERT(emRemoveEvent(em, 5) == EM_SUCCESS);

    ASSERT(emRemoveEvent(em, 2) == EM_SUCCESS);
    ASSERT(strcmp(emGetNextEvent(em), "event3") == 0);

    ASSERT(emRemoveEvent(em, 3) == EM_SUCCESS);
    ASSERT(strcmp(emGetNextEvent(em), "event4") == 0);

    destroy:
    dateDestroy(date);
    destroyEventManager(em);
    return result;
}




/* ========== TESTING emPrintAllResponsibleMembers ========== */
bool testEMPrintAllResponsibleMembersSimpleTests() {
    bool result = true;
    Date date = NULL;
    EventManager em = createEM(1, 1, 2000);

    emPrintAllResponsibleMembers(em, "simple_tests0.out.txt");
    ASSERT(isFilePrintOutputCorrect("simple_tests0.out.txt", ""));

    ASSERT(emAddMember(em, "member1", 1) == EM_SUCCESS);
    ASSERT(emAddEventByDiff(em, "event1", 1, 1) == EM_SUCCESS);
    ASSERT(emAddMemberToEvent(em, 1, 1) == EM_SUCCESS);
    emPrintAllResponsibleMembers(em, "simple_tests1.out.txt");
    ASSERT(isFilePrintOutputCorrect("simple_tests1.out.txt", "member1,1\n"));


    ASSERT(emAddMember(em, "member2", 2) == EM_SUCCESS);
    ASSERT(emAddEventByDiff(em, "event2", 2, 2) == EM_SUCCESS);
    ASSERT(emAddMemberToEvent(em, 2, 2) == EM_SUCCESS);
    ASSERT(emAddMemberToEvent(em, 1, 2) == EM_SUCCESS);
    emPrintAllResponsibleMembers(em, "simple_tests2.out.txt");
    ASSERT(isFilePrintOutputCorrect("simple_tests2.out.txt", "member1,2\nmember2,1\n"));

    ASSERT(emAddMemberToEvent(em, 2, 1) == EM_SUCCESS);
    emPrintAllResponsibleMembers(em, "simple_tests3.out.txt");
    ASSERT(isFilePrintOutputCorrect("simple_tests3.out.txt", "member1,2\nmember2,2\n"));


    emTick(em, 5);
    emPrintAllResponsibleMembers(em, "simple_tests4.out.txt");
    ASSERT(isFilePrintOutputCorrect("simple_tests4.out.txt", ""));

    destroy:
    dateDestroy(date);
    destroyEventManager(em);
    return result;
}


/* ========== TESTING testEMPrintAllEvents ========== */
bool testEMPrintAllEventsBasicTests() {
    bool result = true;
    Date date = NULL;
    EventManager em = createEM(1, 1, 2000);

    emAddEventByDiff(em, "event1", 1, 1);
    emAddEventByDiff(em, "event2", 2, 2);
    emAddEventByDiff(em, "event3", 2, 3);
    emAddEventByDiff(em, "event4", 1, 4);
    emAddMember(em, "member1", 1);
    emAddMember(em, "member2", 2);
    emAddMember(em, "member3", 3);

    emAddMemberToEvent(em, 1, 1);
    emAddMemberToEvent(em, 2, 1);
    emAddMemberToEvent(em, 3, 1);
    emAddMemberToEvent(em, 1, 2);
    emAddMemberToEvent(em, 3, 2);
    emAddMemberToEvent(em, 1, 3);

    emPrintAllEvents(em, "basic_tests1.out.txt");
    ASSERT(isFilePrintOutputCorrect("basic_tests1.out.txt",
                                    "event1,2.1.2000,member1,member2,member3\nevent4,2.1.2000\nevent2,3.1.2000,member1,member3\nevent3,3.1.2000,member1\n"));

    destroy:
    dateDestroy(date);
    destroyEventManager(em);
    return result;
}


/* ========== Tests from more people ========== */

bool testBigEventManagerCreatorYanTomsinsky() {
    bool result = true;
    Date start_date = dateCreate(1, 12, 2020);
    EventManager em = createEventManager(start_date);
    char *event_name1 = "event1";
    char *event_name2 = "event2";
    char *event_name3 = "event3";
    char *event_name4 = "event4";
    char *event_name5 = "event5";
    Date event_date1 = dateCreate(1, 12, 2020);
    Date event_date2 = dateCreate(5, 12, 2020);
    Date event_date3 = dateCreate(10, 12, 2020);
    Date event_date4 = dateCreate(11, 06, 2021);
    Date event_date5 = dateCreate(1, 11, 2020);
    ASSERT_TEST(emAddEventByDate(em, event_name1, event_date1, 1) == EM_SUCCESS, destroyDates);
    ASSERT_TEST(emAddEventByDate(em, event_name2, event_date2, 2) == EM_SUCCESS, destroyDates);
    ASSERT_TEST(emAddEventByDate(em, event_name3, event_date3, 3) == EM_SUCCESS, destroyDates);
    ASSERT_TEST(emAddEventByDate(em, event_name4, event_date4, 4) == EM_SUCCESS, destroyDates);
    ASSERT_TEST(emAddEventByDate(em, event_name5, event_date5, 5) == EM_INVALID_DATE, destroyDates);
    ASSERT_TEST(emGetEventsAmount(em) == 4, destroyDates);
    Date new_date_to_event_4 = dateCreate(4, 12, 2020);
    ASSERT_TEST(emChangeEventDate(em, 8, new_date_to_event_4) == EM_EVENT_ID_NOT_EXISTS, destroyDates2);

    ASSERT_TEST(emChangeEventDate(em, 4, new_date_to_event_4) == EM_SUCCESS, destroyDates2);

    char *event_name_same_as_1 = "event1";
    ASSERT_TEST(emAddEventByDate(em, event_name_same_as_1, event_date3, 6) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emChangeEventDate(em, 6, event_date1) == EM_EVENT_ALREADY_EXISTS, destroyDates2);

    ASSERT_TEST(emGetEventsAmount(em) == 5, destroyDates2);
    ASSERT_TEST(emRemoveEvent(em, 6) == EM_SUCCESS, destroyDates2);

    ASSERT_TEST(emGetEventsAmount(em) == 4, destroyDates2);

    ASSERT_TEST(strcmp(emGetNextEvent(em), event_name1) == 0, destroyDates2);

    //adding 5 members
    ASSERT_TEST(emAddMember(em, "yan1", 1) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emAddMember(em, "yan2", 2) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emAddMember(em, "yan3", 3) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emAddMember(em, "yan4", 4) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emAddMember(em, "yan5", 5) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emAddMember(em, "yan6", 5) == EM_MEMBER_ID_ALREADY_EXISTS, destroyDates2);

    /*  member 1- events 1,2,3,4
        member 2 - events 2,3
        member 3 - events 2
        member 4 - events 2
        member 5 - events 2,4*/
    ASSERT_TEST(emAddMemberToEvent(em, 1, 1) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emAddMemberToEvent(em, 1, 2) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emAddMemberToEvent(em, 1, 3) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emAddMemberToEvent(em, 1, 4) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emAddMemberToEvent(em, 2, 2) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emAddMemberToEvent(em, 2, 3) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emAddMemberToEvent(em, 3, 2) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emAddMemberToEvent(em, 4, 2) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emAddMemberToEvent(em, 5, 1) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emAddMemberToEvent(em, 5, 2) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emAddMemberToEvent(em, 5, 4) == EM_SUCCESS, destroyDates2);

    ASSERT_TEST(emAddMemberToEvent(em, 2, 10) == EM_EVENT_ID_NOT_EXISTS, destroyDates2);
    ASSERT_TEST(emAddMemberToEvent(em, 8, 1) == EM_MEMBER_ID_NOT_EXISTS, destroyDates2);
    ASSERT_TEST(emAddMemberToEvent(em, 1, 1) == EM_EVENT_AND_MEMBER_ALREADY_LINKED, destroyDates2);
    ASSERT_TEST(emRemoveMemberFromEvent(em, 1, 1) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emAddMemberToEvent(em, 1, 1) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emRemoveMemberFromEvent(em, 2, 10) == EM_EVENT_ID_NOT_EXISTS, destroyDates2);
    ASSERT_TEST(emRemoveMemberFromEvent(em, 8, 1) == EM_MEMBER_ID_NOT_EXISTS, destroyDates2);
    ASSERT_TEST(emRemoveMemberFromEvent(em, 2, 1) == EM_EVENT_AND_MEMBER_NOT_LINKED, destroyDates2);

    emPrintAllEvents(em, "yan_file1.out.txt");
    isFilePrintOutputCorrect("yan_file1.out.txt",
                             "event1,1.12.2020,yan1,yan5\nevent4,4.12.2020,yan1,yan5\nevent2,5.12.2020,yan1,yan2,yan3,yan4,yan5\n event3,10.12.2020,yan1,yan2\n");

    emPrintAllResponsibleMembers(em, "yan_file2.out.txt");
    isFilePrintOutputCorrect("yan_file2.out.txt", "yan1,4\nyan5,3\nyan2,2\nyan3,1\nyan4,1\n");

    ASSERT_TEST(emTick(em, 4) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emGetEventsAmount(em) == 2, destroyDates2);

    emPrintAllEvents(em, "yan_file3.out.txt");
    isFilePrintOutputCorrect("yan_file3.out.txt",
                             "event2,5.12.2020,yan1,yan2,yan3,yan4,yan5\nevent3,10.12.2020,yan1,yan2\n");

    emPrintAllResponsibleMembers(em, "yan_file4.out.txt");
    isFilePrintOutputCorrect("yan_file4.out.txt", "yan1,2\nyan2,2\nyan3,1\nyan4,1\nyan5,1\n");

    ASSERT_TEST(emTick(em, 2) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emGetEventsAmount(em) == 1, destroyDates2);
    ASSERT_TEST(emTick(em, 20) == EM_SUCCESS, destroyDates2);
    ASSERT_TEST(emGetEventsAmount(em) == 0, destroyDates2);
    ASSERT_TEST(emAddEventByDate(em, event_name3, event_date3, 3) == EM_INVALID_DATE, destroyDates2);

    //Good Luck!

    destroyDates2:
    dateDestroy(new_date_to_event_4);
    destroyDates:
    dateDestroy(event_date1);
    dateDestroy(event_date2);
    dateDestroy(event_date3);
    dateDestroy(event_date4);
    dateDestroy(event_date5);
    dateDestroy(start_date);
    destroyEventManager(em);
    return result;
}


#define TEST_NAMES \
    X(testEMCreateStandardTest) \
    X(testEMDestroyNullArgument) \
    X(testEMAddEventByDateCopiesDate) \
    X(testEMAddEventByDateCopiesEventName) \
    X(testEMAddEventByDateBasicUsageTests) \
    X(testEMAddEventByDateBasicParameterErrors) \
    X(testEMAddEventByDateBasicErrorsAreInCorrectOrder) \
    X(testEmAddEventByDiffDateTickUsage) \
    X(testEMAddEventByDiffBasicUsage) \
    X(testEMAddEventByDiffBasicParameterErrors) \
    X(testEMRemoveEventBasicErrors)\
    X(testEMRemoveEventDoesntFindByName) \
    X(testEMRemoveEventFindsEventFromAddByDiffAndAddByDate) \
    X(testEMChangeEventDateBasicErrors) \
    X(testEMChangeEventDateReordersTheEvent) \
    X(testEMChangeEventDateCopiesDate) \
    X(testEMChangeEventDateReinsertsTheEvent) \
    X(testEMAddMemberBasicUsage) \
    X(testEMAddMemberBasicErrors) \
    X(testEMAddMemberToEventBasicParameterErrors) \
    X(testEMAddMemberToEventBasicUsage) \
    X(testEMRemoveMemberFromEventBasicErrors) \
    X(testEMRemoveMemberFromEventBasicUsage) \
    X(testEMTickBasicErrors) \
    X(testEMTickDeletesPastEvents) \
    X(testEMGetEventsAmountBasicErrors) \
    X(testEMGetEventsAmountBasicUsage) \
    X(testEMGetNextEventBasicErrors) \
    X(testEMGetNextEventGetsEventInCorrectOrder) \
    X(testEMPrintAllResponsibleMembersSimpleTests) \
    X(testEMPrintAllEventsBasicTests) \
    X(testBigEventManagerCreatorYanTomsinsky)

bool (*tests[])(void) = {
#define X(test_name) test_name,
        TEST_NAMES
#undef X
};

const char *testNames[] = {
#define X(name) #name,
        TEST_NAMES
#undef X
};

#define NUMBER_TESTS 32

int main(int argc, char **argv) {
    if (argc == 1) {
        for (int test_idx = 0; test_idx < NUMBER_TESTS; test_idx++) {
            RUN_TEST(tests[test_idx], testNames[test_idx]);
        }
        return 0;
    }
    if (argc != 2) {
        fprintf(stdout, "Usage: priority_queue_tests <test index>\n");
        return 0;
    }

    int test_idx = strtol(argv[1], NULL, 10);
    if (test_idx < 1 || test_idx > NUMBER_TESTS) {
        fprintf(stderr, "Invalid test index %d\n", test_idx);
        return 0;
    }

    RUN_TEST(tests[test_idx - 1], testNames[test_idx - 1]);
    return 0;

}

