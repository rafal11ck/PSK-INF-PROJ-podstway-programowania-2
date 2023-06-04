#include "carsmenu.h"
#include "car.h"
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
 * @file
 * @brief Cars menu implementation.
 */

/**
 * @brief Parse form.
 * @param result Car object where to save parsed result, Car::m_ID will be
 * set to INVALIDCARID. Does not allocate object.
 * @param form Filled already form containing car data.
 * @result
 * - True if any of fields has been altered
 * - False if none of fields has been altered.
 */
static bool carFormParse(struct Car **result, FORM *form) {
  assert(result && "Can not be null pointer.");
  bool isFormAltered = false;
  struct Car *resultPtr = *result;

  // for each changed field in form
  for (int i = 0; i < field_count(form); ++i) {
    FIELD *curField = form_fields(form)[i];
    assert(curField);
    char *curFieldBuffer = field_buffer(curField, 0);
    assert(curFieldBuffer);
    // if field was edited
    if (field_status(curField) == true) {
      isFormAltered = true;
      // corresponding field indices in car structure.
      switch (i) {
      case 0:
        resultPtr->m_regNum = calloc(sizeof(char), strlen(curFieldBuffer) + 1);
        strcpy(resultPtr->m_regNum, curFieldBuffer);
        break;
      case 1:
        resultPtr->m_brand = calloc(sizeof(char), strlen(curFieldBuffer) + 1);
        strcpy(resultPtr->m_brand, curFieldBuffer);
        break;
      case 2:
        resultPtr->m_model = calloc(sizeof(char), strlen(curFieldBuffer) + 1);
        strcpy(resultPtr->m_model, curFieldBuffer);
        break;
      case 3:
        resultPtr->m_yOfProd = atoi(curFieldBuffer);
        break;
      case 4:
        resultPtr->m_color = calloc(sizeof(char), strlen(curFieldBuffer) + 1);
        strcpy(resultPtr->m_color, curFieldBuffer);
        break;
      case 5:
        resultPtr->m_mileage = atoi(curFieldBuffer);
        break;
      }
    }
  }
  return isFormAltered;
}

/**
 * @brief Function for editing car.
 * @param result Where to put results at.
 * @param placeHolder Structure of placeholder values for form. If NULL
 * placeholders are empty (useful when instead of editing adding).
 * @return Whenever changes should be propagated.
 * - false = nothing to do.
 *
 */
static bool carFormEdit(struct Car **result, const struct Car *const placeHolder) {
  assert(result);
  const char *const formFieldNames[] = {"Registration Number", "Brand",
                                        "Model", "Year of Production", "Color", "Mileage (KM)"};
  int fieldCount = sizeof(formFieldNames) / sizeof(*formFieldNames);

  FORM *form = formInit(fieldCount);
  set_field_type(form_fields(form)[3], TYPE_INTEGER, 0, 0, 0);
  set_field_type(form_fields(form)[5], TYPE_INTEGER, 0, 0, 0);
  //! @todo set fields initial values as in edit given Car structure.
  if (placeHolder) {
    if (placeHolder->m_regNum) {
      set_field_buffer(form_fields(form)[0], 0, placeHolder->m_regNum);
    }
    if (placeHolder->m_brand) {
      set_field_buffer(form_fields(form)[1], 0, placeHolder->m_brand);
    }
    if (placeHolder->m_model) {
      set_field_buffer(form_fields(form)[2], 0, placeHolder->m_model);
    }
    if (placeHolder->m_yOfProd != INVALIDCARYOFPROD) {
      char *tempstr = calloc(FORMFIELDLENGTH, sizeof(char));
      sprintf(tempstr, "%d", placeHolder->m_yOfProd);
      set_field_buffer(form_fields(form)[3], 0, tempstr);
      free(tempstr);
    }
    if (placeHolder->m_color) {
      set_field_buffer(form_fields(form)[4], 0, placeHolder->m_color);
    }
    if (placeHolder->m_mileage != INVALIDCARMILEAGE) {
      char *tempstr = calloc(FORMFIELDLENGTH, sizeof(char));
      sprintf(tempstr, "%ld", placeHolder->m_mileage);
      set_field_buffer(form_fields(form)[5], 0, tempstr);
      free(tempstr);
    }
  }
  formInvoke(form, formFieldNames, "Car");

  bool altered = false;
  altered = carFormParse(result, form);

  formFree(form);
  return altered;
}

/**
 * @brief Function for adding a car.
 */
void addCar(void) {
  struct Car *newCar = carNew();
  if (carFormEdit(&newCar, 0) && carIsComplete(newCar)) {
    if (!dbHandleCarInsert(newCar)) {
      const char *mess[] = {"Database error", NULL};
      menuUtilMessagebox("Adding car failed", (mess));
    }
  } else {
    const char *mess[] = {"Not all fields were set.", NULL};
    menuUtilMessagebox("Adding car failed", (mess));
  }
  carFree(newCar);
}

/**
 * @brief Given car, generates a string representing car data in listView
 * friendly format (whole row).
 * @param car Car based on which to generate the string.
 * @return String representing the car.
 */
char *carGetListViewString(const struct Car *car) {
  struct Car *carPtr = (struct Car *)car;
  // 6 is the number of fields in the resulting string
  const int fieldCount = 6;
  // +1 for null terminator
  char *result = calloc(fieldCount * FORMFIELDLENGTH + 1, sizeof(char));
  sprintf(result, "%*s%*s%*s%*d%*s%*ld", FORMFIELDLENGTH,
          carPtr->m_regNum, FORMFIELDLENGTH, carPtr->m_brand,
          FORMFIELDLENGTH, carPtr->m_model,FORMFIELDLENGTH, carPtr->m_yOfProd,
          FORMFIELDLENGTH, carPtr->m_color, FORMFIELDLENGTH, carPtr->m_mileage);
  return result;
}

/**
 * @brief Extracts car from the given ListNode.
 * @param out Where to save the extracted Car.
 * @param node From where the Car is extracted.
 */
static void extractCar(struct Car **out, const struct ListNode *node) {
  assert(out != NULL && "extractCar cannot output to NULL");
  struct Car *res = node->m_data;
  carClone(out, node->m_data);
}

/**
 * @brief Invokes the listView of cars.
 * @return
 * - Chosen car clone
 * - NULL if user cancelled.
 *
 *@note Extracted client has to be freed manually. See also @link
 *clientChooseNoReturn @endlink.
 **/
static struct Car *carChoose(void) {
  const char *colNames[] = {"Registration Number", "Brand", "Model",
                            "Year of Production", "Color", "Mileage (KM)"};
  const int colCount = sizeof(colNames) / sizeof(*colNames);

  struct Car *out = NULL;
  bool didChoose = listViewInvoke(
      (void **)&out, (void *)(const struct ListNode *)extractCar,
      carGetList, colNames, colCount,
      (char *(*)(void *))carGetListViewString, (void *)(void *)carFree);

#ifndef NOTRACE
  if (out) {
    char *outVal = calloc(100, sizeof(char));
    char *msg[] = {carGetListViewString(out), NULL};
    sprintf(outVal, "carChoose -- out val = %p", out);
    menuUtilMessagebox(outVal, (const char **)msg);
    free(msg[0]);
    free(outVal);
  }
#endif

  doupdate();
  return out;
}

/**
 * @brief Removes a car.
 */
void carRemove(void) {
  struct Car *toRemove = carChoose();
  if (toRemove) {
#ifndef NOTRACE
    char *str = calloc(200, sizeof(char));
    char *info = carGetListViewString(toRemove);
    const char *msg[] = {"Removing car with data:", info, NULL};
    menuUtilMessagebox("carRemove", (const char **)msg);
#endif
    dbHandleCarRemove(toRemove->m_ID);
    carFree(toRemove);
  }
}

/**
 *@brief Wrapper around @link carChoose @endlink frees extracted car
 *instanntly.
 **/
void carChooseNoReturn(void) {
  struct Car *r = carChoose();
  if (r)
    carFree(r);
}

/**
 *@brief Edit car.
 **/
void carEdit(void) {
  struct Car *toEdit = carChoose();
  struct Car *edited = NULL;
  carClone(&edited, toEdit);

  if (carFormEdit(&edited, toEdit)) {
    dbHandleCarUpdate(edited);
  }

  carFree(toEdit);
  carFree(edited);
}

/**
 *@brief Handles displaying of cars menu.
 */
void carsMenu(void) {

  const char *const title = "Cars";
  const char *const choices[] = {"List cars", "Add cars", "Remove cars",
                                 "Edit cars", "Return to main menu"};
  const int choicesCount = sizeof(choices) / sizeof(choices[0]);
  //! @todo implement submenus.
  void (*menuFun[])(void) = {(void (*)(void))carChooseNoReturn, addCar,
                             carRemove, carEdit, NULL};
  menuInvoke(title, choices, choicesCount, menuFun);
}
