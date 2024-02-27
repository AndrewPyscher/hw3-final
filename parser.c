// **********************************************
// Program1: C Parser
// Course: CS451
// Student Name: Andrew Pyscher
// Instructor: Dr. Cho
// Date of submission: 3/17/23
// Program Description: a lexical analyzer that has been expanded to support floating point numbers, 
// along with if, while, for, and else tokens, and other operators and symbols.
// it follows the grammar stated in the hw3-parser.docx
// **********************************************



// o [X] My program compiles without error on the csis machine.
// o [X] The lexical analyzer successfully recognizes floating point number.
// o [X] The lexical analyzer successfully recognizes all the new key words if, while, and else.
// o [X] The lexical analyzer successfully recognizes the comparison operators as new tokens.
// o [X] My program prints the proper messages for newly added tokens and keywords
// o [X] My program implements functions for each non-terminals
// o [X] I prepared for a typescript file (hw3-lastname.txt) that includes the compilation,
// source code listing, and the run result for all four input files.



/* parser.c - a lexical analyzer system for simple arithmetic expressions */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
int charClass;
char lexeme [100];
char nextChar;
int lexLen;
int token;
int nextToken;
FILE *in_fp, *fopen();

void addChar();
void getChar();
void getNonBlank();
int lex();
void expr();
void term();
void factor();
// new functions
void stmts();
void stmt();
void ifstmt();
void whilestmt();
void forstmt();
void asgnstmt();
void boolexpr();
void comp();
void end();

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10
#define IDENT 11

//added tokens
#define FLOAT_CONST 12
#define DECIMAL_POINT 13
#define GREATER_THAN 14
#define LESS_THAN 15
#define SEMICOLON 16
#define LEFT_CURLY 17
#define RIGHT_CURLY 18
#define IF_STMT 30
#define ELSE_STMT 31
#define WHILE_STMT 32
#define FOR_LOOP 33

#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26

/******************************************************/
/* main driver */
int main() {
    // array to store the input file names
    char input[5][15] = {"in1.txt", "in2.txt", "in3.txt", "in4.txt", "in5.txt"};
    int i = 0;
    /* Open the input data file and process its contents */
    // runs each file
    for(i=0; i<5; i++){
        printf("=================%s================\n", input[i]);
    if ((in_fp = fopen(input[i], "r")) == NULL)
    printf("ERROR - cannot open front.in \n");
    else {
        getChar();
        do {
            // start the code using the grammar
            lex();
            stmts();
        } while (nextToken != EOF);
    }
    }
    return 0;
}

/*****************************************************/
/* lookup - a function to lookup operators and parentheses
 * and return the token */
// added support for ., >, <. {, }
int lookup(char ch) {
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
        case '.':
            addChar();
            nextToken = DECIMAL_POINT;
            break;
        case '>':
            addChar();
            nextToken = GREATER_THAN;
            break;
        case '<':
            addChar();
            nextToken = LESS_THAN;
            break;
        case '{':
            addChar();
            nextToken = LEFT_CURLY;
            break;
        case '}':
            addChar();
            nextToken = RIGHT_CURLY;
            break;
        case '=':
            addChar();
            nextToken = ASSIGN_OP;
            break;
        case ';':
            addChar();
            nextToken = SEMICOLON;
            break;
        
        default:
            addChar();
        nextToken = EOF;
        break;
    }
    return nextToken;
}

//changed nothing
/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    }
    else
    printf("Error - lexeme is too long \n");
}
// changed nothing
/*****************************************************/
/* getChar - a function to get the next character of
 * input and determine its character class */
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
// changed nothing
/*****************************************************/
/* getNonBlank - a function to call getChar until it
 * returns a non-whitespace character */
void getNonBlank() {
    while (isspace(nextChar))
    getChar();
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic expressions */
// added support for floating point numbers, and If, while, for and else tokens
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
                getChar();
            }
        // compare the current characters in the lexeme arr to see if they match any keywords
        // if not, it it is an IDENT
        if(!strcmp(lexeme, "if")){
            nextToken = IF_STMT;
        }else if(!strcmp(lexeme, "while")){
            nextToken = WHILE_STMT;
        }else if(!strcmp(lexeme, "for")){
            nextToken = FOR_LOOP;
        }else if(!strcmp(lexeme, "else")){
            nextToken = ELSE_STMT;
        }else{
            nextToken = IDENT;
        }
        break;
     
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            // check if the next character is a dot, if it is then its a FLOAT_CONST
            if(charClass == UNKNOWN){
                // had to do this because i had a hard time separating ints and )
                if(strcmp(&nextChar, ".")== 0){
                    getChar();
                while (charClass == DIGIT) {
                    addChar();
                    getChar();
                }   
            nextToken = FLOAT_CONST;
            break;
        }
    }   // if theres no decimal, then its a normal INT
        /* Parse integer literals */
            nextToken = INT_LIT;
            break;
        /* Parentheses and operators */
        case UNKNOWN:
       
        if(lookup(nextChar) == DECIMAL_POINT){
                getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }   
            nextToken = FLOAT_CONST;
            break;
        }
    
            getChar();
            break;
        /* EOF */
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
        break;
    } /* End of switch */

    printf("Next token is: %d, Next lexeme is %s \n",
    nextToken, lexeme);
    return nextToken;
} /* End of function lex */

/* Parses strings in the language generated by the rule:
 *    <expr> -> <term> {(+ | -) <term>} */
void expr() {
    printf("Enter <expr>\n");
    /* Parse the first term */
    term();
    /* As long as the next token is + or -, get
 *     the next token and parse the next term */
    while (nextToken == ADD_OP || nextToken == SUB_OP) {
        lex();
        term();
    }
    printf("Exit <expr>\n");
}

/* Parses strings in the language generated by the rule:
 *    <term> -> <factor> {(* | /) <factor>) */
void term() {
    printf("Enter <term>\n");
    /* Parse the first factor */
    factor();
    /* As long as the next token is * or /, get the
 *     next token and parse the next factor */
    while (nextToken == MULT_OP || nextToken == DIV_OP) {
        lex();
        factor();
    }
    printf("Exit <term>\n");
}

/* Parses strings in the language generated by the rule:
 *     <factor> -> id | int_constant | ( <expr> ) */
void factor() {
    printf("Enter <factor>\n");
    /* Determine which RHS */
    if (nextToken == IDENT || nextToken == INT_LIT || nextToken == FLOAT_CONST) {
        printf("ID or LIteral\n");
        /* Get the next token */
        lex();
    }
    /* If the RHS is ( <expr>), call lex to pass over the left parenthesis,
 *        call expr, and check for the right parenthesis */
    else {
        if (nextToken == LEFT_PAREN) {
            lex();
            expr();
            if (nextToken == RIGHT_PAREN)
                lex();
            else
                end();
        } /* End of if (nextToken == ... */
        /* It was not an id, an integer literal, or a left parenthesis */
        else
           end();
    } /* End of else */
    printf("Exit <factor>\n");
}

void end(){
    printf("Error");
    exit(1);
}
//Parses strings in the language generated by the rule:
//<stmts> → <stmt> {; <stmt> }
void stmts(){
    printf("Enter <stmts>\n");
    stmt();
    // if semicolon, next statement
    if (nextToken == SEMICOLON){
        lex();
        stmt();
    }

    printf("Exit <stmts>\n");
}

//Parses strings in the language generated by the rule:
// <stmt> → <ifstmt> | <whilestmt> | <forstmt> | <asgnstmt>
// check if the stmt is one of the new tokens, if not its an identifier
void stmt(){
    printf("Enter <stmt>\n");
    
    if(nextToken == IF_STMT){
        lex();
        ifstmt();
    }
    else if(nextToken == FOR_LOOP){
        lex();
        forstmt();    
    }
    else if(nextToken == WHILE_STMT){
        lex();
        whilestmt();
    }
    else if(nextToken == IDENT){
        lex();
        asgnstmt();
    }
        
    else{
        printf("error\n");
    }
    printf("Exit <stmt>\n");
}
//Parses strings in the language generated by the rule:
//<ifstmt> → if (<boolexpr>) ‘{‘ <stmts> ‘}’ [ else ‘{‘ <stmts> ‘}’ ]
void ifstmt(){
    printf("Enter <ifstmt>\n");
    // check if the first character is a (
    if(nextToken != LEFT_PAREN)
        end();

    lex();
    boolexpr();
    lex();
    // check if the character after boolexpr() is a )
    if(nextToken != RIGHT_PAREN)
        end();
    lex();
    // check if the next is a {
    if(nextToken != LEFT_CURLY)
        end();
    lex();
    stmts();
// check if the character after stmts is a }
    if(nextToken != RIGHT_CURLY)
        end();
    lex();
    // check if theres an else
    if(nextToken == ELSE_STMT){
        printf("enter <else>\n");
        lex();
        // check if theres a {
        if(nextToken != LEFT_CURLY)
            end();
        lex();
        stmts();
        // check if theres a } after the stmts()
        if(nextToken != RIGHT_CURLY)
            end();
        // get an error if this isnt here
        lex();
    
    printf("exit <else>\n");
    }

    printf("exit <ifstmt>\n");
}

//Parses strings in the language generated by the rule:
//<whilestmt> → while (<boolexpr>) ‘{‘ <stmts> ‘}’
void whilestmt(){
    printf("Enter <whilestmt>\n");
        // check if the first character is a (
    if(nextToken != LEFT_PAREN)
        end();
    
    lex();
    boolexpr();
    lex();
    // check if the character after boolexpr() is a )
    if(nextToken != RIGHT_PAREN)
        end();
    
    lex();
    // check if the next is a {
    if(nextToken != LEFT_CURLY)
        end();
    
    lex();
    stmts();
        // check if theres a } after the stmts()
    if(nextToken != RIGHT_CURLY)
        end();

    lex();
    printf("Exit <whilestmt>\n");
}
//Parses strings in the language generated by the rule:
//<forstmt> → for (<asgnstmt>; <boolexpr>; <asgnstmt>) ‘{‘ <stmts> ‘}’
void forstmt(){
    printf("Enter <forstmt>\n");
    // check if the first character is a (
    if(nextToken != LEFT_PAREN)
        end();
    
    lex();
    lex();
    asgnstmt();
    // check if theres a ; after asgnstmt()
    if(nextToken != SEMICOLON)
        end();
    
    lex();
    boolexpr();
    lex();
    // check if theres a ; after boolexpr()
    if(nextToken != SEMICOLON)
        end();
    
    lex();
    lex();
    asgnstmt();
    // check if the character after asgnstmt() is a )
    if(nextToken != RIGHT_PAREN)
        end();
    
    lex();
    // check if theres a {
    if(nextToken != LEFT_CURLY)
        end();
    
    lex();
    stmts();
        // check if theres a } after stmts()
    if(nextToken != RIGHT_CURLY)
        end();
    
    lex();
    printf("Exit <forstmt>\n");
}
//Parses strings in the language generated by the rule:
//<asgnstmt> → id = <expr>
void asgnstmt(){
    printf("Enter <asgnstmt>\n");
    // check if theres an =
    if(nextToken != ASSIGN_OP)
        end();
    
    lex();
    expr();

    printf("Exit <asgnstmt>\n");
}
//Parses strings in the language generated by the rule:
//<booexpr> → id <comp> int_constant
void boolexpr(){
    printf("enter <boolexpr>\n");
    // check if there first token is an identifer
    if(nextToken != IDENT)
        end();
    lex();
    comp();
    lex();
    printf("exit <boolexpr>\n");
}
//Parses strings in the language generated by the rule:
//<comp> → < | >
void comp(){
    printf("enter <comp>\n");
    // check if the token is either < or >
    if(nextToken != LESS_THAN && nextToken != GREATER_THAN)
        end();

    printf("exit <comp>\n");
}
   