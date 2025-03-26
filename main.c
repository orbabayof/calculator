#include "connectors.h"

#include <stddef.h>
#include <stdio.h>

int main()
{
  struct number num3 = {.val = 3, .next = NULL};
  struct connector con2 = {.next = &num3, .op = '*', .precedence = 5};

  struct number num2 = {.val = 5, .next = &con2};
  struct connector con = {.next = &num2, .op = '+', .precedence = 2};
  struct number num1 = {.val = 5, .next = &con};


  printf("5 + 5 * 3(null op) = %lf\n", calc(num1));

}
