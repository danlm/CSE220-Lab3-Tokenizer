//
//  scanner.c
//  Lab3
//
//  Created by Bryce Holton.
//  Copyright (c) 2014 Bryce Holton. All rights reserved.
//  
//  Student Name: Daniel Martin

#include <stdio.h>
#include "scanner.h"

/*******************
 Static functions needed for the scanner
 You need to design the proper parameter list and 
 return types for functions with ???.
 ******************/
static char get_char(char** ch);
static char* skip_comment(char** ch);
static char* skip_blanks(char** ch);
static char* get_word(char** original, char ch[], char* ch_ptr, Token* t);
static double get_number(char** original, char ch[], char* ch_ptr);
static char* get_string(char** original, char ch[], char* ch_ptr);
static char* get_special(char** original, char ch[], char* ch_ptr, Token* t);
static char* downshift_word(char ch[]);
static BOOLEAN is_reserved_word(char word[], Token* tok);

typedef enum
{
    LETTER, DIGIT, QUOTE, SPECIAL, EOF_CODE,
}
CharCode;

/*********************
 Static Variables for Scanner
 Must be initialized in the init_scanner function.
 *********************/
static FILE *src_file;
static char src_name[MAX_FILE_NAME_LENGTH];
static char todays_date[DATE_STRING_LENGTH];
static CharCode char_table[256];  // The character table

typedef struct
{
    char *string;
    TokenCode token_code;
}
RwStruct;

const RwStruct rw_table[9][10] = {
    {{"do",DO},{"if",IF},{"in",IN},{"of",OF},{"or",OR},{"to",TO},{NULL,0}}, //Reserved words of size 2
    {{"and",AND},{"div",DIV},{"end",END},{"for",FOR},{"mod",MOD},{"nil",NIL},{"not",NOT},{"set",SET},{"var",VAR},{NULL,0}}, //Reserved words of size 3
    {{"case",CASE},{"else",ELSE},{"file",FFILE},{"goto",GOTO},{"then",THEN},{"type",TYPE},{"with",WITH},{NULL,0}}, //Reserved words of size 4
    {{"array",ARRAY},{"begin",BEGIN},{"const",CONST},{"label",LABEL},{"until",UNTIL},{"while",WHILE},{NULL,0}},  //Reserved words of size 5
    {{"downto",DOWNTO}, {"packed",PACKED},{"record",RECORD}, {"repeat",REPEAT},{NULL,0}},  // Reserved words of size 6
    {{"program", PROGRAM},{NULL,0}}, // Reserved words of size 7
    {{"function", FUNCTION},{NULL,0}}, // Reserved words of size 8
    {{"procedure", PROCEDURE},{NULL,0}}  // Reserved words of size 9
};

void init_scanner(FILE *source_file, char source_name[], char date[])
{
    src_file = source_file;
    strcpy(src_name, source_name);
    strcpy(todays_date, date);
    
    /*******************
     initialize character table, this table is useful for identifying what type of character 
     we are looking at by setting our array up to be a copy the ascii table.  Since C thinks of 
     a char as like an int you can use ch in get_token as an index into the table.
     *******************/
	int i;
    for(i=0;i<256;i++)
    {
		if((i >= '0') && (i <= '9')){
			char_table[i] = DIGIT;
		}
		else if(((i >= 'a') && (i <= 'z')) || ((i >= 'A') && (i <= 'Z'))){
			char_table[i] = LETTER;
		}
		else{
			char_table[i] = SPECIAL;
		} 
	}
}
BOOLEAN get_source_line(char source_buffer[])
{
    char print_buffer[MAX_SOURCE_LINE_LENGTH + 9];
//    char source_buffer[MAX_SOURCE_LINE_LENGTH];  //I've moved this to a function parameter.  Why did I do that?
    static int line_number = 0;
    
    if (fgets(source_buffer, MAX_SOURCE_LINE_LENGTH, src_file) != NULL)
    {
        ++line_number;
        sprintf(print_buffer, "%4d: %s", line_number, source_buffer);
        print_line(print_buffer, src_name, todays_date);
        return (TRUE);
    }
    else
    {
        return (FALSE);
    }
}
Token* get_token()
{
    char ch; //This can be the current character you are examining during scanning.
    static char* current_char = src_name; //This is the pointer to the current character being read
    char token_string[MAX_TOKEN_STRING_LENGTH]; //Store your token here as you build it.
    char *token_ptr = token_string; //write some code to point this to the beginning of token_string
    Token* token = (Token *) malloc(sizeof(Token));  //I am missing the most important variable in the function, what is it?  Hint: what should I return?
    ch = get_char(&current_char);
    
    //1.  Skip past all of the blanks
    //2.  figure out which case you are dealing with LETTER, DIGIT, QUOTE, EOF, or special, by examining ch
    //3.  Call the appropriate function to deal with the cases in 2.
	if (ch == ' '){
		ch = *(skip_blanks(&current_char));
	}
	else if (ch == '{'){
		ch = *(skip_comment(&current_char));
	}

	if (ch == '\n'){
		ch = get_char(&current_char);
	}
	
    if (char_table[ch] = DIGIT){
		token_string[0] = ch;
    	token -> literal_type = INTEGER_LIT;
    	token -> token_code = NUMBER;
    	get_number(&current_char, token_string, token_ptr);
    }
    else if (ch == '\''){
    	token -> literal_type = STRING_LIT;
    	token -> token_code = STRING;
    	get_string(&current_char,token_string, token_ptr);
    }
    else if (char_table[ch] = LETTER){
		token_string[0] = ch;
    	token -> literal_type = REAL_LIT;
		get_word(&current_char,token_string, token_ptr, token);
    }
    else if (char_table[ch] = SPECIAL){
    	token -> literal_type = REAL_LIT;
		token_string[0] = ch;
    	get_special(&current_char,token_string, token_ptr, token);
    }
	token -> content = (char *) malloc(sizeof(char) * strlen(token_string));
    strcpy(token -> content, token_string);
    return token; //What should be returned here?
}
char get_char(char** ch)
{
    /*
     If at the end of the current line (how do you check for that?),
     we should call get source line.  If at the EOF (end of file) we should
     set the character ch to EOF and leave the function.
     */
    if(*(*ch) == '\n'){
    	if(get_source_line(src_name)){
			(*ch) = src_name;
		}
		else{
			*(*ch) = EOF;
		}
    }
    else if(*(*ch) != '\0'){
    	(*ch)++;
    }
    /*
     Write some code to set the character ch to the next character in the buffer
     */
     return *(*ch);
}
char* skip_blanks(char** ch)
{
    /*
     Write some code to skip past the blanks in the program and return a pointer
     to the first non blank character
     */
    while(*(*ch) == ' '){
		(*ch)++;
	}
    return (*ch);
}
char* skip_comment(char** ch) //Acts only on a single line
{
    /*
     Write some code to skip past the comments in the program and return a pointer
     to the first non blank character.  Watch out for the EOF character.
     */
    while(*(*ch) != '}'){
		(*ch)++; //Move pointer to ending brace
    }
	(*ch)++; // Move pointer to character after brace
    if (*(*ch) == ' '){
		skip_blanks(ch);
    }
    return (*ch);
}
char* get_word(char** original, char ch[], char* ch_ptr, Token* t)
{
    /*
     Write some code to Extract the word
     */
    ch_ptr = ch;
    int i = 1;
    while(isalpha(get_char(original))){
		ch[i] = get_char(original);
		i++;
    }
    ch[i] = '\0';
	
    //Downshift the word, to make it lower case
    downshift_word(ch);
    /*
     Write some code to Check if the word is a reserved word.
     if it is not a reserved word its an identifier.
     */
	if(is_reserved_word(ch, t) == FALSE){
		t -> token_code = IDENTIFIER;
	}
	return ch_ptr;
}
double get_number(char** original, char ch[], char* ch_ptr)
{
    /*
     Write some code to Extract the number and convert it to a literal number.
     */
    ch_ptr = ch;
    int i = 1;
    while((get_char(original) != ' ') && (*(*original) != '\n')){
		ch[i] = get_char(original);
		i++;
    }
    ch[i] = '\0';
    return strtod(ch, &ch_ptr);
}
char* get_string(char** original, char ch[], char* ch_ptr)
{
    /*
     Write some code to Extract the string
     */
    ch_ptr = ch;
    int i = 1;
    while(get_char(original) != '\''){
	ch[i] = get_char(original);
	i++;
    }
    ch[i] = '\0';
    return ch_ptr;
}
char* get_special(char** original, char ch[], char* ch_ptr, Token* t)
{
    /*
     Write some code to Extract the special token.  Most are single-character
     some are double-character.  Set the token appropriately.
     */
    ch_ptr = ch;
	char add = get_char(original);
    if((add == '=') || (add == '>') || (add == '.')){
		ch[1] = add;
		ch[2] = '\0';
    }
	else{
		ch[1] = '\0';
		(*original)--;
	}
    if(strlen(ch) == 1){
		switch(ch[0]){
			case '^':
				t -> token_code = UPARROW;
				break;
		
			case '*':
				t -> token_code = STAR;
				break;
		
			case '(':
				t -> token_code = LPAREN;
				break;
		
			case ')':
				t -> token_code = RPAREN;
				break;
		
			case '-':
				t -> token_code = MINUS;
				break;
		
			case '+':
				t -> token_code = PLUS;
				break;
		
			case '=':
				t -> token_code = EQUAL;
				break;
		
			case '[':
				t -> token_code = LBRACKET;
				break;
		
			case ']':
				t -> token_code = RBRACKET;
				break;
		
			case ':':
				t -> token_code = COLON;
				break;
		
			case ';':
				t -> token_code = SEMICOLON;
				break;
		
			case '<':
				t -> token_code = LT;
				break;
		
			case '>':
				t -> token_code = GT;
				break;
		
			case ',':
				t -> token_code = COMMA;
				break;
		
			case '.':
				t -> token_code = PERIOD;
				break;

			case '/':
				t -> token_code = SLASH;
				break;
		}
    }
    else{
		switch(ch[0]){
			case ':':
				t -> token_code = COLONEQUAL;
				break;
		
			case '<':
				switch(ch[1]){
					case '=':
						t -> token_code = LE;
					break;

					case '>':
						t -> token_code = NE;
					break;
				}
				break;
		
			case '>':
				t -> token_code = GE;
				break;
		
			case '.':
				t -> token_code = DOTDOT;
				break;
		}
    }
    return ch_ptr;
    
}
char* downshift_word(char ch[])
{
    /*
     Make all of the characters in the incoming word lower case.
     */
	int i;
    for(i=0; ch[i]; ++ch){
	 ch[i] = tolower(ch[i]);
    }
    return ch;
}
BOOLEAN is_reserved_word(char word[], Token* tok)
{
    /*
     Examine the reserved word table and determine if the function input is a reserved word.
     */
	int i,j;
    BOOLEAN isReserved = FALSE;
    for(i=0;i<8;i++)
    {
		for(j=0;j<9;j++)
		{	
			if(strcasecmp((rw_table[i][j]).string, word) == 0){
				isReserved = TRUE;
				tok -> token_code = (rw_table[i][j]).token_code;
			}
		}
    }
    return isReserved;
}
