#include "clientsmenu.h"
#include "../client.h" //! @todo remove that dots but LSP is retarded and complains, despite cmake working...
#include "menuutil.h"
#include "mmenu.h"
#include <assert.h>
#include <form.h>
#include <ncurses.h>
#include <panel.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
bool clientFormParse(struct Client **result, FORM *form) {
  assert(result && "Can not be null pointer.");
  bool isFormAltered = false;
  struct Client *resultPtr = *result;
  resultPtr->m_ID = INVALIDCLIENTID;

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
 * @todo make placeHolder do shit.
 */
bool ClientFormEdit(struct Client **result,
                    const struct Client *const placeHolder) {
  assert(result);
  const char *const formFieldNames[] = {"Card id", "Name", "Surname", "Address",
                                        "Phone Number"};
  int fieldCount = sizeof(formFieldNames) / sizeof(*formFieldNames);

  FORM *form = formInit(fieldCount);
  set_field_type(form_fields(form)[0], TYPE_INTEGER, 0, 0, 0);
  //! @todo set fields initial values as in edit given Client structure.
  formInvoke(form, formFieldNames, "Client");

  bool state = false;
  state = clientFormParse(result, form);

  formFree(form);
  return state;
}

/**
 *@brief function for adding client;
 **/
void addClient(void) {
  struct Client *newClient = clientNew();
  if (ClientFormEdit(&newClient, 0) == TRUE) {
    //! @todo propgate changes...
  }

  clientFree(newClient);
}

/**
 *@brief Handles displaying of clients menu.
 */
void clientsMenu(void) {

  const char *const title = "Clients";
  const char *const choices[] = {"list client", "Add client", "remove client",
                                 "edit client", "Return to main menu"};
  const int choicesCount = sizeof(choices) / sizeof(choices[0]);
  //! @todo implement submenus.
  void (*menuFun[])(void) = {NULL, addClient, NULL, NULL, NULL};
  menuInvoke(title, choices, choicesCount, menuFun);
}
