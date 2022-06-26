**Author: Venkata Subbu Dheeraj Shyam Polavarapu.**

- \*\*\*  This is a comment/multiline comment. \*\*\*

<hr/>

- 
  function_call -> object LPAREN params RPAREN<br>
  object -> typeobject | IDENTIFIER<br>
  typeobject -> STRING | INTEGER | FLOAT | DOUBLE<br>
  param -> object<br>
  params -> param params | EPSILON<br>

<hr/>

- 
  variable_declaration: DOLLAR IDENTIFIER variable_decl_rhs<br>
  variable_decl_rhs: EQUALS_TO values variable_decl_rhs | ISTO type COLON<br>
  value: object | function_call | list | dictionary<br>
  values: value COMMA values<br>

<hr/>

-
  list: LBRACE list_values RBRACE<br>
  list_value: object | function_call | list<br>
  list_values: list_value list_values<br>

<hr/>

-
  dictionary: OPAREN dictionary_mappings CPAREN<br>
  dictionary_mappings: key:value dictionary_mappings<br>
