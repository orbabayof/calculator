#pragma once 

struct number
{
  double val;
  struct connector* next;
};

struct connector
{
  char op;
  int precedence;
  struct number* next; 
};

double calc(struct number num);
