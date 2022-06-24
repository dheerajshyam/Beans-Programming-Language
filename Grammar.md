#Author: Venkata Subbu Dheeraj Shyam Polavarapu.

function_call -> object LPAREN params RPAREN<br>
object -> typeobject | IDENTIFIER<br>
typeobject -> STRING | INTEGER | FLOAT | DOUBLE<br>
param -> object<br>
params -> param params | EPSILON<br>

<hr/>

tuple -> LPAREN RPAREN
