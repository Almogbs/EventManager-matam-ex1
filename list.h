#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/**@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@EDIT
* Generic List Container
*
* Implements a list container type.
* The list has an internal iterator for external use. For all functions
* where the state of the iterator after calling that function is not stated,
* it is undefined. That means that you cannot assume anything about it.
*
* The following functions are available:
*   pqCreate		    - Creates a new empty list
*   pqDestroy		    - Deletes an existing list and frees all resources
*   pqCopy		        - Copies an existing list
*   pqGetSize		    - Returns the size of a given list
*   pqContains	        - returns whether or not an element exists inside the list.
*   pqInsert	        - Insert an element with a given priority to the list.
*   				        Duplication in the list is allowed.
*   				        Iterator value is undefined after this operation.
*   pqChangePriority  	- Changes priority of an element with specific priority
*					        Iterator value is undefined after this operation.
*   pqRemove		    - Removes the highest priority element in the list
*                           Iterator value is undefined after this operation.
*   pqGetFirst	        - Sets the internal iterator to the first element in the list and returns it
*   pqGetNext		    - Advances the internal iterator to the next key and returns it.
*	pqClear		        - Clears the contents of the list. Frees all the elements of
*	 				        the list using the free function.
* 	PQ_FOREACH	        - A macro for iterating over the list's elements.
*/

/** Type for defining the list */
typedef struct List_t *List;

/** Type for defining the list */
typedef struct Node_t *Node;

/** Type used for returning error codes from list functions */
typedef enum ListResult_t {
    LIST_SUCCESS,
    LIST_OUT_OF_MEMORY,
    LIST_NULL_ARGUMENT,
    LIST_ELEMENT_DOES_NOT_EXISTS,
    LIST_ITEM_DOES_NOT_EXIST,
    LIST_ERROR
} ListResult;

/** Data element data type for list container */
typedef void *ListElement;

/** Type of function for copying a data element of the List */
typedef ListElement(*CopyListElement)(ListElement);

/** Type of function for deallocating a data element of the list */
typedef void(*FreeListElement)(ListElement);

Node nodeCreate(ListElement element);

/**
* Type of function used by the list to identify equal elements.
* This function should return:
* 		true if they're equal;
*		false otherwise;
*/
typedef bool(*EqualListElements)(ListElement, ListElement);


/**
* listCreate: Allocates a new empty list.
*
* @param copy_element - Function pointer to be used for copying data elements into
*  	the list or when copying the list.
* @param free_element - Function pointer to be used for removing data elements from
* 		the list
* @param compare_element - Function pointer to be used for comparing elements
* 		inside the list. Used to check if new elements already exist in the list.
* @return
* 	NULL - if one of the parameters is NULL or allocations failed.
* 	A new Map in case of success.
*/
List listCreate(ListElement element,
                CopyListElement copy_element, 
                FreeListElement free_element, 
                EqualListElements equal_elements);
                /**
* listDestroy: Deallocates an existing list. Clears all elements by using the
* free functions.
*
* @param list - Target liste to be deallocated. If list is NULL nothing will be
* 		done
*/
void listDestroy(List list);


/**
* listGetSize: Returns the number of elements in a list
* @param list - The list which size is requested
* @return
* 	-1 if a NULL pointer was sent.
* 	Otherwise the number of elements in the list.
*/
int listGetSize(List list);

/**
* listContains: Checks if an element exists in the list. The element will be
* considered in the list if one of the elements in the list it determined equal
* using the comparison function used to initialize the list.
*
* @param list - The list to search in
* @param element - The element to look for. Will be compared using the
* 		comparison function.
* @return
* 	false - if one or more of the inputs is null, or if the key element was not found.
* 	true - if the key element was found in the list.
*/
bool listContains(List list, ListElement element);

/**
*   listInsert: add a specified element.
*
* @param list - The list for which to add the data element
* @param element - The element which need to be added.
*      A copy of the element will be inserted as supplied by the copying function
*      which is given at initialization.
* @return
* 	LIST_NULL_ARGUMENT if a NULL was sent as one of the parameters
* 	LIST_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
* 	an element failed)
* 	LIST_SUCCESS the paired elements had been inserted successfully
*/
ListResult listInsert(List list, ListElement element);



/**
*   listRemoveElement: Removes the element from the list which have its value equal to element.
*   the elements are removed and deallocated using the free functions supplied at initialization.
*
* @param list - The list to remove the elements from.
* @param element
* 	The element to find and remove from the list. The element will be freed using the
* 	free function given at initialization. 
* @return
* 	LIST_NULL_ARGUMENT if a NULL was sent to the function.
* 	LIST_ELEMENT_DOES_NOT_EXISTS if given element does not exists.
* 	LIST_SUCCESS the most prioritized element had been removed successfully.
*/
ListResult listRemoveElement(List list, ListElement element);

#endif /* LIST_H_ */
