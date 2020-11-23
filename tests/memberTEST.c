#include "../member.h"

int main(void)
{
    //memberCreate TEST
    Member member1 = memberCreate("Avi Cohen", 231321);
    if(!member1)
    {
        printf("by1\n");
        return 1;
    }

    //memberCopy TEST
    Member member2 = memberCopy(member1);
    if(!member2)
    {
        printf("by2\n");
        memberDestroy(member1);
        return 1;
    }

    printMember(member1);
    printMember(member2);

    //memberIdCompare TEST
    if(memberIdCompare(member1,member2)) printf(">> 1=2\n");


    //memberGet TEST
    int x;
    int *px = &x;
    memberIdGet(member2,px);
    printf("%d%d%d\n",x,x,x);


    //memberDestroy TEST (check vergrid)
    memberDestroy(member1);
    memberDestroy(member2);
    return 0;
}