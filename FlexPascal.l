%{
	#include "Pascal1.tab.h"
%}

%% 

\n|\t|" " {printf("");}
"\""."\"" {printf("");}
"program" {return PROGRAM;}
"." {return DOT;}
"(" {return LPAREN;}
")" {return RPAREN;}
"uses" {return USES;}
";" {return SEMI;}
"label" {return LABEL;}
"," {return COMMA;}
"const" {return CONST;}
"=" {return EQUAL;}
"+" {return PLUS;}
"-" {return MINUS;}
"true" {return TRUE;}
"false" {return FALSE;}
"type" {return TYPE;}
"function" {return FUNCTION;}
":=" {return ASSIGN;}
":" {return COLON;}
"procedure" {return PROCEDURE;}
"char" {return CHAR;}
"int" {return INT;}
"boolean" {return BOOLEAN;}
"string" {return STRING;}
"packed" {return PACKED;}
"begin" {return BEGINX;}
"[" {return LBRACK;}
"]" {return RBRACK;}
"(." {return LBRACK2;}
".)" {return RBRACK2;}
"record" {return RECORD;}
"end" {return ENDX;}	
"case" {return CASEX;}
"of" {return OF;}
"set" {return SET;}
"file" {return FILEX;}
"^" {return POINTER;}
"@" {return AT;}
"var" {return VAR;}
"<>" {return NOT_EQUAL;}
">" {return GT;}
"<" {return LT;}
">=" {return GE;}
"<=" {return LE;}
"in" {return IN;}
"*" {return STAR;}
"/" {return SLASH;}
"mod" {return MOD;}
"nil" {return NILX;}
"and" {return ANDX;}
"not" {return NOT;}
"goto" {return GOTO;}
"if" {return IFX;}
"then" {return THEN;}
"else" {return ELSEX;}
"while" {return WHILE;}
"do" {return DOX;}
"repeat" {return REPEAT;}
"until" {return UNTIL;}
"for" {return FOR;}
"to" {return TO;}
"downto" {return DOWNTO;}
"with" {return WITH;}
[0-9]+ {return NUM_INT;}
[a-zA-Z][a-zA-Z0-9_]* {return IDENT;}
. {return yytext[0];}

%%

int yywrap()
{
	return 1;
}
