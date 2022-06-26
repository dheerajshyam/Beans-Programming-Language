**Author: Venkata Subbu Dheeraj Shyam Polavarapu.**

- \*\*\*  This is a comment/multiline comment. \*\*\*

<hr/>

- 
  function_call: IDENTIFIER LPAREN params RPAREN<br>
  object: typeobjects | IDENTIFIER<br>
  typeobject: STRING | INTEGER | FLOAT | DOUBLE<br>
  typeobjects: typeobject typeobjects | EPSILON<br>
  param: object<br>
  params: param COMMA params | EPSILON<br>

<hr/>

- 
  variable_declaration: DOLLAR IDENTIFIER variable_decl_rhs<br>
  variable_decl_rhs: EQUALS_TO values | ISTO type COLON<br>
  value: object | function_call | list | dictionary<br>
  values: value COMMA values | EPSILON<br>

<hr/>

-
  list: LBRACE list_values RBRACE<br>
  list_value: object | function_call | list<br>
  list_values: list_value list_values | EPSILON<br>
