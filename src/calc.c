#include "calc.h"
#include "glibconfig.h"
#include "stringUtils.h"

#include <assert.h>
#include <glib-2.0/glib.h>
#include <stdlib.h>

static void ptrSwap(void** ptr1, void** ptr2)
{
  char** first = (char**)ptr1;
  char** second = (char**)ptr2;

  char* temp = *first;
  *first = *second;
  *second = temp;
}

static GList* g_list_prepend_arg_full(GList* list, Arg arg)
{
  Arg* ptr = malloc(sizeof arg);
  assert(ptr);
  *ptr = arg;
  return g_list_append(list, ptr);
}

GList* deserializeString(const char* str)
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

int prioretyOfSymbol(Symbol sym)
{
  switch (sym) 
  {
    case op_add:
    case op_sub:
      return 1;

    case op_dev:
    case op_mult:
      return 2;

    case open_p:
    case close_p:
      return 10;

    default:
      return -1;
  }
}

static bool listHaveAtListNNodes(GList* list, gsize n)
{
  while(n-- != 0)
  {
    if(!list) 
      return false;

    list = list->next;
  }

  return true;
}

bool isBinaryExpr(GList* start)
{
  if(!listHaveAtListNNodes(start, 3))
    return false;

  Arg* first = start->data;
  Arg* second = start->next->data;
  Arg* third = start->next->next->data;

  return first->type == arg_type_number 
         && second->type == arg_type_symbol 
         && third->type == arg_type_number;
}

bool isUnaryExpr(GList* start)
{
  if(!listHaveAtListNNodes(start, 2))
    return false;

  Arg* first = start->data;
  Arg* second = start->next->data;

  return first->type == arg_type_symbol  
         && second->type == arg_type_number;
}

int prioretyOfExpr(GList *start)
{
  Arg* symbol;
  if(isBinaryExpr(start))
  {
    symbol = start->next->data; 
  }
  else if(isUnaryExpr(start))
  {
    symbol = start->data;
  }
  else 
  {
    return -1;
  }

  return prioretyOfSymbol(symbol->val.sym);
}

static Number binaryExprReduce(Arg* firstArg, Arg* sym, Arg* secondArg)
{
  Number first = firstArg->val.num;
  Number second = secondArg->val.num;

  switch (sym->val.sym) 
  {
    case op_add : return first + second; 
    case op_sub : return first - second; 
    case op_dev : return first / second;
    case op_mult: return first * second;

    default:
      assert(false && "binary expr not implemented");
  }
}

static Number unaryExprReduce(Arg* sym, Arg* arg)
{
  Number num = arg->val.num;
  switch (sym->val.sym) 
  {
    case op_sub: return -num;
    case op_add: return num;
      
    default:
      assert(false && "unary expr not implemented");
  }
}

static GList* ListremoveNElem(GList *list, gsize n)
{
  while (n-- != 0) 
  {
    list = g_list_remove_link(list, list->next);    
  } 

  return list;
}

reduceStatus argListReduce(GList *list)
{
  if(!list->next)
    return status_finished;

  if(isBinaryExpr(list))
  {
    if(prioretyOfExpr(list) >= prioretyOfExpr(list->next->next)) 
    {
      Arg* arg1 = list->data;

      arg1->val.num = binaryExprReduce(list->data, list->next->data, list->next->next->data);
      list = ListremoveNElem(list, 2);
      return status_success;
    }
    else  
    {
      argListReduce(list->next->next);
    }
  }
  if(isUnaryExpr(list))
  {
    Arg* arg2 = list->next->data;
    arg2->val.num = unaryExprReduce(list->data, arg2);

    ptrSwap(&list->data, &list->next->data);
    ListremoveNElem(list, 1);

    return status_success;
  }

  return status_fail;
}
