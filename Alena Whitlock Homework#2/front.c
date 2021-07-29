
#include <stdio.h>
#include <ctype.h>


int charClass;
//this is to detect floating point literals
int prevClass;
char lexeme [100];
char nextChar;
char nextCharPlus;
int lexLen;
int token;
int nextToken;

int prevToken = 0;
FILE *in_fp, *fopen();

/* Function declarations */
void addChar();
void getChar();
void getNonBlank();
int lex();

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define COMMENT 2
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26


main() {
	
	if ((in_fp = fopen("front.in", "r")) == NULL)
	printf("ERROR - cannot open front.in \n");
	else {
		getChar();
		do {
			lex();
		} while (nextToken != EOF);
	}
}

int lookup(char ch) {
	prevToken = nextToken;
	switch (ch) {
		case '(':
			addChar();
			nextToken = LEFT_PAREN;
			break;
		case ')':
			addChar();
			nextToken = RIGHT_PAREN;
			break;
		case '+':
			addChar();
			nextToken = ADD_OP;
			break;
		case '-':
			addChar();
			nextToken = SUB_OP;
			break;
		case '*':
			addChar();
			nextToken = MULT_OP;
			break;
		case '/':
			addChar();
			nextToken = DIV_OP;
			break;
		default:
			addChar();
			nextToken = EOF;
			break;
	}
	return nextToken;
}


void addChar() {
	if (lexLen <= 98) {
		lexeme[lexLen++] = nextChar;
		lexeme[lexLen] = 0;
	}
	else
	printf("Error - lexeme is too long \n");
}


void getChar() {
	if ((nextChar = getc(in_fp)) != EOF) {
		if (isalpha(nextChar))
			charClass = LETTER;
		else if (isdigit(nextChar))
			charClass = DIGIT;
		else charClass = UNKNOWN;
	}
	else
	charClass = EOF;
}


void getNonBlank() {
	while (isspace(nextChar))
		getChar();
}


int lex() {
	lexLen = 0;
	getNonBlank();
	switch (charClass) {
		/* Parse identifiers */
		case LETTER:
			addChar();
			getChar();
			while (charClass == LETTER || charClass == DIGIT) {
				addChar();
				prevClass = charClass;
				getChar();
			}
			nextToken = IDENT;
			break;
		/* Parse integer literals */
		case DIGIT:
			addChar();
			getChar();
			while (charClass == DIGIT) {
				addChar();
				getChar();
			}
			nextToken = INT_LIT;
			break;
		/* Parentheses and operators */
		case UNKNOWN:
			lookup(nextChar);
			getChar();
			if (prevToken == 24 && nextToken == 23) {
					addChar();
					getChar();
				}
			else if (prevToken == 23 && nextToken == 24) {
					addChar();
					getChar();
				}			
			break;
		
		case EOF:
			nextToken = EOF;
			lexeme[0] = 'E';
			lexeme[1] = 'O';
			lexeme[2] = 'F';
			lexeme[3] = 0;
			break;
	} /* End of switch */
	if (prevToken == 24 && nextToken == 23)
		{printf("Comment detected! \n");}
	else if (prevToken == 23 && nextToken == 24)
		{printf("End of Comment detected! \n");}
	else if (prevClass == DIGIT && (charClass == LETTER || nextToken == SUB_OP))
		{printf("Floating point detected! \n");}
	printf("Next token is: %d, Next lexeme is %s\n",
	nextToken, lexeme);
	
	return nextToken;
} 
