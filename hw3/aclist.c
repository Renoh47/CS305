/* Filename: aclist.c
 * Created by: Tanya L. Crenshaw
 * Modified by: Kieran Losh
 * Date created: 9/30/2014
 * Date modified: 10/21/2014
 *
 * Description:  The functions implemented are:
 * 
 *               1. initializeACL(): create the access control list
 *                  node that contains the file name and the pointer
 *                  to the linked list of entries.
 *
 *               2. addEntry(): insert a new entry into an access
 *               control list.
 *
 *               3. printList(): print the contents of an access
 *               control list.
 *                
 *               4. addRight(): Adds a right to an entry
 *
 *               5. deleteRight(): Removes a right from an entry
 *
 *               6. deleteEntry(): Deletes an entry from the acl
 *               
 *               7. freeACL(): Deletes and frees the acl
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "aclist.h"

/* Function:    initializeACL()
 *
 * Parameters:  1. filename: The name of the file for this access
 *              control list.  
 *
 *              2. aclPtr: The address of a NULL pointer to an access
 *              control list.  Description: Allocate memory for an
 *              access control list.  Initialize the access control
 *              list with the filename.
 */
int initializeACL(char * filename, AccessControlList ** aclPtr)
{

  /* Check if aclPtr is NULL.  If so, then this function cannot
   * proceed. 
   */
  if (aclPtr == NULL)
    {
      return INITIALIZE_FAILURE;
    }
  
  /* Check if the access control list pointer is NULL.  If it's not
   * NULL, this access control list may have already been initialized.
   */
  if (*aclPtr != NULL)
    {
      return INITIALIZE_FAILURE;
    }

  /* Allocate enough memory to hold an access control list.  
   */
  (*aclPtr) = malloc(sizeof(AccessControlList));

  /* Copy the filename into the new access control list.
   */
  strncpy((*aclPtr) -> filename, filename, MAX_LENGTH);  
  
  /* Set the pointer to the list of access control entries to NULL
   * as there are no entries at this time.
   */
  (*aclPtr) -> aces = NULL;

  return INITIALIZE_SUCCESS;
}
 
/* Function:    addEntry()
 * Parameters:  1. newUser, the new user to be inserted into the access control list.
 *              2. rights, the rights applied to the new user.
 *              3. acl, a pointer to the list.
 * Description: This function inserts a user and set of rights into an 
 *              access control list.  
 */
int addEntry(char * newUser, int rights, AccessControlList * acl)
{

  /* Create a temporary pointer which initially points to the same location
   * as the start of the list of entries.
   */
  AccessControlEntry * currentNode;

  /* Create a pointer which will point to the new element to be
   * added to the list of entries.
   */
  AccessControlEntry * newNode;


  /* Check if the access control list pointer is empty.  If so,
   * then this list is not well-formed.  One cannot add a new user
   * to an empty access control list.
   */
  if (acl == NULL)
    {
      return ADD_ENTRY_FAILURE;
    }

  /* Allocate enough memory to hold an access control entry.
   */
  newNode = malloc(sizeof(AccessControlEntry));

  /* Initialize the new node with the username.
   */
  strncpy(newNode -> user, newUser, MAX_LENGTH);
	
  /* No null character is added by strncpy, so add one deliberately
   */
  newNode -> user[MAX_LENGTH -1] = '\0';
  
  /* Initialize the new node with the user's rights. 
   */
  newNode -> rights = rights;


  /* Set the next pointer to NULL 
   */
  newNode -> next = NULL;

  /* Check if the list has 0 users.  If so, simply point at the
   * new user and exit the function.
   */
  if (acl -> aces == NULL)
    {
#ifdef DEBUG_MODE_2
  printf("Filename: %s, Line no: %d \n", __FILE__, __LINE__);
#endif 
      acl -> aces = newNode;
      return ADD_ENTRY_SUCCESS;
    }

  /* Otherwise, there is at least one entry in the linked list of
   * access control entries.
   */
  currentNode = acl -> aces;


  /* Insert the new entry at the front of the list.
   */
  newNode -> next = acl -> aces;
  acl-> aces = newNode;

#ifdef DEBUG_MODE_2
  printf("Filename: %s, Line no: %d \n", __FILE__, __LINE__);
#endif 

  return 0;
}

/* Function:    printACL()
 * Parameters:  head, a pointer to a list node
 * Description: This function prints the contents of a list of
 *              AccessControlEntries (ACE) in an AccessControlList:
 *
 *              printList: ( filename:  ACE1, ACE2, ..., ACEN)
 *
 *              If the AccessControlList is empty, it prints:
 * 
 *              printList: ( empty access control list)
 */
void printACL(AccessControlList * acl)
{

  /* Create a temporary pointer will point to the current node
   * being printed in the access control list.
   */
  AccessControlEntry * currentNode;

  /* Print an initial "(" 
   */
  printf("printList: (");

  /* Check if the access control list is empty.
   */
  if (acl == NULL)
    {
      printf(" empty access control list )\n");
      return;
    }

  /* Print the filename.
   */
  printf("File: %s. ", acl -> filename);

  /* Check if there are any users who have rights to this file 
   */
  if (acl-> aces != NULL)
    {
      /* Point to the beginning of the list and print the 
       * first element in the list.
       */
      currentNode = acl -> aces;

      /* Traverse the list to the last element of the list.  As long
       * as the next pointer of the node is not NULL, then there is
       * still another element in the list.
       */     
      while(currentNode != NULL)
    	{
    	  printf(", %s (", currentNode->user);
          
        //array of chars that corresponds to rights. Specifically, for a number of a right, the corresponding character is at position log2(right)
        char* rightChars = "xwro";
        //for loop index
        int i;
        //loop through the possible values of permissions, and AND the current permission against the user rights to determine if the user has them
        for (i = NUMBER_OF_RIGHTS - 1; i >= 0; i--) //loop iterates backwards to print in the standard order (orwx)
        {
          //AND user rights with current right (2^i) to determine if the user has that right
          if (currentNode->rights & (int)(pow(2, i)))
            //print the letter of the right
            printf("%c", rightChars[i]);
        } 
        printf(")"); //match the parenthesis around the permissions

    	  // Point to the next entry
    	  currentNode = currentNode->next;
    	}
    }

  else
    {
      printf("  No entries.");
    }
      
  /* Print the final parenthesis 
   */
  printf(") \n");

  return;
}

/* Function:    deleteRight()
 * Parameters:  right, the bit-masked right to remove from the user's permissions
                username, the username of the user whose right we wish to remove
                acl, a pointer to the AccessControlList
 * Description: This function removes the specified right from the specified user's bit-masked rights.
 */
int deleteRight(int right, char * username, AccessControlList * acl)
{
  //check to make sure the list is populated
  if (acl == NULL || acl->aces == NULL)
    return LIST_EMPTY_ERROR;

  //check to make sure that the right is a valid right
  if ((right != R_OWN) && (right != R_READ) && (right != R_WRITE) && (right != R_EXECUTE))
    return DELETE_RIGHT_FAILURE;


  //Pointer to the current element. 
  AccessControlEntry* currentNode;
  //boolean-like variable to store whether the user has been found in the list 
  int userFound = 0; 
  //standard for loop to iterate through the list
  for (currentNode = acl->aces; currentNode != NULL; currentNode = currentNode->next)
  {
    if (strcmp(currentNode->user, username) == 0)
    {
      userFound = 1;
      currentNode->rights &= ~(right); // ANDing with the inverse of the right removes the right. 
    }
  }
  //if the user was not found in the list, return with error code, else return normally
  if (!userFound)
    return USER_NOT_FOUND;
  else 
  return DELETE_RIGHT_SUCCESS;

}

/* Function:    addRight()
 * Parameters:  right, the bit-masked right to add to the user's permissions
 *              username, the username of the user whose rights we wish to add to
 *              acl, a pointer to the AccessControlList
 * Description: This function adds a specified right to the specified user's bit-masked rights.         
 */ 
int addRight(int right, char * username, AccessControlList * acl)
{
  //check to make sure the list is populated
  if (acl == NULL || acl->aces == NULL)
    return LIST_EMPTY_ERROR;

  //check to make sure that the right is a valid right
  if ((right != R_OWN) && (right != R_READ) && (right != R_WRITE) && (right != R_EXECUTE))
    return ADD_RIGHT_FAILURE;


  //Pointer to the current element. 
  AccessControlEntry* currentNode;
  //boolean-like variable to store whether the user has been found in the list 
  int userFound = 0; 
  //standard for loop to iterate through the list
  for (currentNode = acl->aces; currentNode != NULL; currentNode = currentNode->next)
  {
    if (strcmp(currentNode->user, username) == 0)
    {
      userFound = 1;
      currentNode->rights |= right; //ORing with the right adds the right. 
    }
  }
  //if the user was not found in the list, return with error code, else return normally
  if (!userFound)
    return USER_NOT_FOUND;
  return ADD_RIGHT_SUCCESS;
}

/* Function:  deleteEntry()
 * Parameters:  char * username: name of the user who we wish to delete from the access control list
 *              AccesControlList* acl: Pointer to the acl that we wish to delete the user from
 * Description: Deletes the entry with the specified user's name. If the specified user cannot be found,
 *              it returns with USER_NOT_FOUND code. Similarly, if the list head or first element of the 
 *              is empty, it returns with LIST_EMPTY_ERROR code.
 */ 
int deleteEntry(char * username, AccessControlList * acl)
{
  //check to see if the list is empty or the head pointer is null
  if (acl == NULL || acl->aces == NULL)
    return LIST_EMPTY_ERROR;

  //double pointer to the element to allow easy deletion
  AccessControlEntry** ptr = &(acl->aces);

  //boolean-like variable to store whether the user has been found in the list
  int userFound = 0;
  //loop to move through the list
  while (*ptr != NULL)
  {
    if (strcmp((*ptr)->user, username) == 0)//check the username of the current element
    {
      AccessControlEntry* temp = *ptr; //temp pointer to the element to be deleted
      *ptr = (*ptr)->next; //set the pointer of the current element to the next element's pointer
      //free malloc'd memory and in effect delete the element
      free(temp); 
      userFound = 1;
    }
    ptr = &((*ptr)->next); //point to the next element's pointer
  }
  if (!userFound)
      return USER_NOT_FOUND;
  return DELETE_ENTRY_SUCCESS;
}

/* Function:    freeACL()
 * Parameters:  acl: Pointer to the Access Control List that is to be freed.
 * Description: Deletes all of the list entries and the list head, and frees
 *              the memory previously used by them.
 */ 
int freeACL(AccessControlList * acl)
{
  //check to make sure that the list is actually populated. If it's already NULL our job is done.
  if (acl == NULL)
    return FREE_ACL_SUCCESS;

  //double pointer to the element to allow easy deletion
  AccessControlEntry** ptr = &(acl->aces);
  //loop to iterate through the list and delete and free elements
  while (*ptr != NULL)
  {
    AccessControlEntry* temp = *ptr; //temp pointer to the element to be deleted
    *ptr = (*ptr)->next; //set the pointer of the current element to the next element's pointer
    //free malloc'd memory and in effect delete the element
    free(temp);
  }
  free(acl); //free the head of the acl
  return FREE_ACL_SUCCESS;
}
