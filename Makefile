
NRLEX=../aslt/nrlex
RDPPGEN=../aslt/rdppgen

CFLAGS+=-I. -gstabs

all: libabjson.a jsonparser

jsonparser: jsonparser.o libabjson.a
	${CC} ${CFLAGS} -o jsonparser jsonparser.o libabjson.a

libabjson.a: json_lex.nrlex json_parser.rdpp jsonvalue.o 
	${RDPPGEN} json_parser.rdpp
	mv json_parser.h json_parser.tokens.h abjson
	${NRLEX} < json_lex.nrlex > json_lex.c
	${CC} ${CFLAGS} -c json_parser.c
	${CC} ${CFLAGS} -c json_lex.c
	${AR} rcs libabjson.a json_lex.o json_parser.o jsonvalue.o

clean:
	rm -f *.o json_lex.c json_parser.c abjson/json_parser.h abjson/json_parser.tokens.h jsonparser libabjson.a

