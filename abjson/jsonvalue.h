#ifndef _ABJSON_VALUE_H_
#define _ABJSON_VALUE_H_

#include <inttypes.h>
#include <stdint.h>

enum JsonValueTypes {JSON_NULL = 0, JSON_NUMBER, JSON_BOOLEAN, JSON_STRING, JSON_OBJECT, JSON_ARRAY, JSON_UNDEFINED};

typedef struct JsonValue_struct *JsonValue;

struct JsonValue_struct {
	int	type;
	union {
		double		f;
		char *		s;
		int		b;
		struct {
			int		size;
			int		allocated;
			JsonValue	*buffer;
		} a;
		struct {
			int		size;
			int		allocated;
			char		**keys;
			JsonValue	*values;
		} o;
	} value;
};

JsonValue	JsonValueNew();
void		jsonValueFree(JsonValue self);

void		jsonValueAssign(JsonValue self, int type, void *value);
void		jsonValuePrint(JsonValue self);

int		jsonValueInvert(JsonValue self);

int		jsonValueGetType(JsonValue self);
int		jsonValueGetLength(JsonValue self);
char		*jsonValueGetAsNumber(JsonValue self);
char		*jsonValueGetAsString(JsonValue self);
JsonValue	jsonValueGet_k(JsonValue self, const char *key);
JsonValue	jsonValueGet_i(JsonValue self, int index);
char		*jsonValueGet_ik(JsonValue self, int index);
int		jsonValueSet_k(JsonValue self, const char *key, JsonValue e);
int		jsonValueAdd_i(JsonValue self, JsonValue e);

#endif
