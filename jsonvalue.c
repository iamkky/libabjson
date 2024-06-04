#include <stdlib.h>
#include <string.h>
#include <stdio.h>
//#include "mpaland/printf.h"
#include "abd/printf.h"
#include "abjson/jsonvalue.h"

//#include "abd/debug.h" 

// Implements a C representation for JSON values

JsonValue JsonValueNew()
{
JsonValue self;
	
	if((self = malloc(sizeof(struct JsonValue_struct)))==NULL) {
		return NULL;
	}

	self->type = JSON_NULL;
	self->value.b = 0;

	return self;
}

void jsonValueSetNull(JsonValue self)
{
int c;

	switch(self->type){
	case JSON_STRING:
		if(self->value.s) free(self->value.s);
		break;
	case JSON_OBJECT:
		for(c=0; c<self->value.o.size; c++){
			free(self->value.o.keys[c]);
			jsonValueFree(self->value.o.values[c]);
		}

		if(self->value.o.keys) free(self->value.o.keys);
		if(self->value.o.values) free(self->value.o.values);
		break;
	case JSON_ARRAY:
		for(c=0; c<self->value.a.size; c++){
			jsonValueFree(self->value.a.buffer[c]);
		}

		if(self->value.a.buffer) free(self->value.a.buffer);
		break;
	}
	
	self->type = JSON_NULL;
	self->value.b = 0;
}

void jsonValueFree(JsonValue self)
{
	if(self==NULL) return;

	jsonValueSetNull(self);

	free(self);
}

void jsonValueAssign(JsonValue self, int type, void *value)
{
	if(self==NULL) return;

	jsonValueSetNull(self);

	switch(self->type = type){
	case JSON_NULL:
		self->value.b = 0;
		break;
	case JSON_NUMBER:
		self->value.f = *((double *)value);
		break;
	case JSON_BOOLEAN:
		self->value.b = *((int *)value);
		break;
	case JSON_STRING:
		self->value.s = strdup((char *)value);
		break;
	case JSON_OBJECT:
		self->value.o.size = 0;
		self->value.o.allocated = 0;
		self->value.o.keys = NULL;
		self->value.o.values = NULL;
		break;
	case JSON_ARRAY:
		self->value.a.size = 0;
		self->value.a.allocated = 0;
		self->value.a.buffer = NULL;
		break;
	}

	return;
}

int jsonValueSet_k(JsonValue self, const char *key, JsonValue e)
{
int allocated;

	if(self==NULL) return -1;
	if(key==NULL) return -1;
	if(e==NULL) return -1;
	if(self->type!=JSON_OBJECT) return -1;

	allocated = self->value.o.allocated;

	if(self->value.o.allocated <= self->value.o.size){
		if(self->value.o.allocated == 0){
			self->value.o.keys = malloc(sizeof(char *) * 32);
			self->value.o.values = malloc(sizeof(JsonValue) * 32);
			self->value.o.allocated = 32;
		}else{
			self->value.o.keys = realloc(self->value.o.keys, sizeof(char *) * 2 * allocated);
			self->value.o.values = realloc(self->value.o.values, sizeof(JsonValue) * 2 * allocated);
			self->value.o.allocated = allocated * 2;
		}
	}

	self->value.o.keys[self->value.o.size] = strdup(key);
	self->value.o.values[self->value.o.size] = e;

	self->value.o.size = self->value.o.size + 1;

	return 0;
}

int jsonValueAdd_i(JsonValue self, JsonValue e)
{
int allocated;

	if(self==NULL) return -1;
	if(self->type!=JSON_ARRAY) return -1;

	allocated = self->value.o.allocated;

	if(self->value.a.allocated <= self->value.a.size){
		if(self->value.a.allocated == 0){
			self->value.a.buffer = malloc(sizeof(JsonValue) * 64);
			self->value.a.allocated = 64;
		}else{
			self->value.a.buffer = realloc(self->value.a.buffer, sizeof(JsonValue) * 2 * allocated);
			self->value.a.allocated = allocated * 2;
		}
	}

	self->value.a.buffer[self->value.a.size] = e;
	self->value.a.size = self->value.a.size + 1;

	return 0;
}

int jsonValueInvert(JsonValue self)
{
JsonValue tmp, *values;
char	  **keys, *ktmp;
int	  size, n;

	if(self->type==JSON_ARRAY) {
		size = self->value.a.size;
		values = self->value.a.buffer;

		for(n=0; n<size/2; n++){
			tmp = values[n];
			values[n] = values[size-n-1];
			values[size-n-1] = tmp;
		}

		return 0;
	}

	if(self->type==JSON_OBJECT) {
		size = self->value.o.size;
		keys = self->value.o.keys;
		values = self->value.o.values;

		for(n=0; n<size/2; n++){
			tmp = values[n];
			values[n] = values[size-n-1];
			values[size-n-1] = tmp;

			ktmp = keys[n];
			keys[n] = keys[size-n-1];
			keys[size-n-1] = ktmp;
		}

		return 0;
	}

	return -1;
}

int jsonValueGetType(JsonValue self)
{
	return self->type;
}

int jsonValueGetLength(JsonValue self)
{
	if(self==NULL) return 0;

	if(self->type==JSON_ARRAY) return self->value.a.size;
	return 0;
}

JsonValue jsonValueGet_k(JsonValue self, const char *key)
{
	if(self==NULL) return NULL;
	if(key==NULL) return NULL;
	if(self->type!=JSON_OBJECT) return NULL;

	for(int c=0; c<self->value.o.size; c++){
		if(!strcmp(self->value.o.keys[c], key)){
			return self->value.o.values[c];
		}
	}

	return NULL;
}

JsonValue jsonValueGet_i(JsonValue self, int index)
{
	if(self==NULL) return NULL;

	if(self->type!=JSON_ARRAY) return NULL;

	if(index<0 || index>=self->value.a.size) return NULL;

	return self->value.a.buffer[index];
}

char *jsonValueGetAsString(JsonValue self)
{
char buffer[64];

	if(self==NULL) return strdup("(null)");

	switch(self->type){
	case JSON_NULL:
		return strdup("(null)");
	case JSON_NUMBER:
		snprintf(buffer, 60, "%f", self->value.f);
		return strdup(buffer);
	case JSON_BOOLEAN:
		if(self->value.b)
			return strdup("true");
		else
			return strdup("false");
	case JSON_STRING:
		return strdup(self->value.s);
	case JSON_OBJECT:
		return strdup("object");
	case JSON_ARRAY:
		return strdup("array");
	}

	return strdup("undefined");
}

// Print functions;

void jsonValuePrint(JsonValue self)
{
int n = 0;

	if(self==NULL) return;

	switch(self->type){
	case JSON_NULL:
		printf("null");
		break;
/*
	case JSON_INTEGER:
		printf("%" PRId64, self->value.i);
		break;
*/
	case JSON_NUMBER:
		printf("%f", self->value.f);
		break;
	case JSON_BOOLEAN:
		printf(self->value.b ? "true" : "false");
		break;
	case JSON_STRING:
		printf("\"%s\"", self->value.s);
		break;
	case JSON_OBJECT:
		printf("{");
		for(n=0; n<self->value.o.size; n++){
			if(n>0) printf(", ");
			printf("\"%s\":", self->value.o.keys[n]);
			jsonValuePrint(self->value.o.values[n]);
		}
		printf("}");
		break;
	case JSON_ARRAY:
		printf("[");
		for(n=0; n<self->value.a.size; n++){
			if(n>0) printf(", ");
			jsonValuePrint(self->value.a.buffer[n]);
		}
		printf("]");
		break;
	}
}

	

