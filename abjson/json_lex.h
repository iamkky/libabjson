#ifndef _JSON_LEX_
#define _JSON_LEX_

#include <abd/AData.c.h>
#include <abjson/json_parser.h>

int json_lex(char *buffer, int *cursor, JsonParserType *value,void *extra);

#endif
