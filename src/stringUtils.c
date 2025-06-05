#include "stringUtils.h"
#include "calc.h"
#include "glibconfig.h"

#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glib-2.0/glib.h>


bool strlenEqual(const char* str1, const char* str2)
{
  size_t diff = 0;
  while(*str1 != '\0' && *str2 != '\0')
  {
    diff += *(str1++) - *(str2++);
  }

  return diff == 0;
}

Symbol symFromString(const char *str, char** end)
{
  Symbol sym_out = symbol_error;

  if(strlenEqual("+", str))
    sym_out = op_add;
  if(strlenEqual("-", str))
    sym_out = op_sub;
  if(strlenEqual("/", str))
    sym_out = op_dev;
  if(strlenEqual("*", str))
    sym_out = op_mult;

  if(strlenEqual("(", str))
    sym_out = open_p;
  if(strlenEqual(")", str))
    sym_out = close_p;

  if(end)
  {
    *end = (char*)(str + 1);
  }

  return sym_out;
}

Number numberFromString(const char *str, char** end)
{
  return strtod(str, end);
}

GString* strTruncSpaces(const char *str_with_spaces)
{
  GString *str = g_string_sized_new(strlen(str_with_spaces));
  for(const char* curr = str_with_spaces; *curr != '\0'; ++curr)
  {
    if(*curr != ' ')
    {
      g_string_append_c(str, *curr);
    }
  }

  return str;
}

Arg argFromString(const char *strNoSpaces, char **end)
{
  Arg out = { 0 };

  if(strlen(strNoSpaces) == 0)
    out = (Arg){ .type = arg_type_error };

  //if we should look for number
  else if(isdigit(*strNoSpaces))
  {
    out = (Arg){ .val.num = numberFromString(strNoSpaces, end), .type = arg_type_number };
  }
  else 
  {
    out = (Arg){ .val.sym = symFromString(strNoSpaces, end), .type = arg_type_symbol };
  }

  return out;
}

const char* symbolToStr(Symbol sym)
{
  switch (sym) 
  {
    case op_add : return "+"; 
    case op_sub : return "-"; 
    case op_dev : return "/"; 
    case op_mult: return "*"; 

    case open_p      : return "(";
    case close_p     : return ")";
    case symbol_error: return "symbol error";

    default: return "unknown";
  }
}

static GString* numberToStr(Number num)
{
  GString* str = g_string_new("");

  g_string_printf(str, "%lf", num);
  
  
  gsize newLen = str->len;
  for(gsize i = str->len - 1; i != 0; --i)
  {
    if(str->str[i] != '0')
    {
      break;
    }
    else 
    {
      --newLen;
    }
  }
  if(str->str[newLen - 1] == '.')
    --newLen;

  g_string_truncate(str, newLen);

  return str;
}

void argPrint(Arg arg)
{
  switch (arg.type) 
  {
    case arg_type_number:
    {
      GString* numStr = numberToStr(arg.val.num);
      printf("%s", numStr->str);
      g_string_free(numStr, true);
      break;
    }
    case arg_type_symbol:
    {
      printf("%s", symbolToStr(arg.val.sym));
      break;
    }
    case arg_type_error:
    {
      printf("Arg error");
      break;
    }
  }
}

static void argElemPrint(gpointer data, gpointer user_data)
{
  (void)user_data;
  Arg* arg = data; 
  if(arg)
    argPrint(*arg);
  else 
    printf("(null)");
}

void argListPrint(GList* list)
{
  printf("{ ");
  g_list_foreach(list, argElemPrint, NULL);
  printf(" }");
}

void argListPrintln(GList* list)
{
  argListPrint(list);
  puts("");
}

