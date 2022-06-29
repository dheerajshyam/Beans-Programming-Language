// Author: Venkata Subbu Dheeraj Shyam Polavarapu.

#pragma once

#ifndef BEANS_PARSER_HPP
#define BEANS_PARSER_HPP

#include "Token.hpp"
#include "beans_lexer.hpp"

extern YY_BUFFER_STATE
    yy_scan_string (const char * yystr );

extern int
    yylex(void);

extern Token* 
    lex(string filename);

struct GrammarTable
{
    string key;
    vector< vector<string> > values;

    vector<GrammarTable*> attachedNodes;
};

extern bool isAllLower(string value);

extern GrammarTable* baseNode;

extern map<string, vector<string> > FIRST_table;
extern map<string, vector<string> > FOLLOW_table;

extern vector<string>
    predictive_parsing_table[7][9];

enum TERMINAL_ID
{
    LPAREN,
    RPAREN,
    COMMA,
    IDENTIFIER,
    STRING,
    INTEGER,
    FLOAT,
    DOUBLE,
    EPSILON
};

enum NON_TERMINAL_ID
{
    FUNCTION_CALL,
    OBJECT,
    TYPE_OBJECT,
    TYPE_OBJECTS,
    PARAM,
    PARAMS,
    TUPLE
};

extern map<string,TERMINAL_ID> terminal_id_mapping;
extern map<string,NON_TERMINAL_ID> non_terminal_id_mapping;

struct StorageStack
{
    struct StorageStack* bottom;

    string value;

    struct StorageStack* top;
};

struct InputStack
{
    struct InputStack* bottom;

    vector<string> token;
    int lineno;
    int lexpos;

    struct InputStack* top;
};

struct OutputMappingStack
{
    struct OutputMappingStack* bottom;

    map<string, vector<string> > expansion;
    bool isFilled;

    struct OutputMappingStack* top;
};

void FIRST();
void FOLLOW();
void ParsingTable();

OutputMappingStack* parseTree();

#endif // BEANS_PARSER_HPP