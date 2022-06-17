%{
  #include "Token.hpp"

  int lineno = 1;

  Token* head = new Token();
  Token* ptr = head;

%}

%%

[a-zA-Z_][a-zA-Z0-9_]* {
  
  ptr->value = yytext;
  ptr->TokenType = "IDENTIFIER";
  ptr->lineno = lineno;
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
        cout << "Unexpected EOF while parsing string literal" << endl;
        str.clear();
        return 0;
      }

      str += c;
      c = yyinput();
  }

  ptr->value = str;
  ptr->TokenType = "STRING";
  ptr->lineno = lineno;
  ptr->count = 2;

  Token* link = new Token();

  ptr->link = link;
  ptr = link;
  
  str.clear();
}

\( {
  
  ptr->TokenType = "LPAREN";
  ptr->lineno = lineno;
  ptr->count = 1;

  Token* link = new Token();

  ptr->link = link;
  ptr = link;

}

\) {
  
  ptr->TokenType = "RPAREN";
  ptr->lineno = lineno;
  ptr->count = 1;

  Token* link = new Token();

  ptr->link = link;
  ptr = link;

}

\n {lineno++;}

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
