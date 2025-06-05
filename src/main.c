#include "calc.h"
#include "stringUtils.h"
#include <float.h>
#include <glib-2.0/glib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define INPUT "5.33//22*345+5"




int main(void)
{
  GList* list = serializeString(INPUT);

  argListPrintln(list); 

  g_list_free_full(list, free);
}
