#include "connectors.h"

#include <stddef.h>
#include <assert.h>

#define IS_BINARY_EXPR(num) \
  ((num).next && (num).next->next)

#define IS_BINARY_EXPR_PTR(num) \
  (num && (num)->next && (num)->next->next)

typedef enum 
{
  ADD_OP,
  MULT_OP,

  MAX_OP, 
}op_t;

op_t get_operation(char op)
{
  switch(op)
  {
    case '+':
      return ADD_OP;
    case '*':
      return MULT_OP;
    default:
      return MAX_OP;
  }
}

double add(double fir, double sec)
{
  return fir + sec;
}

double mult(double fir, double sec)
{
  return fir * sec;
}

static double(*op_table[MAX_OP])(double fir, double sec) =
{
  [ADD_OP] = add,
  [MULT_OP] = mult,

};


double calc(struct number num)
{
  //expr of the type "a op b"
  if(IS_BINARY_EXPR(num))
  {
    op_t operation = get_operation(num.next->op);

    //here we check if we have another binary expr, like this "a op b op c ..." and "b op c" is more important than "a op b"
    struct number* next_expr = num.next->next;
    if(IS_BINARY_EXPR_PTR(next_expr) && next_expr->next->precedence)
    {
      assert(operation != MAX_OP && "double calc(struct number): undefined operation");
      assert(next_expr && "WTF");

      return op_table[operation](num.val, calc(*num.next->next));
    }

    return op_table[operation](num.val, next_expr->val);
  }
  else
  {
    return num.val;
  }
}

