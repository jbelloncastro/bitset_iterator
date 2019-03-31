%{
#include "bitset_parse.tab.hh"
%}

%option noyywrap

%%

[0-9]+   { yylval.num = atoi(yytext); return NUMBER; }
[-]      { return DASH; }
[,]      { return COMMA; }
.        { return ERROR; }
%%

