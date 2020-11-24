#include "../member.h"
#include "../list.h"


int main(void)
{
    
    Member one = memberCreate("Almog Yazek", 23231);
    List member_list = listCreate((ListElement)one, (CopyListElement)memberCopy, (FreeListElement)memberDestroy, (EqualListElements)memberIdCompare);
    printf("%d\n", listGetSize(member_list));
    Member two = memberCreate("Ben Yazek", 38982);
    printf(">>CODE %d\n", listInsert(member_list, (ListElement)two));
    printf("%d\n", listGetSize(member_list));

    //printf("CODE %d\n",listRemoveElement(member_list, one));
    printf("%d\n", listGetSize(member_list));

    listDestroy(member_list);
    if(one==NULL) printf("wor");
    return 0;
}