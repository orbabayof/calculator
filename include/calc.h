#pragma once

#include <glib-2.0/glib.h>
#include <stdbool.h>
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

GList* deserializeString(const char* str);

typedef enum 
{
  status_finished,
  status_fail,
  status_success,

}reduceStatus;

reduceStatus argListReduce(GList* list);
int prioretyOfSymbol(Symbol sym);


bool isBinaryExpr(GList* start);
bool isUnaryExpr(GList* start);

int prioretyOfExpr(GList* start);


