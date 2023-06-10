#include "clientsmenu.h"
#include "client.h" //! @todo remove that dots but LSP is retarded and complains, despite cmake working...
#include "dbhandle.h"
#include "list.h"
#include "menuutil.h"
#include <assert.h>
#include <form.h>
#include <ncurses.h>
#include <panel.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOTRACE

/**
 *@file
 *@brief Clients menu implementation.
 */

/**
 *@brief Parse form.
 *@param result Client object where to save parsed result, Client::m_ID will be
 *set to INVALIDCLIENTID. Does not allocate object.
 *@param form Filled already form containing client data.
 *@result
 * - True if any of fields has been altered
 * - False if none of fields has been altered.
 *
 */
static bool clientFormParse(struct Client **result, FORM *form) {
  assert(result && "Can not be null pointer.");
  bool isFormAltered = false;
  struct Client *resultPtr = *result;

  // for each changed field in form
  for (int i = 0; i < field_count(form); ++i) {
    FIELD *curField = form_fields(form)[i];
    assert(curField);
    char *curFieldBuffer = field_buffer(curField, 0);
    assert(curFieldBuffer);
    // if field was edited
    if (field_status(curField) == true) {
      isFormAltered = true;
      // corresponding field indices in  client structure.
      switch (i) {
      case 0:
        resultPtr->m_cardID = atoi(curFieldBuffer);
        break;
      case 1:
        resultPtr->m_name = calloc(sizeof(char), strlen(curFieldBuffer) + 1);
        strcpy(resultPtr->m_name, curFieldBuffer);
        break;
      case 2:
        resultPtr->m_surname = calloc(sizeof(char), strlen(curFieldBuffer) + 1);
        strcpy(resultPtr->m_surname, curFieldBuffer);
        break;
      case 3:
        resultPtr->m_adress = calloc(sizeof(char), strlen(curFieldBuffer) + 1);
        strcpy(resultPtr->m_adress, curFieldBuffer);
        break;
      case 4:
        resultPtr->m_phoneNum = atoi(curFieldBuffer);
        break;
      }
    }
  }
  return isFormAltered;
}

/**
 * @brief Function for editing client.
 * @param result Where to put results at.
 * @param placeHolder Structure of placeholder values for form. If NULL
 * placeholders are empty (usefull when instead of editing adding).
 * @return Whenever changes should be propagated.
 * - false = nothing to do.
 *
 */
static bool clientFormEdit(struct Client **result,
                           const struct Client *const placeHolder) {
  assert(result);
  const char *const formFieldNames[] = {"Card id", "Name", "Surname", "Address",
                                        "Phone Number"};
  int fieldCount = sizeof(formFieldNames) / sizeof(*formFieldNames);

  FORM *form = formInit(fieldCount);
  set_field_type(form_fields(form)[0], TYPE_INTEGER, 0, 0, 0);
  set_field_type(form_fields(form)[4], TYPE_INTEGER, 0, 0, 0);
  //! @todo set fields initial values as in edit given Client structure.
  if (placeHolder) {
    if (placeHolder->m_cardID != INVALIDCLIENTCARDID) {
      char *tempstr = calloc(FORMFIELDLENGTH, sizeof(char));
      sprintf(tempstr, "%d", placeHolder->m_cardID);
      set_field_buffer(form_fields(form)[0], 0, tempstr);
      free(tempstr);
    }
    if (placeHolder->m_name) {
      set_field_buffer(form_fields(form)[1], 0, placeHolder->m_name);
    }
    if (placeHolder->m_surname) {
      set_field_buffer(form_fields(form)[2], 0, placeHolder->m_surname);
    }
    if (placeHolder->m_adress) {
      set_field_buffer(form_fields(form)[3], 0, placeHolder->m_adress);
    }
    if (placeHolder->m_phoneNum != INVALIDCLIENTPHONENUM) {
      char *tempstr = calloc(FORMFIELDLENGTH, sizeof(char));
      sprintf(tempstr, "%d", placeHolder->m_phoneNum);
      set_field_buffer(form_fields(form)[4], 0, tempstr);
      free(tempstr);
    }
  }
  formInvoke(form, formFieldNames, "Client");

  bool altered = false;
  altered = clientFormParse(result, form);

  formFree(form);
  return altered;
}

/**
 *@brief function for adding client;
 **/
void addClient(void) {
  struct Client *newClient = clientNew();
  if (clientFormEdit(&newClient, 0) && clientIsComplete(newClient)) {
    if (!dbHandleClientInsert(newClient)) {
      const char *mess[] = {"Database error", NULL};
      menuUtilMessagebox("Adding client failed", (mess));
    }
  } else {
    const char *mess[] = {"Not all fields were set.", NULL};
    menuUtilMessagebox("Adding client failed", (mess));
  }
  clientFree(newClient);
}

/**
 *@brief Given client generates string repesenting client string in listView
 *friendly format. (whole row)
 *@param client Client based on which generate string.
 *@return string repersenting client
 * */
char *clientGetListViewString(const struct Client *client) {
  struct Client *clientPtr = (struct Client *)client;
  // 5 is number of fields in resulting string
  const int fieldCount = 5;
  // +1 for null termintaor
  char *result = calloc(fieldCount * FORMFIELDLENGTH + 1, sizeof(char));
  sprintf(result, "%*d%*s%*s%*s%*d", FORMFIELDLENGTH, clientPtr->m_cardID,
          FORMFIELDLENGTH, clientPtr->m_name, FORMFIELDLENGTH,
          clientPtr->m_surname, FORMFIELDLENGTH, clientPtr->m_adress,
          FORMFIELDLENGTH, clientPtr->m_phoneNum);
  return result;
}

/**
 * @brief extract client given ListNode.
 * @param out where to save extracted Client.
 * @param node from where Client is extracted.
 **/
static void extractClient(struct Client **out, const struct ListNode *node) {
  assert(out != NULL && "extractClient can not output to NULL");
  struct Client *res = node->m_data;
  clientClone(out, node->m_data);
}

/**
 *@brief Invokes ListView of clients
 *@return
 *-Chosen client clone
 *-NULL if canceled.
 *
 *@note Extracted client has to be freed manually. See also @link
 *clientChooseNoReturn @endlink.
 **/
static struct Client *clientChoose(void) {
  const char *colNames[] = {"cardId", "name", "surname", "adress",
                            "phone number"};
  const int colCount = sizeof(colNames) / sizeof(*colNames);

  struct Client *out = NULL;
  bool didChoose = listViewInvoke(
      (void **)&out, (void *)(const struct ListNode *)extractClient,
      clientGetList, colNames, colCount,
      (char *(*)(void *))clientGetListViewString, (void *)(void *)clientFree);

#ifndef NOTRACE
  if (out) {
    char *outVal = calloc(100, sizeof(char));
    char *msg[] = {clientGetListViewString(out), NULL};
    sprintf(outVal, "clientChoose -- out val = %p", out);
    menuUtilMessagebox(outVal, (const char **)msg);
    free(msg[0]);
    free(outVal);
  }
#endif

  doupdate();
  return out;
}

/**
 *@brief Remove client
 **/
void clientRemove(void) {
  struct Client *toRemove = clientChoose();
  if (toRemove) {
#ifndef NOTRACE
    char *str = calloc(200, sizeof(char));
    char *info = clientGetListViewString(toRemove);
    const char *msg[] = {"Removing clinet with data:", info, NULL};
    menuUtilMessagebox("clientRemove", (const char **)msg);
#endif
    dbHandlClientRemove(toRemove->m_ID);
    clientFree(toRemove);
  }
}

/**
 *@brief Wrapper around @link clientChoose @endlink frees extracted client
 *instanntly.
 **/
void clientChooseNoReturn(void) {
  struct Client *r = clientChoose();
  if (r)
    clientFree(r);
}

/**
 *@brief Edit client.
 **/
void clientEdit(void) {
  struct Client *toEdit = clientChoose();
  struct Client *edited = NULL;
  clientClone(&edited, toEdit);

  if (clientFormEdit(&edited, toEdit)) {
    dbHandleClientUpdate(edited);
  }

  clientFree(toEdit);
  clientFree(edited);
}

/**
 *@brief Handles displaying of clients menu.
 */
void clientsMenu(void) {

  const char *const title = "Clients";
  const char *const choices[] = {"List clients", "Add client", "Remove client",
                                 "Edit clients", "Return to main menu"};
  const int choicesCount = sizeof(choices) / sizeof(choices[0]);
  //! @todo implement submenus.
  void (*menuFun[])(void) = {(void (*)(void))clientChooseNoReturn, addClient,
                             clientRemove, clientEdit, NULL};
  menuInvoke(title, choices, choicesCount, menuFun);
}
