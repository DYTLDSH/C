# Introduction
- ## **Key words**
lvalue, rvalue, lvalue reference, rvalue reference, universal reference, reference collapsing.
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
- ## **universal reference**
There are two necessary conditions for universal reference:
1. T&&;
2. Type deduction of T is must.

Determine whether it is universal reference, after knowing the form of the expression is 'T&&', another important thing is to make sure that the type of T hasn't known until it gets the deduction.  
There are four types of contexts that might exist universal reference:
1. Function template parameters;
2. auto declarations;
3. typedef declarations;
4. decltype expression;
- #### **Function template parameters**
```C++
template<typename T>
void func1(T&& param); //universal reference.
//---------------------------------------------------------------------------
template<typename T>
void func1(const T&& param); // rvalue reference, deduced parameter type is const T&&.
//--------------------------------------------------------
template<typename T>
class A{
public:
    void func(T&& param){}  // rvalue reference, as the type T already known.
}
//----------------------------------------------------------------------------
template<typename T>
class A{
public:
    template<typename S>
    void func(S&& param){}  // universal reference.
}
//----------------------------------------------------------------------------
template<typename T>
void func(std::vector<T>&& p); 
// rvalue reference, here exist type deduction of T, but it's the type of vector, but not the T of T&&.
```
- #### **auto declarations**
```C++
auto&& a = 10; // universal reference. 
int x = 20;
auto&& b = x; // universal reference.
const auto&& c = 20; // universal reference.
const auto&& d = b; // error, as d is const int && but b is int &. Can not bind an lvalue to rvalue.
```
- #### **typedef declarations**
```C++
class A{
    // do something here ...
}
A a;
typedef A&& myType;
myType& b = a; // reference collapsing, b's type is A&.
const myType& c = move(a);  // c's type is const A&.
myType&& d = move(a); // the type of d is A&&.
```
- #### **decltype expression**
For decltype it's different from auto.
```C++
int v;
decltype(v) &&a = 10; // type of a in an rvalue, int &&.
decltype((v)) &&b = v; // as v has been parenthesized which indicate reference, the type of b is int &.
```
- ## **Reference collapsing rules**
The reference collapsing rules make universal reference work. We can not declare a reference to reference, but the compiler can do it. When the compiler meet such a case like int && &&, it will run the reference collapsing rules.
![image](https://github.com/DYTLDSH/C/blob/main/figs/reference_collapse_rules.png)
- ## **move**
