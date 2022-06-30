%{
  // Author: Venkata Subbu Dheeraj Shyam Polavarapu.

  #include "Token.hpp"

  Token* head = new Token();
  Token* ptr = head;

  bool lparen_found = false;

  int lexpos;

%}

%option outfile="beans_lexer.cpp" header-file="beans_lexer.hpp"
%option yylineno

%%

[a-zA-Z_][a-zA-Z0-9_]* {

  ptr->value = yytext;
  ptr->TokenType = "IDENTIFIER";
  ptr->lineno = yylineno;
  
  ptr->count = 2;

  Token* link = new Token();
  
  ptr->link = link;
  ptr = link;

  lexpos += yyleng;

}

"\"" {
  
  string str;
  char c = yyinput();

  while(c != '"')
  {
      if(c == '\0')
      {
        cout << "Unexpected EOF encountered while constructing string literal." << endl;
        str.clear();
        exit(0);
      }

      str += c;
      c = yyinput();
  }
  
  ptr->value = str;
  ptr->TokenType = "STRING";
  ptr->lineno = yylineno;

  ptr->count = 2;

  Token* link = new Token();

  ptr->link = link;
  ptr = link;
  
  str.clear();
}

[0-9]* {

  ptr->value = yytext;
  ptr->TokenType = "INTEGER";
  ptr->lineno = yylineno;
  
  ptr->count = 1;

  Token* link = new Token();

  ptr->link = link;
  ptr = link;
}

[0-9]*\.[0-9]* {

  ptr->value = yytext;
  ptr->TokenType = "DOUBLE";
  ptr->lineno = yylineno;
  
  ptr->count = 1;

  Token* link = new Token();

  ptr->link = link;
  ptr = link;
}

-[0-9]*\.[0-9]* {

  ptr->value = yytext;
  ptr->TokenType = "FLOAT";
  ptr->lineno = yylineno;
  
  ptr->count = 1;

  Token* link = new Token();

  ptr->link = link;
  ptr = link;
}

\( {

  lparen_found = true;

  ptr->TokenType = "LPAREN";
  ptr->lineno = yylineno;
  
  ptr->count = 1;

  Token* link = new Token();

  ptr->link = link;
  ptr = link;
}

\) {

  if(lparen_found)
  {

    lparen_found = false;

    ptr->TokenType = "RPAREN";
    ptr->lineno = yylineno;
    
    ptr->count = 1;

    Token* link = new Token();

    ptr->link = link;
    ptr = link;
  }
  else
  {
    cout << "Unmatched ')' found in line: " << yylineno << endl;
    exit(0);
  }

}

\, {
  
  ptr->TokenType = "COMMA";
  ptr->lineno = yylineno;
  
  ptr->count = 1;

  Token* link = new Token();

  ptr->link = link;
  ptr = link;
}

\$ {

  ptr->TokenType = "DOLLAR";
  ptr->lineno = yylineno;
  
  ptr->count = 1;

  Token* link = new Token();

  ptr->link = link;
  ptr = link;
}

\; {

  ptr->TokenType = "COLON";
  ptr->lineno = yylineno;
  
  ptr->count = 1;

  Token* link = new Token();

  ptr->link = link;
  ptr = link;
}

\n ;

[ |\t\b\f\r] {lexpos += yyleng;}

. {;}

%%

int yywrap() { return 1; }

Token* lex(string filename)
{
    yyin = fopen( (char*) filename.c_str(), "r" );
    if(yyin == NULL)
      return NULL;
    yylex();
    fclose(yyin);

    return head;
}
