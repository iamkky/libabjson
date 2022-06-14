#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "abjson/jsonvalue.h"
#include "abjson/json_parser.h"

char *readToBuffer(int fd, int inisize, int extrasize, int *readsize)
{
int bsize, r, rsize;
char *buffer, *tmp;

	bsize=inisize;
	if((buffer = malloc(bsize + extrasize))==NULL) return NULL;

	rsize=0;

	do{
		if(rsize>=bsize){
			bsize += bsize;
			if((tmp = realloc(buffer, bsize + extrasize))==NULL){
				*readsize = 0;
				free(buffer);
				return NULL;
			}
			buffer = tmp;
		} 
		r = read(fd, buffer+rsize, bsize - rsize);
		rsize += r;
	}while(r>0);

	*readsize = rsize;
	return buffer;
}

int main(int argc, char **argv)
{
JsonValue  json;
JsonParser jp;
char *buffer;
int bytes, result;
int lcount;

	//bytes = read(0, buffer, 32767);
	buffer = readToBuffer(0, 32768, 1, &bytes);
	buffer[bytes] = 0;

	jp = JsonParserNew(buffer, &lcount);	

	lcount = 0;
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
		fprintf(stderr,"Failed to parse line %d\n", lcount);
	}

	JsonParserFree(jp);
	free(buffer);
}

