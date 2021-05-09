# Introduction
- ## **Key words**
lvalue, rvalue, lvalue reference, rvalue reference
- ## **lvalue**
For an lvalue is something that has a specific memory location, we can get the address of it. Lvalue must have an container.
- ## **rvalue**
For an rvalue, it different from lvalue. rvalue is a temporary object which do not have a name. The return value of a function and the value of an expression are rvalues. We can understand lvalue and rvalue as lvalue is a container but rvalue is the value in the container. We can get the specific address of lvalue, but we can not get the address of a rvalue. rvalue always has a short lifetime.
```C++
int x = 10;
int y = x;
```
Lvalue can be used as an rvalue. For the above example, x, y are lvalues. Assign the value of x to the lvalue y, in this way x is used as an rvalue. But in contrast, an rvalue can not be converted to an lvalue. As an rvalue is a temporary object, and it do not have a specific location in memory but lvalue has.
- ## **lvalue reference**
In short, lvalue reference is a reference that binds to an lvalue which marked with one '&' symbol. It has to be initialized to a valid object or function when creating it. But keep in mind the object or function must be an lvalue.
```C++
int &lref = 10;
```
Lvalue can not bind to an rvalue, the above code will an error. There are two methods to deal with the case we want to bind rvalue to an lvalue reference.
```C++
int x = 10;
int &lref = x;
```
```C++
const int &lref = 10;
```
The first method is we create an intermediate variable to obtain a container. The second one, the C++ compiler allow us to define a constant lvalue reference point to an rvalue. The mechanism behind the definition is similar as below:
```C++
int tmp = 10;
const int &lref = tmp;
```
- ## **rvalue reference**
rvalue reference is a reference that binds t an rvalue. It marked with symbol '&&'.
It has to be initialized when creating it.
- ## **move**
