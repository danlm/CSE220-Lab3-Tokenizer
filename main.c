//
//  main.c
//  Lab3
//
//  Created by Bryce Holton.
//  Copyright (c) 2014 Bryce Holton. All rights reserved.
//  
//  Student Name: Daniel Martin

#include <stdio.h>
#include "common.h"
#include "print.h"
#include "scanner.h"

FILE *init_lister(const char *name, char source_file_name[], char dte[]);
void quit_scanner(FILE *src_file, Token *list);
void add_token_to_list(Token *list, Token *new_token);

int main(int argc, const char * argv[])
{
    Token *token;
    Token *token_list; //This needs to be implemented as a linked list in scanner.h.
    char source_name[MAX_FILE_NAME_LENGTH];
    char date[DATE_STRING_LENGTH];
    FILE *source_file = init_lister(argv[1], source_name, date);
    init_scanner(source_file, source_name, date);
    
    do
    {        
		token = get_token();
		if (!token_list){
			token_list = token;
		}
        add_token_to_list(token_list, token);
        print_token(token);
    }
    while (get_token() != NULL);//What is the sentinel value that ends this loop?
    
    quit_scanner(source_file, token_list);
    return 0;
}
void add_token_to_list(Token *list, Token *new_token)
{
    // Add new_token to the list knowing that list is a linked list.
	Token* next = list;
	while(next -> next){
		next = next -> next;
	}
    next -> next = new_token;
}
void quit_scanner(FILE *src_file, Token *list)
{
    //write code to free all of the memory for the token list
    Token *temp;
    while (list != NULL){
		temp = list -> next;
		free(temp);
	}
    fclose(src_file);
}
FILE *init_lister(const char *name, char source_file_name[], char dte[])
{
    time_t timer;
    FILE *file;
    
    memcpy(source_file_name, name, strlen(source_file_name));
    file = fopen(source_file_name, "r");
    time(&timer);
    strcpy(dte, asctime(localtime(&timer)));
    return file;
}

