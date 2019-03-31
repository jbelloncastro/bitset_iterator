%{
#include <cassert>  // assert
#include <cstdlib> // atoi
#include <cstdint> // uint64_t
#include <iostream> // std:cerr

uint64_t range_set( unsigned low, unsigned high ) {
    assert( low  <= high );
    assert( high < 64 );

    return ~UINT64_C(0) << low
        & ((UINT64_C(1) << high) -1);
}

uint64_t discrete_set( unsigned value ) {
    assert( value < 64 );

    return UINT64_C(1)<<value;
}

//-- Lexer prototype required by bison, aka getNextToken()
extern "C" {
    int yylex(); 
    int yyerror(const char *p) { std::cerr << "Error: " << p << "\n"; return 0; }
}
%}

%code requires {
    struct bitset_gen {
        unsigned lo;
        unsigned hi;
        uint64_t set;
    };
}

//-- SYMBOL SEMANTIC VALUES -----------------------------
%union {
    unsigned num;
    uint64_t set;
};
%token       COMMA DASH ERROR
%token <num> NUMBER
%type  <set> range exp

//-- GRAMMAR RULES ---------------------------------------
%%
exp: range COMMA exp     { $$ = $1 | $3; }
    | range              { $$ = $1; }

range: NUMBER            { $$ = discrete_set($1); }
    | NUMBER DASH NUMBER { $$ = range_set($1, $3); }

%%
//-- FUNCTION DEFINITIONS ---------------------------------
int main()
{
  yyparse();
  return 0;
}

