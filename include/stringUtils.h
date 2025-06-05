#pragma once

#include "calc.h"
#include <stdbool.h>

#include <glib-2.0/glib.h>

bool strlenEqual(const char* str1, const char* str2);

GString* strTruncSpaces(const char* str);

Symbol symFromString(const char *str, char **end);

Number numberFromString(const char *str, char **end);

Arg argFromString(const char *str, char** end);

const char* symbolToStr(Symbol sym);

void argPrint(Arg arg);

void argListPrint(GList* list);
void argListPrintln(GList* list);




