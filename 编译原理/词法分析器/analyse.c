/*
 * this function analyse word.
 *
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFERSIZE 1024
#define DIGIT 1
#define LETTER 2

typedef struct {
	int end;
	char str[BUFFERSIZE+1];
}buffer;

/* operation function */
int isDigit( char);	/* if it is digit, return 1 */
int isLetter( char);	/* if it is letter, return 2*/
int parseWord( char);
int belongToDigit( char);

void concat(char *, int *, char);
char get_char(char *, int *);
void analyse(char *);
int notLetterOrDigitOr_(char );
void clear_word( char *, int *);

void predeal(char *);
int readFromFile( buffer *, FILE *);
void loadToBuffer( buffer *, char *);

/* check is the key word*/
int isKeyWord(char *);

/* global value */
char c_Value;		/* read the char. */
char word[BUFFERSIZE];	/* get a word.    */

/* key word values*/
char key_word[][20]={
	"auto", "break", "case", "char", "const",
	"continue", "default", "do", "double", "else",
	"enum", "extern", "float", "for", "goto",
	"if", "inline", "int", "long", "register",
	"restrict", "return", "short", "signed", "sizeof",
	"static", "void", "volatile", "while", "_bool",
	"unsigned", "struct", "switch", "typedef", "union",
	"_Complex", "_Imaginary", "main", "\0"
};/* all is 37 word. */

int row;
int col;

int off_set;
int word_off_set;
int has_note;
int type;		/* tag a type. */

int main(int ac, char *av[])
{
	if( ac != 2)
	{
		fprintf(stderr, "by now, you only can input one parameter.\n");
		exit(1);
	}


	/* declare some value */
	FILE *of = NULL;
	buffer cache;
	char some_str[BUFFERSIZE+1];
	row = 1;
	col = 0;
	has_note = 0;

	/* init buffer */
	cache.end = 0;
	cache.str[0]='\0';

	/* open file */
	of = fopen(av[1], "r");
	if( !of){
		fprintf(stderr, "open file %s error!\n", "text.c");
		exit(1);
	}

	printf("----------------------start to analyse-----------------------------\n");

	/* read into buffer */
	while( readFromFile( &cache, of) > 0){
		printf("read the file:\n%s\n=============================================\n",cache.str);
	/* load buffer */
	printf(">>>>>>>> load buffer<<<<<<<<<<<\n");
	loadToBuffer( &cache, some_str );
	printf("load in data:\n%s\n=======================================================\n",some_str);

	/* analyse */
	printf(">>>>>>>> analyse <<<<<<<<<<<<<<<\n");
	analyse(some_str);
	printf(">>>>>>>> analyse phase..........\n");
	}

	printf("\n");
	return 0;
}

void loadToBuffer( buffer * cache, char *str)
/*
 * load the cache data into read buffer.
 *
 * */
{
	int finish_point = cache->end;
	int i=0;
	char *tmp_buffer;

	/* find the mostly long string */
	while(1)
	{
		if(cache->str[finish_point] == '}')
			break;
		if(cache->str[finish_point] == ';')
			break;
		finish_point--;
	}
	printf("buffer divide:---------finish_point: %d\n", finish_point);

	/* start to load */
	while( i <= finish_point ){
		str[i] = cache->str[i];
		i++;
	}/* a = finish_point + 1. */
	str[i] = '\0';

	/* reset the buffer */
	i = 0;
	tmp_buffer = (char *)malloc(sizeof(char) * BUFFERSIZE);
	finish_point++; /*finish_point = finish_point + 1. */

	while( cache->str[finish_point]){
		tmp_buffer[i] = cache->str[finish_point];
		i++;
		finish_point++;
	}

	tmp_buffer[i] = '\0';
	/* fill back to cache. */
	i = 0;
	while( tmp_buffer[i] ){
		cache->str[i] = tmp_buffer[i];
		i++;
	}
	cache->end = i;
}


int readFromFile( buffer *cache, FILE *rf)
/*
 * read the file and fill into the cache.
 * */
{
	int start = cache->end;
	int char_number = BUFFERSIZE - start;
	char *sptr = (char *)malloc(sizeof(char) * (char_number+1));
	int len;
	int i = 0;

	printf("start: %d\n", start);

	/* read data base on the number of chars. */
	if( (len = fread(sptr, sizeof(char), char_number, rf)) <= 0){
		printf("\nlen: %d\n", len);
		if( len == 0){
			printf("at the end of file\n");
			return 0;
		}
		else
		{
			fprintf(stderr, "something wrong !\n");
			exit(1);
		}
	}

	for( i; i<len; i++){
		cache->str[start++] = sptr[i];
	}
	cache->str[start] = '\0';

	cache->end = start;

	/* free */
	free(sptr);

	return len;
}

int stringIsEqualTo(char *compareString, char *withString)
/* a simple version of strncmp 
 *
 * if not equal return 0
 *
 * or return 1.
 *
 * */
{
	int len = strlen(withString);
	int c,w;

	/* if length is not equal return false */
	if( len != strlen(compareString))
		return 0;

	/* compare */
	for(c = w = 0; w < len; w++, c++)
		if( compareString[c] != withString[w] )
			return 0;
	return 1;
}

int isKeyWord(char * str)
/*
 * check str is key word 
 *
 * and return 1
 *
 * or return 0.
 * */
{
	int i = 0;
	
	while( key_word[i][0] ){
		if( stringIsEqualTo(str, key_word[i]) ){
				return 1;
		}
		i++;
	}

	return 0;
}

void clear_word( char *str, int * off_set)
/*
 * clear the word and print the word and type.
 *
 * and set off_set = 0.
 * */
{
	char str_tmp[256];

	str[*off_set] = '\0';

	/* check the key word */
	if( isKeyWord(str) )
		type = 1;
	/* prints the word */
	/*else
		printf("type: %d, word: %s\n", type, str);
	*/
	/* assign off_set = 0 */
	if(has_note)
		;
	else{
	printf("(%d,\'%s\')\n", type, str);
	}
	*off_set = 0;
}

int notLetterOrDigitOr_( char c)
/*
 * check the c is letter or digit or _
 *
 * and return 1.
 *
 * or will return 0
 * */
{
	if( isLetter(c) == LETTER || isDigit(c) == DIGIT || c == '_')
		return 0;
	else
		return 1;
}

char get_char( char *str, int * off_set)
/* get a char from str base on the off_set 
 * */
{

	char c = str[*off_set];

	(*off_set)++;
	col++;

	return c;
}

void concat( char * word, int * off_set, char c)
/*
 * concat word with c, in off_set.
 */
{
	if(*off_set < 0 )
		return ;
	word[*off_set] = c;
	(*off_set)++;
}

int parseWord( char c)
/* parse a char */
{

	if( isLetter(c) == 2){
		return LETTER;
	}
	if( isDigit(c) == 1){
		return DIGIT;
	}
	else {
		return c;
	}
}
int belongToDigit( char c)
/*
 * parse the c is digit or dot,
 *
 * and return 1
 *
 * or return 0.
 * */
{
	if( isDigit(c) == DIGIT || c == '.' )
		return 1;
	else
		return 0;
}

int isDigit( char c)
/*
 * check c is digit or not.
 *
 * if it is digit return 1.
 *
 * or return 0
 * */
{
	if( c >= '0' && c <= '9')
		return 1;
	else 
		return 0;
}

int isLetter( char c)
/* 
 * check c is letter or not.
 *
 * if is letter return 2,
 *
 * if not, return c.
 * */
{
	if( ( c >= 'a' && c <= 'z') || ( c >= 'A' && c <= 'Z'))
		return 2;
	else
		return 0;
}

void analyse( char * str)
/*
 * analyse str, get some word and classification.
 * */
{	
	off_set = 0;	/* the all string off set. */
	word_off_set = 0; /* the buffer string off set. */

	while( ( c_Value = get_char(str, &off_set)) != '\0' )
	{
		switch( parseWord(c_Value) )
		{

			case '_':
			case LETTER:{

				type = 2;	/* set the type */
				concat(word, &word_off_set, c_Value);/* add a letter */
				while(1){
					c_Value = get_char(str, &off_set); /* get char. */

					if( notLetterOrDigitOr_( c_Value)){ /* check the char is letter or digit or _ */
						off_set -= 1;
						col--;

						clear_word(word, &word_off_set); /* clear word buffer. */
						break;
					}

					concat( word, &word_off_set, c_Value);/* concat char. */
				}
			}
			break;
			/* word -  type 2. */
			case DIGIT:{
				type = 3;
				concat(word, &word_off_set, c_Value); /* add a digit to word*/
				while(1){
					c_Value = get_char(str, &off_set);

					if( !belongToDigit(c_Value)){ /* check the c_Value is belong to digit. */
						off_set -= 1;
						col--;
						clear_word(word, &word_off_set);/* clear buffer. */
						break;
					}

					concat( word, &word_off_set, c_Value);/* concat char. */
				}
			}
			break;
			/* digit - type 3 */
			case ',':
			case '.':
			case '[':
			case ']':
			case ':':
			case ';':
			case '{':
			case '}':
			case '(':
			case ')':{
				type = 5;
				concat(word, &word_off_set, c_Value); /* add a divide letter to buffer */
				clear_word(word, &word_off_set);      /* clear buffer */
			}
			break;
			/* type 5 -- operation notation. */
			case '+': {
				type = 4;
				concat(word, &word_off_set, c_Value);
				switch(c_Value = get_char(str, &off_set)){
					case '+':
					case '=': {
						concat(word, &word_off_set, c_Value);
						clear_word(word, &word_off_set);
					}
					break;
					default: {
						clear_word(word, &word_off_set);
						off_set -= 1;
						col--;
					}
						break;
				}
			}
			break;

			case '-': {
				type = 4;
				concat(word, &word_off_set, c_Value);
				switch( c_Value = get_char(str, &off_set)){
					case '-':
					case '=': {
						concat(word, &word_off_set, c_Value);
						clear_word(word, &word_off_set);
					}
					break;
					default:{
						clear_word(word, &word_off_set);
						off_set -= 1;
						col--;
					}
				}
			}
			break;

			case '*':
	    		{
				type = 4;
				concat(word, &word_off_set, c_Value);
				switch(c_Value = get_char(str, &off_set)){
					case '=':
					case '/': {
					concat(word, &word_off_set, c_Value);
					clear_word(word, &word_off_set);
					has_note = 0;
					}
					break;

					default: {
					clear_word(word, &word_off_set);
					off_set -= 1;
					col--;
					}
					break;
				}
			}
			break;

			case '/':{
				type = 4;
				concat(word, &word_off_set, c_Value);
				switch(c_Value = get_char(str, &off_set)){
					case '=':
					case '*': {
					has_note = 1;
					concat(word, &word_off_set, c_Value);
					clear_word(word, &word_off_set);
					}
					break;

					default: {
					clear_word(word, &word_off_set);
					off_set -= 1;
					col--;
					}
					break;
				}
			}
		        break;

			case '=': 
			case '!':
			case '~':
			case '^':
			{
				type = 4;
				concat(word, &word_off_set, c_Value);
				if( (c_Value = get_char(str, &off_set)) == '='){
					concat(word, &word_off_set, c_Value);
					clear_word(word, &word_off_set);
				}
				else {
					clear_word(word, &word_off_set);
					off_set -= 1;
					col--;
				}
			}
			break;

			case '>':
			{
				type = 4;
				concat(word, &word_off_set, c_Value);
				switch( c_Value = get_char(str, &off_set))
				{
					case '=':{
						concat(word, &word_off_set, c_Value);
						clear_word(word, &word_off_set);
					}
					break;
					case '>':{
						concat(word, &word_off_set, c_Value);
						if( (c_Value = get_char(str, &off_set)) == '='){
							concat(word, &word_off_set, c_Value);
							clear_word(word, &word_off_set);
						}
						else{
							clear_word(word, &word_off_set);
							off_set -= 1;
							col--;
						}
					}
					break;
					default:
					{
						clear_word(word, &word_off_set);
						off_set -= 1;
						col--;
					}
				}
			}
			break;

			case '<': {
				type = 4;
				concat(word, &word_off_set, c_Value);
				switch( c_Value = get_char(str, &off_set))
				{
					case '=':{
						concat(word, &word_off_set, c_Value);
						clear_word(word, &word_off_set);
					}
					break;
					case '<':{
						concat(word, &word_off_set, c_Value);
						if( (c_Value = get_char(str, &off_set)) == '='){
							concat(word, &word_off_set, c_Value);
							clear_word(word, &word_off_set);
						}
						else{
							clear_word(word, &word_off_set);
							off_set -= 1;
							col--;
						}
					}
					break;
					default:
					{
						clear_word(word, &word_off_set);
						off_set -= 1;
						col--;
					}
				}
			}
			break;
			
			case '&':
			case '|':{
				type = 4;

				concat(word, &word_off_set, c_Value);

				switch(c_Value = get_char(str, &off_set)){
					case '=':
					case '|':{
						concat(word, &word_off_set, c_Value);
						clear_word(word, &word_off_set);
					}
					break;

					default:{
						clear_word(word, &word_off_set);
						off_set -= 1;
						col--;
					}
				}
			}
			break;

			case '#':{
				type = 0;
				concat(word, &word_off_set, c_Value);/* add a letter */

				while(1){
					c_Value = get_char(str, &off_set); /* get char. */

					if( ! isLetter( c_Value)){ /* check the char is letter or digit or _ */
						off_set -= 1;
						col--;

						clear_word(word, &word_off_set); /* clear word buffer. */
						break;
					}

					concat( word, &word_off_set, c_Value);/* concat char. */
				}

			}
			break;

			case '\'':{
				type = 6;
				concat(word, &word_off_set, c_Value);/* add to word. */
				c_Value = get_char(str, &off_set);/* get the next */

				if(c_Value == '\\'){/* if it is \ concat and get the next */
					concat(word, &word_off_set, c_Value);
					c_Value = get_char( str, &off_set);
				}/* or concat it and get the next */
				concat(word, &word_off_set, c_Value);

				c_Value = get_char( str, &off_set);/* check c_Value is ' or return error */

				/* end */
				if( c_Value == '\''){
						concat(word, &word_off_set, c_Value);
						clear_word(word, &word_off_set);
						break;
				}
				else{
					if( ! has_note){
						fprintf(stderr, " %c is char  , not string! in %d,%d\n", c_Value, row, col);
						exit(1);
					}
				}
				
			}	
			break;

			case '\"':{
				type = 6;
				concat(word, &word_off_set, c_Value);

				while(1){
					c_Value = get_char(str, &off_set);

					if( c_Value == '\"'){
						concat(word, &word_off_set, c_Value);
						clear_word(word, &word_off_set);
						break;
					}
					concat( word, &word_off_set, c_Value);
				}
			}	
			break;

			case ' ':
			case '\t':
			break;
			case '\n': {
				row++;
				col = 0;
			}
			break;

			default:
				if(! has_note){
					fprintf(stderr, "wrong start \'%c\' at <%d,%d> ...\n", c_Value, row, col - 1);
					exit(1);
				}
				break;
		}
	}
}

	

