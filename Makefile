
include make.wasm

NRLEX=nrlex
RDPPGEN=rdppgen

CFLAGS+=-I.

all: libabjson.a 

jsonparser: jsonparser.o libabjson.a
	${CC} ${LDFLAGS} -o jsonparser jsonparser.o libabjson.a -labd

libabjson.a: json_lex.nrlex json_parser.rdpp
	${RDPPGEN} json_parser.rdpp
	mv json_parser.h json_parser.tokens.h abjson
	${NRLEX} < json_lex.nrlex > json_lex.c
	${CC} ${CFLAGS} -c json_parser.c
	${CC} ${CFLAGS} -c json_lex.c
	${AR} rcs libabjson.a json_lex.o json_parser.o

clean:
	rm -f *.o json_lex.c json_parser.c abjson/json_parser.h abjson/json_parser.tokens.h jsonparser libabjson.a

