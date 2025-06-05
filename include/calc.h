#pragma once

#include "glib.h"
#define Number double 

typedef enum 
{
  op_add,
  op_sub,
  op_dev,
  op_mult,

  open_p,
  close_p,

  symbol_error,

}Symbol;


typedef union
{
  Number num;   
  Symbol sym;
}ArgValue;

typedef enum 
{
  arg_type_number,
  arg_type_symbol,
  arg_type_error,
}ArgType;

typedef struct 
{
  ArgValue val; 
  ArgType type;
}Arg;

GList* serializeString(const char* str);



