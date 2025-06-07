#include "calc.h"
#include "stringUtils.h"
#include <float.h>
#include <glib-2.0/glib.h>
#include <stdbool.h>
#include <stdio.h>

#define INPUT "+6+5+2+1+2+3+4"


int main(void)
{
  GList* list = deserializeString(INPUT);

  argListPrintln(list); 

  
  reduceStatus status = status_fail;
  while((status = argListReduce(list)) == status_success);

  if(status == status_finished)
  {
    argListPrintln(list); 
  }
  else 
  {
    puts("error");
  }

  g_list_free_full(list, free);
}
