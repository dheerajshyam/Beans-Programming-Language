%{
  #include "Token.hpp"

  Token* head = new Token();
  Token* ptr = head;

  bool lparen_found = false;

%}

%s COMMENT

%%

[a-zA-Z_][a-zA-Z0-9_]* {
  
  ptr->value = yytext;
  ptr->TokenType = "IDENTIFIER";
  ptr->lineno = yylineno;
  ptr->count = 2;

  Token* link = new Token();
  
  ptr->link = link;
  ptr = link;

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

  ptr->TokenType = "INTEGER";
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

"***"((.|\n)*)"***" ;

\n {;}

[ |\t\b\f\r] {;}

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
