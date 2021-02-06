@mainpage Manuel Linear Algebra Solver  
**Members** : Valentin Koeltgen  
**License** : MIT  

This project goal is to create a program with a simple command-line interface to do calculations on polynomial and matrices.  
Right now the project is limited to handling real values but imaginary values handling can be added in the future.

The program should be able to handle advanced maths in the form of simple user commands or script files containing commands.  
It should be able to handle all kind of objects, like polynomials, matrices, vectors (matrices with 1 column) or simple values (not yet implemented).

# Command List  
This is a list of commands that are available to the user

## Simple commands  
The following commands don't need any arguments, they are used to do action related to the user

`help` This command display this page in the terminal  
`displayAll` This command display the whole content of the main register  
`clear` This command empty the main register  
`readScript(<link>)` This command apply the content of a script located at \<link>, it reads it line by line and apply every command

## Simple operations
The following commands are final but accept composite operations as argument

`display(<operation>)` This command display the result of \<operation> in the terminal  
`eig(<operation>)` This command display the eigen values of \<operation>, \<operation> must be a matrix  
`trace(<operation>)` This command display the trace of \<operation>, \<operation> must be a matrix  
`det(<operation>)` This command display the determinant of \<operation>, \<operation> must be a matrix  
`solve(<operation>)` This command display the result of solving \<operation>,
- if \<operation> is an augmented matrix, the result will be the matrix in echelon form  
- if \<operation> is a polynomial, the result will be the roots of the polynomial  

`<objectName> = <operation>` This command create a new object in the register which contain the result of \<operation> and has for name \<objectName>   

## Composite operations
The following commands are not final, they can be used recursively

`<operation1> + <operation2>` This command return the sum of \<operation1> and \<operation2> if they are of the same type   
`<operation1> - <operation2>` This command return the difference of \<operation1> and \<operation2> if they are of the same type  
`<operation1> * <operation2>` This command return the product of \<operation1> and \<operation2> if they are of the same type  
`trans(<operation>)` This command return the transposed of \<operation>, \<operation> must be a matrix

## Basic object
The following arguments can be used as fundamentals to create or call objects

`<objectName>` This will refer to the object with the same name, it must have been created beforehand. An object name can take any ASCII character but can only be the size of one word, no spaces accepted  
`[<matrixFormat>]` This will create a matrix object. The format used to create a matrix is the following :
- all values in a row are separated by a `,`
- all rows are separated by a `;`, the last one doesn't need it

_Example :_ `[1,2,3;4,5,6;7,8,9]` is a correct 3x3 matrix, `[1,2;3,4,5]` is not correct since the 2nd row has more values than the first  

`<polynomialFormat>` This will create a polynomial object. The format used to create a polynomial is the following :
- to write coefficient, one must use the format `<coefficient>X^<power>` with \<coefficient> any real value and \<power> an integer
    - if \<power> is 1, the coefficient can be written as `<coefficient>X`
    - if \<power> is 0, the coefficient can be written as `<coefficient>`
- every coefficient must have an explicit value, for example `X^3` is not accepted but `1X^3` is.  

_Example :_ `2X + 1X^2 * (2X + 2)` is correct an will create the polynomial `2X^3 + 2X^2 + 2X + 0`, `X + 2X^1.5` is not correct because the first X doesn't have a coefficient and the power of the second is not an integer