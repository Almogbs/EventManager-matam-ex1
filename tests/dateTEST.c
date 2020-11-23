#include "../date.h"

int main(void)
{
    //dateCreate TEST
    Date date1 = dateCreate(30,7,2003);
    if(!date1)
    {
        printf("by1\n");
        return 1;
    }

    //dateCopy TEST
    Date date2 = dateCopy(date1);
    if(!date2)
    {
        printf("by2\n");
        dateDestroy(date1);
        return 1;
    }

    printDate(date1);
    printDate(date2);

    //dateCompare TEST
    printf(">> %d  is 0?\n", dateCompare(date1,date2));

    //dateTick TEST
    dateTick(date2);
    printDate(date2);

    printf(">> %d   is -?\n", dateCompare(date1,date2));

    //dateGet TEST
    int x,y,z;
    int *px = &x,*py = &y,*pz = &z;
    dateGet(date2,px,py,pz);
    printf("%d%d%d\n",x,y,z);

    //isValidDate TEST
    Date date3 = dateCreate(0,2,2);
    if(date3==NULL) printf("yep\n");

    Date date4 = dateCreate(30,12,12);
    printDate(date4);
    dateTick(date4);
    printDate(date4);

    printf(">> %d  is +?\n", dateCompare(date2,date4));

    //dateDestroy TEST (check vergrid)
    dateDestroy(date1);
    dateDestroy(date2);
    dateDestroy(date4);
    return 0;
}