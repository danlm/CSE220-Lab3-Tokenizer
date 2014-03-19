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
static ??? get_char(???);
static char* skip_comment(char* ch)
static char* skip_blanks(char* ch);
static ??? get_word(???);
static ??? get_number(???);
static ??? get_string(???);
static ??? get_special(???);
static char* downshift_word(char* ch);
static BOOLEAN is_reserved_word(char* word);

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
    for(i=0;i<256;i++)
    {
	if (i<32){
		char_table[i] = EOF_CODE;
	}
	elseif (i<48){
		char_table[i] = SPECIAL;
	}
	elseif (i<58){
		char_table[i] = DIGIT;
	}
	elseif (i<65){
		char_table[i] = SPECIAL;
	}
	elseif (i<91){
		char_table[i] = LETTER;
	}
	elseif (i<97){
		char_table[i] = SPECIAL;
	}
	elseif (i<123){
		char_table[i] = LETTER;
	}
	elseif (i<145){
		char_table[i] = SPECIAL;
	}
	elseif (i<149){
		char_table[i] = QUOTE;
	}
	else{
		char_table[i] = SPECIAL;
	}		
    }
    char_table[34] = QUOTE; // Double quotes
    char_table[39] = QUOTE; // Single quotes
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
    char token_string[MAX_TOKEN_STRING_LENGTH]; //Store your token here as you build it.
    char *token_ptr; //write some code to point this to the beginning of token_string
    Token* token;  //I am missing the most important variable in the function, what is it?  Hint: what should I return?
    
    //1.  Skip past all of the blanks
    //2.  figure out which case you are dealing with LETTER, DIGIT, QUOTE, EOF, or special, by examining ch
    //3.  Call the appropriate function to deal with the cases in 2.
    
    return token; //What should be returned here?
}
static ??? get_char(???)
{
    /*
     If at the end of the current line (how do you check for that?),
     we should call get source line.  If at the EOF (end of file) we should
     set the character ch to EOF and leave the function.
     */
    
    /*
     Write some code to set the character ch to the next character in the buffer
     */
}
static char* skip_blanks(char* ch)
{
    /*
     Write some code to skip past the blanks in the program and return a pointer
     to the first non blank character
     */
    while(*(ch) == ' '){
	++ch;
	}
    return ch;
}
static char* skip_comment(char* ch) //Acts only on a single line
{
    /*
     Write some code to skip past the comments in the program and return a pointer
     to the first non blank character.  Watch out for the EOF character.
     */
    ch = strchr(ch, '}');
    if (!ch){
	ch = '\n';
    {
    return ch;
}
static char* get_word(char* ch)
{
    /*
     Write some code to Extract the word
     */
    char* word;
	sscanf(ch, "\s", word);
	
    //Downshift the word, to make it lower case
    downshift_word(word);
    /*
     Write some code to Check if the word is a reserved word.
     if it is not a reserved word its an identifier.
     */
	// Assign token here?
}
static int get_number(char* ch)
{
    /*
     Write some code to Extract the number and convert it to a literal number.
     */
    int x;
	sscanf(s, "\d", &x);
	return x;	
}
static char* get_string(char* ch)
{
    /*
     Write some code to Extract the string
     */
    
}
static ??? get_special(???)
{
    /*
     Write some code to Extract the special token.  Most are single-character
     some are double-character.  Set the token appropriately.
     */
}
static char* downshift_word(char* ch)
{
    /*
     Make all of the characters in the incoming word lower case.
     */
    for( ; *ch; ++ch) *ch = tolower(*ch);
    return ch;
}
static BOOLEAN is_reserved_word(char* word)
{
    /*
     Examine the reserved word table and determine if the function input is a reserved word.
     */
    BOOLEAN isReserved = FALSE;
    for(i=0;i<8;i++)
    {
		for(j=0;j<9;j++)
		{
			char* reserve = downshift_word(rwtable[i][j] -> string);	
			if(strcmp(reserve, word) == 0){
				isReserved = TRUE;
			}
		}
    }
    return isReserved;
}