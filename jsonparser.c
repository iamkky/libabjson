#include <stdio.h>
#include <unistd.h>

#include "abjson/jsonvalue.h"
#include "abjson/json_parser.h"

int main(int argc, char **argv)
{
JsonValue  json;
JsonParser jp;
char buffer[32768];
int bytes, result;

	bytes = read(0, buffer, 32767);
	buffer[bytes]=0;

	jp = JsonParserNew(buffer);	

	result = JsonParserParse(jp);

	fprintf(stdout,"Input Size: %d\n", bytes);
	fprintf(stdout,"cursor: %d\n", jp->cursor);
	fprintf(stdout,"Last Token: %d\n", jp->currToken);

	if(result>0){
		json = jp->value[0].value;
		fprintf(stderr,"Value Type %02d\n", jsonValueGetType(json));
		jsonValuePrint(json);
		fprintf(stdout,"\n");
		jsonValueFree(json);
	}else{
		fprintf(stderr,"Failed to parse\n");
	}

	JsonParserFree(jp);
}

