#include "calc.h"
#include "stringUtils.h"

#include <assert.h>
#include <glib-2.0/glib.h>
#include <stdlib.h>

static GList* g_list_prepend_arg_full(GList* list, Arg arg)
{
  Arg* ptr = malloc(sizeof arg);
  assert(ptr);
  *ptr = arg;
  return g_list_append(list, ptr);
}

GList* serializeString(const char* str)
{
  GString* noSpaces = strTruncSpaces(str); 
    
  char* curr = noSpaces->str;
  Arg a = { .type = arg_type_number, .val.num = 0 };
  GList* list = NULL;

  while(a.type != arg_type_error)
  {
    char* next = NULL; 

    a = argFromString(curr, &next);
    if(a.type != arg_type_error)
    {
      list = g_list_prepend_arg_full(list, a);
    }

    curr = next;
  }

  g_string_free(noSpaces, true);

  return list;
}

