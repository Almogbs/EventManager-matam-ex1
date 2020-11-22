#include "date.h"
#include <assert.h>

struct Date_t {
    int day;
    int month;
    int year;
};


Date dateCreate(int day, int month, int year)
{
    Date date = malloc(sizeof(*date));
    if(!date)
    {
        return NULL;
    }
    date->day = day;
    date->month = month;
    date->year = year;
    if(!isValidDate(date))
    {
        free(date);
        return NULL;
    }
    return date;
}


void dateDestroy(Date date)
{
    if(!date)
    {
        return;
    }
    free(date);
}


Date dateCopy(Date date)
{
    if(!date)
    {
        return NULL;
    }
    Date new_date = dateCreate(date->day, date->month, date->year);
    if(!new_date)
    {
        return NULL;
    }
    return new_date;
}


bool dateGet(Date date, int* day, int* month, int* year)
{
    if(!date || !day || !month || !year )
    {
        return false;
    }
    *day = date->day;
    *month = date->month;
    *year = date->year;
    return true;
}


int dateCompare(Date date1, Date date2)
{
    if(!date1 || !date2)
    {
        return 0;
    }

    if(date1->year != date2->year)
    {
        return (date1->year - date2->year);
    }
    assert(date1->year - date2->year == 0);
    if(date1->month != date2->month)
    {
        return (date1->month - date2->month);
    }
    assert(date1->month - date2->month == 0);
    return (date1->day - date2->day);
}


void dateTick(Date date)
{
    if(!date)
    {
        return;
    }
    if(date->day == MAX_DAY)
    {
        if(date->month == MAX_MONTH)
        {
            date->day = MIN_DAY;
            date->month = MIN_MONTH;
            date->year = date->year + 1;
            return;
        }
        date->month = date->month + 1;
        date->day = MIN_DAY;
        return;
    }
    date->day = date->day + 1;
}


static bool isValidDate(Date date)
{
    if(date->day < MIN_DAY || date->day > MAX_DAY || date->month < MIN_MONTH || date->month > MAX_MONTH)
    {
        return false;
    }
    return true;
}

//DELETE ME!!!!!
void printDate(Date date)
{
    printf("%d.%d.%d\n", date->day, date->month, date->year);
}