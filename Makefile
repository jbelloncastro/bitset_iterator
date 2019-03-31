
all: bitset_parse

bitset_parse: bitset_parse.tab.o lex.yy.o
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $@ $^

lex.yy.c: bitset_parse.l bitset_parse.tab.hh
	flex $^

# Makefile: A simple makefile for ex1.
bitset_parse.tab.hh bitset_parse.tab.cc: bitset_parse.yy
	bison -d $^

bitset_it: bitset_it.cpp
	$(CXX) $^ -o $@

clean:
	rm -f bitset_parse.tab.* lex.yy.c

