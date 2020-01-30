
//Note: For the symbol table to work, the test file should have a '$' sign at the end to mark the end of file. This, however does not follow the rules of the grammar.

#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

int line = 1, col = 1;
int ca,cb,cc;
FILE *fa;
char arr[23][100] = {"begin","const","function","procedure","var","program","printf","scanf","double","float","switch","case","if","end","else","while","while","for","integer","char","do","break","exit"};
int n = 23;

typedef struct token{
	char lexeme[100];
	char value[100];
	int line;
	int column;
}token;

typedef struct node{

	char name[100];
	char type[100];
	int size;
	char scope;
	int noarg;
	char arg[100];
	char rettype[100];
	struct node *next;

}node;

int ind = 0;
node* table[100];

int i = 0;


char mostrecent[100][100];
int most = 0;
char scopevar = 'G';
int brackets = 0;
char datatypes[3][100] = {"integer","char","double"};
int datatype = 3;


char structures[5][100] = {"program","var","const","fucntion","procedure"};
int structure = 5;


token getNextToken();
void symboltable(token t);
token copy(token t, char buf[], char val[],int line, int col){
	strcpy(t.lexeme,buf);
	strcpy(t.value,val);
	t.line = line;
	t.column = col;
	return t;
}

int convert(char buf2[]){

	int i = 0;
	int number = 0;
	while(buf2[i] != '\0'){
		number = (number*10) + (buf2[i] - 48);
		i++;
	}
	return number;
}

int checkintable(char buf[]){

	int i;
	for(i = 0; i < ind; i++){
		if(strcmp(buf,table[i]->name) == 0){
			return 1;
		}
	}
	return 0;
}

int isdatatype(char buf[]){

	int i;
	for(i = 0;i < datatype; i++){
		if(strcmp(buf,datatypes[i]) == 0){
			return 1;
		}
	}
	return -1;
}

int iskeyword(char buf[]){

	int i;
	for(i = 0;i < n; i++){
		if(strcmp(buf,arr[i]) == 0){
			return 1;
		}
	}
	return -1;
}

int getsize(char buf[]){

	if(strcmp(buf, "integer") == 0){
		return sizeof(int);
	}
	else if(strcmp(buf,"double") == 0){
		return sizeof(double);
	}
	else if(strcmp(buf,"char") == 0){
		return sizeof(char);
	}
}

int isnumber(char buf[]){

	int i;
	for(i = 0; i < strlen(buf); i++){

		if(buf[i] < 48 || buf[i] > 57){
			return 0;
		}
	}
	return 1;
}
void display(node* n){

	printf("Name : %s\n Type :%s\n Size : %d\n Scope : %c\n No.Arguments : %d\n Arguments : %s\n Return Type : %s\n\n\n",n->name,n->type,n->size,n->scope,n->noarg,n->arg,n->rettype);
}

void copyinto(char arg[], char arguments[10][100], int jarg){

	int i;
	char final[1000] = " ";
	for(i = 0; i < jarg - 1; i++){
		strcat(final,arguments[i]);
		strcat(final,",");
	}
	strcat(final,arguments[i]);

	strcpy(arg,final);
}

int isfunction = 0;
int isprocedure = 0;
char args[100][100];
char argsind = 0;



void function(token t);
void procedure(token t);
void constant(token t);

void program(token t){

	
	
	i = 0;
	//buf[i] = '\0';
	token next = getNextToken();
	fseek(fa,1,SEEK_CUR);
	node* new = (node*)malloc(sizeof(node));

	strcpy(new->name,next.lexeme);
	strcpy(new->type,t.lexeme);
	new->size = 0;
	new->scope = 'G';
	new->noarg = 0;
	strcpy(new->arg,"NA");
	strcpy(new->rettype,"NA");

	table[ind++] = new;

}

char vars[100][100];
int var = 0;

void variable(token t){

	
	if(isfunction){
		scopevar = 'L';
	}
	i = 0;
	//buf[i] = '\0';
	token next = getNextToken();

	while((strcmp(next.lexeme,":") != 0)){

		if((strcmp(next.lexeme,",") != 0)){
			strcpy(vars[var++],next.lexeme);
			strcpy(args[argsind++],next.lexeme);	
		}
		i = 0;
		next = getNextToken();
	}

	char data[100];
	i = 0;
	next = getNextToken();
	strcpy(data,next.lexeme);

	for(i = 0; i < var; i++){

		node* new = (node*)malloc(sizeof(node));

		strcpy(new->name,vars[i]);
		strcpy(new->type,data);
		new->size = getsize(data);
		new->scope = scopevar;
		new->noarg = 0;
		strcpy(new->arg,"NA");
		strcpy(new->rettype,"NA");

		table[ind++] = new;
	}
	var = 0;
	i = 0;
	next = getNextToken();
	
	if(strcmp(next.lexeme,";") == 0){

		i = 0;
		next = getNextToken();
		if(strcmp(next.lexeme,"function") == 0){
			function(next);
		}
		else if(strcmp(next.lexeme,"procedure") == 0){
			procedure(next);
		}
		else if(strcmp(next.lexeme,"const") == 0){
			variable(t);
		}
		else if(strcmp(next.lexeme,"begin") == 0){

		}
		else if(strcmp(next.lexeme,"$") == 0){

		}
		else{
			fseek(fa,0-strlen(next.lexeme),SEEK_CUR);
			variable(t);
		}
	}
	else if(strcmp(next.lexeme,")") == 0){
		return;
	}

}


void function(token t){

	i = 0;
	token next = getNextToken();
	node* f = (node*)malloc(sizeof(node));
	strcpy(f->name,next.lexeme);
	strcpy(f->type,t.lexeme);
	f->size = 0;
	f->scope = 'G';
	isfunction = 1;

	i = 0;
	next = getNextToken();
	argsind = 0;
	variable(next);
	f->noarg = argsind;
	copyinto(f->arg,args,argsind);

	i = 0;
	next = getNextToken();
	if(strcmp(next.lexeme,":") == 0){

		i = 0;
		next = getNextToken();

		strcpy(f->rettype,next.lexeme);
	}

	table[ind++] = f;

	i = 0;
	next = getNextToken();

	if(strcmp(next.lexeme,";") == 0){

		i = 0;
		next = getNextToken();
		if(strcmp(next.lexeme,"function") == 0){
			function(next);
		}
		else if(strcmp(next.lexeme,"procedure") == 0){
			procedure(next);
		}
		else if(strcmp(next.lexeme,"var") == 0){
			variable(next);
		}
		else if(strcmp(next.lexeme,"const") == 0){
			variable(t);
		}
		else if(strcmp(next.lexeme,"begin") == 0){

		}
		else if(strcmp(next.lexeme,"$") == 0){

		}
	}

	isfunction = 0;

}

void procedure(token t){

	i = 0;
	token next = getNextToken();
	node* f = (node*)malloc(sizeof(node));
	strcpy(f->name,next.lexeme);
	strcpy(f->type,t.lexeme);
	f->size = 0;
	f->scope = 'G';
	isprocedure = 1;

	i = 0;
	next = getNextToken();
	argsind = 0;
	variable(next);
	f->noarg = argsind;
	copyinto(f->arg,args,argsind);

	i = 0;
	next = getNextToken();
	strcpy(f->rettype,"NA");
	table[ind++] = f;

	if(strcmp(next.lexeme,";") == 0){

		i = 0;
		next = getNextToken();
		if(strcmp(next.lexeme,"function") == 0){
			function(next);
		}
		else if(strcmp(next.lexeme,"procedure") == 0){
			procedure(next);
		}
		else if(strcmp(next.lexeme,"var") == 0){
			variable(next);
		}
		else if(strcmp(next.lexeme,"const") == 0){
			variable(t);
		}
		else if(strcmp(next.lexeme,"begin") == 0){

		}
		else if(strcmp(next.lexeme,"$") == 0){

		}
	}

	isprocedure = 0;
}

void symboltable(token t){

	if(strcmp(t.lexeme,"program") == 0){
		program(t);
	}

	if((strcmp(t.lexeme,"var") == 0) || (strcmp(t.lexeme,"const") == 0)){
		variable(t);
	}
	if(strcmp(t.lexeme,"function") == 0){
		function(t);
	}
	if(strcmp(t.lexeme,"procedure") == 0){
		procedure(t);
	}

}

token t;

token getNextToken(){

	char buf[1000];
	
	buf[i] = '\0';
	int j;
	int print = 0;

	while(ca != EOF && print == 0){

		ca = fgetc(fa);

		if(ca == '\t'){
			col += 4;
		}

		else if(ca == '/'){
			cb = fgetc(fa);

			if(cb == '/'){

				while(cb != '\n'){
					cb = fgetc(fa);
				}
			}
		}
		
		else if(ca == '{'){

			cb = fgetc(fa);
			while(cb != '}'){
				cb = fgetc(fa);
			}
		}

		else if(ca == '\"'){

			cb = fgetc(fa);
			while(cb != '\"'){
				cb = fgetc(fa);
			}
		}


		else if(ca == '='){

			if(i != 0){

				buf[i] = '\0';
				int f = 0 ;
				for(j = 0;j < n; j++){
					if(strcmp(buf,arr[j]) == 0){
						t = copy(t,buf,"Keyword",line,col);
						symboltable(t);
						col += i;
						fseek(fa,-1,SEEK_CUR);
						print = 1;
						f = 1;

						break;
					}
				}
				if(f == 0){
					print = 1;
					if(isnumber(buf) != 1){
						t = copy(t,buf,"Identifier",line,col);
						strcpy(mostrecent[most++],buf);

					}
					else{
						t = copy(t,buf,"Number/Value",line,col);
					}

					col += i;
					fseek(fa,-1,SEEK_CUR);
				}
				
			}
			else{

				buf[i++] = ca;

				cb = fgetc(fa);

				
				if(cb == '='){
					buf[i++] = cb;
					buf[i] = '\0';
					t = copy(t,buf,"Relational operator",line,col);
					col += i;
					print = 1;
				}
				else{
					buf[i] = '\0';
					t = copy(t,buf,"Assignment operator",line,col);
					col += i;
					print = 1;
				}
			}
			//col += i;
			i = 0;
			buf[i] = '\0';
		}
		else if(ca == '>' || ca == '<' || ca == '!'){

			if(i != 0){

				buf[i] = '\0';
				int f = 0 ;
				for(j = 0;j < n; j++){
					if(strcmp(buf,arr[j]) == 0){
						
						t = copy(t,buf,"Keyword",line,col);
						symboltable(t);
						col += i;
						fseek(fa,-1,SEEK_CUR);
						print = 1;
						f = 1;
						break;
					}
				}
				if(f == 0){
					print = 1;

					if(isnumber(buf) != 1){
						t = copy(t,buf,"Identifier",line,col);
					}
					else{
						t = copy(t,buf,"Number/Value",line,col);
					}
					col += i;
					fseek(fa,-1,SEEK_CUR);
				}
		
			}
			else{

				buf[i++] = ca;

				cb = fgetc(fa);

				if(cb == '='){
					buf[i++] = cb;
				}
				
				buf[i] = '\0';
				print = 1;
				t = copy(t,buf,"Relational Operator",line,col);
				col += i;
			}
			
			i = 0;
			buf[i] = '\0';

		}

		else if(ca == '&'){

			if(i != 0){

				buf[i] = '\0';
				int f = 0 ;
				for(j = 0;j < n; j++){
					if(strcmp(buf,arr[j]) == 0){
						
						t = copy(t,buf,"Keyword",line,col);
						symboltable(t);
						col += i;
						fseek(fa,-1,SEEK_CUR);
						print = 1;
						f = 1;
						break;
					}
				}
				if(f == 0){
					print = 1;

					if(isnumber(buf) != 1){
						t = copy(t,buf,"Identifier",line,col);
					}
					else{
						t = copy(t,buf,"Number/Value",line,col);
					}
					col += i;
					fseek(fa,-1,SEEK_CUR);
				}
		
			}
			else{

				buf[i++] = ca;

				cb = fgetc(fa);

				if(cb == '&'){
					buf[i++] = cb;
					buf[i] = '\0';
					print = 1;
					t = copy(t,buf,"Relational Operator",line,col);
					col += i;
				}

			}
			i = 0;
			buf[i] = '\0';

		}


		else if(ca == ')' || ca == ';' || ca == ':' || ca == ',' || ca == '[' || ca == ']'){

			

			if(i != 0){

				buf[i] = '\0';
				int f = 0 ;
				for(j = 0;j < n; j++){
					if(strcmp(buf,arr[j]) == 0){
						t = copy(t,buf,"Keyword",line,col);
						symboltable(t);

						col += i;
						fseek(fa,-1,SEEK_CUR);
						print = 1;
						f = 1;
						break;
					}
				}
				if(f == 0){
					print = 1;
					

					if(isnumber(buf) != 1){
						t = copy(t,buf,"Identifier",line,col);
					}
					else{
						t = copy(t,buf,"Number/Value",line,col);
					}
					col += i;
					fseek(fa,-1,SEEK_CUR);
				}
				
			}
			else{

				buf[i++] = ca;
				
				buf[i] = '\0';
				print = 1;
				t = copy(t,buf,"Symbol",line,col);
				col += i;

			}
			//col += i;
			i = 0;
			buf[i] = '\0';

		}

		else if(ca == '('){

			if(i != 0){

				buf[i] = '\0';
				int f = 0 ;
				for(j = 0;j < n; j++){
					if(strcmp(buf,arr[j]) == 0){
						t = copy(t,buf,"Keyword",line,col);
						symboltable(t);

						col += i;
						fseek(fa,-1,SEEK_CUR);
						print = 1;
						f = 1;
						break;
					}
				}
				if(f == 0){
					print = 1;
					

					if(isnumber(buf) != 1){
						t = copy(t,buf,"Identifier",line,col);
						symboltable(t);
					}
					else{
						t = copy(t,buf,"Number/Value",line,col);
					}
					
					col += i;
					fseek(fa,-1,SEEK_CUR);
				}
				
			}
			else{

				cb = fgetc(fa);
				if(cb == '*'){

					cb = fgetc(fa);
					cc = fgetc(fa);
					while(!(cb == '*' && cc == ')')){

						cb = cc;
						cc = fgetc(fa);
					}
				}

				else{

					fseek(fa,-1,SEEK_CUR);

					buf[i++] = ca;
					
					buf[i] = '\0';
					print = 1;
					t = copy(t,buf,"Symbol",line,col);
					col += i;
				}

			}
			//col += i;
			i = 0;
			buf[i] = '\0';



		}


		else if(ca == '+' || ca == '-' || ca == '*' || ca == '/'){

			if(i != 0){

				buf[i] = '\0';
				int f = 0 ;
				for(j = 0;j < n; j++){
					if(strcmp(buf,arr[j]) == 0){
						
						t = copy(t,buf,"Keyword",line,col);
						symboltable(t);
						col += i;
						fseek(fa,-1,SEEK_CUR);
						print = 1;
						f = 1;
						break;
					}
				}
				if(f == 0){
					print = 1;
					
					if(isnumber(buf) != 1){
						t = copy(t,buf,"Identifier",line,col);
					}
					else{
						t = copy(t,buf,"Number/Value",line,col);
					}
					col += i;
					fseek(fa,-1,SEEK_CUR);
				}
				
			}
			else{

				buf[i++] = ca;
				
				buf[i] = '\0';
				print = 1;
				t = copy(t,buf,"Arithmetic operator",line,col);
				col += i;

			}
			//col+= i;
			i = 0;
			buf[i] = '\0';

		}

		else if(ca == '$'){

			if(i != 0){

				buf[i] = '\0';
				int f = 0 ;
				for(j = 0;j < n; j++){
					if(strcmp(buf,arr[j]) == 0){
						
						t = copy(t,buf,"Keyword",line,col);
						symboltable(t);
						col += i;
						fseek(fa,-1,SEEK_CUR);
						print = 1;
						f = 1;
						break;
					}
				}
				if(f == 0){
					print = 1;
					
					if(isnumber(buf) != 1){
						t = copy(t,buf,"Identifier",line,col);
					}
					else{
						t = copy(t,buf,"Number/Value",line,col);
					}
					col += i;
					fseek(fa,-1,SEEK_CUR);
				}
				
			}
			else{

				buf[i++] = ca;
				
				buf[i] = '\0';
				print = 1;
				t = copy(t,buf,"End operator",line,col);
				col += i;

			}
			//col+= i;
			i = 0;
			buf[i] = '\0';
		}

		else if(ca == ' ' || ca == '\n'){

			if(i != 0){

				buf[i] = '\0';
				int f = 0 ;
				for(j = 0;j < n; j++){
					if(strcmp(buf,arr[j]) == 0){
						
						t = copy(t,buf,"Keyword",line,col);
						symboltable(t);
						col += i;
						fseek(fa,-1,SEEK_CUR);
						print = 1;
						f = 1;
						break;
					}
				}
				if(f == 0){
					print = 1;
					
					if(isnumber(buf) != 1){
						t = copy(t,buf,"Identifier",line,col);
					}
					else{
						t = copy(t,buf,"Number/Value",line,col);
					}
					col += i;
					fseek(fa,-1,SEEK_CUR);
				}
				//col += i;
				i = 0;
				buf[i] = '\0';

			}

		}
		else{

			buf[i++] = ca;
		}
		if(print == 1){
			return t;
		}

	}

}


int main(){

	fa = fopen("in.txt","r");
	if(fa == NULL){
		printf("File does not exist!\n");
		exit(0);
	}

	int in = 1;
	token p = getNextToken();
	while(strcmp(p.value,"End operator") != 0){
		p = getNextToken();
	}

	for(int i = 0;i < ind; i++){
		display(table[i]);
		printf("\n");
	}

	printf("\n");
}
