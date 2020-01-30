%{
  	#include <stdio.h>
   #include <stdlib.h>
	int yyerror();
   int yylex();
	extern FILE *yyin;
%}

%token PROGRAM DOT LPAREN RPAREN USES SEMI LABEL COMMA CONST 
%token EQUAL PLUS MINUS TRUE FALSE TYPE FUNCTION COLON 
%token PROCEDURE CHAR INT BOOLEAN STRING PACKED LBRACK RBRACK %token LBRACK2 RBRACK2 RECORD OF SET ARRAY BEGINX CHR DIV 
%token IMPLEMENTATION FILEX POINTER AT VAR ASSIGN NOT_EQUAL GT 
%token LT GE LE IN STAR SLASH MOD NILX ANDX NOT GOTO IFX THEN
%token ELSEX CASEX ENDX WHILE DOX REPEAT UNTIL FOR TO DOWNTO WITH 
%token NUM_INT IDENT OR REAL STRING_LITERAL UNIT INTEGER 
%token INTERFACE NUM_REAL

%%

programx
   : program {printf("String accepted"); exit(0);}
   ;

program
   : program_heading block_part DOT
   | program_heading INTERFACE block_part DOT 
   ;

program_heading
   : PROGRAM identifier SEMI
   | PROGRAM identifier LPAREN identifier_list RPAREN SEMI
   | UNIT identifier SEMI
   ;

identifier
   : IDENT
   ;

block_part
   : block compound_statement
   ;

block
   : label_declaration_part block
   | constant_declaration_part block
   | type_definition_part block
   | variable_definition_part block
   | procedure_and_function_declaration_part block
   | uses_units_part block
   | IMPLEMENTATION block
   |
   ;

uses_units_part
   : USES identifier_list SEMI
   ;

label_declaration_part
   : LABEL label label_add SEMI
   ;

label_add
   : COMMA label label_add
   | 
   ;

label
   : unsigned_integer
   ;

constant_declaration_part
   : CONST constant_definition SEMI constant_add
   ;

constant_add
   : constant_definition SEMI constant_add
   |
   ;

constant_definition
   : identifier EQUAL constant
   ;

constant_char
   : CHR LPAREN unsigned_integer RPAREN
   ;

constant
   : unsigned_number
   | sign unsigned_number
   | identifier
   | sign identifier
   | string
   | constant_char
   ;

unsigned_number
   : unsigned_integer
   | unsigned_real
   ;

unsigned_integer
   : NUM_INT
   ;

unsigned_real
   : NUM_REAL
   ;

sign
   : PLUS
   | MINUS
   ;

bool
   : TRUE
   | FALSE
   ;

string
   : STRING_LITERAL
   ;

type_definition_part
   : TYPE type_definition SEMI type_add
   ;

type_add
   : type_definition SEMI type_add
   | 
   ;

type_definition
   : identifier EQUAL type 
   | identifier EQUAL function_type 
   | identifier EQUAL procedure_type
   ;

function_type
   : FUNCTION formal_parameter_list COLON result_type
   | FUNCTION COLON result_type
   ;

procedure_type
   : PROCEDURE formal_parameter_list
   | PROCEDURE
   ;

type
   : simple_type
   | structured_type
   | pointer_type
   ;

simple_type
   : scalar_type
   | subrange_type
   | type_identifier
   | string_type
   ;

scalar_type
   : LPAREN identifier_list RPAREN
   ;

subrange_type
   : constant DOT DOT constant
   ;

type_identifier
   : identifier
   | CHAR 
   | BOOLEAN
   | INTEGER 
   | REAL 
   | STRING
   ;

structured_type
   : PACKED unpacked_structure_typed_type
   | unpacked_structure_typed_type
   ;

unpacked_structure_typed_type
   : array_type
   | record_type
   | set_type
   | file_type
   ;

string_type
   : STRING LBRACK identifier RBRACK
   | STRING LBRACK unsigned_number RBRACK
   ;

array_type
   : ARRAY LBRACK type_list RBRACK OF component_type
   | ARRAY LBRACK2 type_list RBRACK2 OF component_type
   ;

type_list
   : index_type index_add
   ;

index_add
   : COMMA index_type index_add
   |
   ;

index_type
   : simple_type
   ;

component_type
   : type
   ;

record_type
   : RECORD field_list ENDX
   | RECORD ENDX
   ;

field_list
   : fixed_part
   | fixed_part SEMI variant_part
   | variant_part
   ;

fixed_part
   : record_section record_add
   ;

record_add
   : SEMI record_section record_add
   | 
   ;

record_section
   : identifier_list COLON type
   ;

variant_part
   : CASEX tag OF variant variant_add
   ;

variant_add
   : SEMI variant variant_add
   |
   ;

tag
   : identifier COLON type_identifier
   | type_identifier
   ;

variant
   : const_list COLON LPAREN field_list RPAREN
   ;

set_type
   : SET OF base_type
   ;

base_type
   : simple_type
   ;

file_type
   : FILEX OF type
   | FILEX
   ;

pointer_type
   : POINTER type_identifier
   ;

variable_definition_part
   : VAR variable_declaration variable_add SEMI
   ;

variable_add
   : SEMI variable_declaration variable_add
   |
   ;

variable_declaration
   : identifier_list COLON type
   ;

procedure_and_function_declaration_part
   : procedure_or_function_declaration SEMI
   ;

procedure_or_function_declaration
   : procedure_declaration
   | function_declaration
   ;

procedure_declaration
   : PROCEDURE identifier SEMI block_part
   | PROCEDURE identifier formal_parameter_list SEMI block_part
   ;

formal_parameter_list
   : LPAREN formal_parameter_section formal_add RPAREN
   ;

formal_add
   : SEMI formal_parameter_section formal_add
   |
   ;

formal_parameter_section
   : parameter_group
   | VAR parameter_group
   | FUNCTION parameter_group
   | PROCEDURE parameter_group
   ;

parameter_group
   : identifier_list COLON type_identifier
   ;

identifier_list
   : identifier identifier_add
   ;

identifier_add: COMMA identifier identifier_add
   | 
   ;

const_list
   : constant const_add
   ;

const_add
   : COMMA constant const_add
   | 
   ;

function_declaration
   : FUNCTION identifier COLON result_type SEMI block_part
   | FUNCTION identifier formal_parameter_list COLON result_type SEMI block_part
   ;

result_type
   : type_identifier
   ;

statement
   : label COLON unlabelled_statement
   | unlabelled_statement
   ;

unlabelled_statement
   : simple_statement
   | structured_statement
   ;

simple_statement
   : assignment_statement
   | procedure_statement
   | goto_statement
   | empty_statement
   ;

assignment_statement
   : variable ASSIGN expression
   ;

variable
   : AT identifier variable_add
   | identifier variable_add
   ;

variable_add
   : LBRACK expression expr_add RBRACK variable_add
   | LBRACK2 expression expr_add RBRACK2 variable_add
   | DOT identifier variable_add
   | POINTER variable_add
   |
   ;

expr_add
   : COMMA expression expr_add
   |
   ;

expression
   : simple_expression relational_operator expression
   | simple_expression
   ;

relational_operator
   : EQUAL
   | NOT_EQUAL
   | LT
   | LE
   | GE
   | GT
   | IN
   ;

simple_expression
   : term
   | term additive_operator simple_expression
   ;

additive_operator
   : PLUS
   | MINUS
   | OR
   ;

term
   : signed_factor multiplicative_operator term
   | signed_factor
   ;

multiplicative_operator
   : STAR
   | SLASH
   | DIV
   | MOD
   | ANDX
   ;

signed_factor
   : PLUS factor
   | MINUS factor
   | factor
   ;

factor
   : variable
   | LPAREN expression RPAREN
   | function_designator
   | unsigned_constant
   | set
   | NOT factor
   | bool
   ;

unsigned_constant
   : unsigned_number
   | constant_char
   | string
   | NILX
   ;

function_designator
   : identifier LPAREN parameter_list RPAREN
   ;

parameter_list
   : actual_parameter parameter_add
   ;

parameter_add
   : COMMA actual_parameter parameter_add
   | 
   ;

set
   : LBRACK element_list RBRACK
   | LBRACK2 element_list RBRACK2
   ;

element_list
   : element element_add
   |
   ;

element_add
   : COMMA element element_add
   |
   ;

element
   : expression
   | expression DOT DOT expression
   ;

procedure_statement
   : identifier LPAREN parameter_list RPAREN
   | identifier 
   ;

actual_parameter
   : expression actual_add
   ;

actual_add
   : parameter_width actual_add
   | 
   ;

parameter_width
   : COLON expression
   ;

goto_statement
   : GOTO label
   ;

empty_statement
   :
   ;

structured_statement
   : compound_statement
   | conditional_statement
   | repetitive_statement
   | with_statement
   ;

compound_statement
   : BEGINX statements ENDX
   ;

statements
   : statement statement_add
   ;

statement_add
   : SEMI statement statement_add
   |
   ;

conditional_statement
   : if_statement
   | case_statement
   ;

if_statement
   : IFX expression THEN statement ELSEX statement
   | IFX expression THEN statement
   ;

case_statement
   : CASEX expression OF case_list_element case_add SEMI ELSEX statements ENDX
   | CASEX expression OF case_list_element case_add ENDX
   ;

case_add
   : SEMI case_list_element case_add
   |
   ;

case_list_element
   : const_list COLON statement
   ;

repetitive_statement
   : while_statement
   | repeat_statement
   | for_statement
   ;

while_statement
   : WHILE expression DOX statement
   ;

repeat_statement
   : REPEAT statements UNTIL expression
   ;

for_statement
   : FOR identifier ASSIGN for_list DOX statement
   ;

for_list
   : initial_value TO final_value
   | initial_value DOWNTO final_value
   ;

initial_value
   : expression
   ;

final_value
   : expression
   ;

with_statement
   : WITH record_variable_list DOX statement
   ;

record_variable_list
   : variable record_add
   ;

record_add
   : COMMA variable record_add
   |
   ;

%%

int yyerror(char *msg)
{
	printf("%s \n",msg);
   printf("Invalid Expression\n");
	exit(0);
}

void main()
{
	yyin = fopen("in.txt", "r");
	while(!feof(yyin)) 
	{
		if(yyparse()){
			printf("Error!\n");
         exit(0);
      }
	}
}
	
