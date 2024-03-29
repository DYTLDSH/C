# Introduction
- ## **Key words**
lvalue, rvalue, lvalue reference, rvalue reference, universal reference, reference collapsing, move, forward.
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
const auto&& d = b; // error, as the type of d is const int && but b is int &. Can not bind an lvalue to rvalue.
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
What is move? How to use it?  
std::move is just a function of C++, **move doesn't move and move unconditionally cast to an rvalue**. std::move doesn't move anything, one thing it do is that when you pass in a variable into it, it will return back an rvalue no matter what you pass in. The principle of sht::move is as follows:
```C++
template<typename T>  
inline typename std::remove_reference<T>::type&& move(T&& t){ 
    return static_cast<typename std::remove_reference<T>::type&&>(t); 
}  
```
The function has only one line which returns a value. It cast type t to type "remove_reference<T>::type&&", then return.  
remove_reference is a class, and the "type" is a member of class, use :: to visit it. The effect of the remove_reference is to remove the reference.
```C++
template <typename T>
struct remove_reference{
    typedef T type;  // "type" is the alias of T
};
template <typename T>
struct remove_reference<T&>{  // lvalue reference
    typedef T type;
}
template <typename T>
struct remove_reference<T&&>{ // rvalue reference
   typedef T type;
}
```
First a value t is passed in, after the automatic type deduction, get the type of T. Then pass the T into remove_reference, remove the reference of T, get the type. Conbine "type" and "&&" to rvalue type&&. The last step is to use to static_cast cast t to type&&, return back. With the reference collapsing rules, the std::move function can pass in both lvalues and rvalues, eventually, it will return an rvalue.  
When I'm studying std::move semantics, an important thing I'm care about is that how to use it, and where to use it. See the example below:
- #### **move constructor and move assignment**
```C++
#include<iostream>
#include<vector>
using namespace std;

class A {
public:
vector<int> val;
A() {
	cout << "Default Constructing..." << endl;
}
A(vector<int> v) {
	val.resize(v.size(), 1);
	for (int i = 0; i < v.size(); ++i) {
		val[i] = v[i];
	}
	cout << "Given Value Constructing..." << endl;
}
A(A& a) {
	val.resize(a.val.size(), 1);
	for (int i = 0; i < a.val.size(); ++i) {
		val[i] = a.val[i];
	}
	cout << "Copy Constructing..." << endl;
}
};
int main() {
	vector<int> v(10);
	A a(v);
	A b(a);
	return 0;
}  
```
```C++
Given Value Constructing...
Copy Constructing...
```
What if the a doesn't need to be used after constructing b, here we can use std::move to reduce the replication step. Letting b take control of the resources of a directly by move. There are two methods, one is **move construtor** another is **move assignment**.
```C++
A(A&& a){
	val = a.val;  // move constructor
}
A& operator=(A&& a){
	val = a.val;  // move assignment
}
```
For a class, if we have write the move constructor function, when using an rvalue to construct an object, it will call the move constructor function. And if we have write the move assignment function, when assign an rvalue to an object, it will call the move assignment function. For an lvalue, can first use the std::move change it to rvalue. But if the lvalue has been state as a constant, then the move constructor function will not be called, it will fall back to the copy constructor. So **if want to move, don't define it as const**. One thing to take care of, make sure the moved object is an rvalue, if it don't, make sure it won't be used in the later time. Once the object has been moved, to set the object to NULL or nullptr for safe.  
If a class or other variables do not contain any resources, that won't be a good idea to use move.
- #### **temp variables**
```C++
vector<vector<int>> nums;
for(int i = 0; i < 10; ++i){
	vector<int> tmp{1,2,3,4,5};
	nums.push_back(move(tmp));
}
```
```C++
string a = "Hello World!";
string b = move(a);  // make sure a do not use after move
```
tmp is a temp vector, we can just move it into nums. After the move operation, vector tmp will becomes to an empty vector, the size of tmp will become 0, and a will become an empty string.
- #### **unique_ptr**
For an unique_ptr, it exclusively control a pointer, move can help here.
```C++
int* p = new int(10);
unique_ptr<int> ptr1(p);
unique_ptr<int> ptr2(move(ptr1));
unique_ptr<int> ptr3 = move(ptr2);
```
- ## **forward**
The biggest difference between move and forward is that, **move unconditionally cast to an rvalue, but forward conditionally cast to an rvalue**. If its an lvalue forward return an lvalue, if its an rvalue, forward return an rvalue.
```C++
#include<iostream>
#include<vector>
#include<memory>
using namespace std;

class A {
public:
	vector<int> val;
	A() {
		cout << "Default Constructing..." << endl;
	}
	A(vector<int> v) {
		val.resize(v.size(), 1);
		for (int i = 0; i < v.size(); ++i) {
			val[i] = v[i];
		}
		cout << "Given Value Constructing..." << endl;
	}
	A(A& a) {
		val.resize(a.val.size(), 1);
		for (int i = 0; i < a.val.size(); ++i) {
			val[i] = a.val[i];
		}
		cout << "Copy Constructing..." << endl;
	}
	A(A&& a) {
		val = a.val;
		cout << "Move Constructing..." << endl;
	}
};

template <typename T>
void func1(T param) {
	cout << "In func1" << endl;
}

template <typename T>
void func2(T&& param) {
	cout << "In func2" << endl;
	func1(param);
}

int main() {
	func2(A());
	return 0;
}
```
```C++
Default Constructing...
In func2
Copy Constructing...
In func1
```
In the main function, we pass in an rvalue into func2, the default constructor being called, then pass the param into func1, from the output we known that it becomes to an lvalue in func1. But sometimes it is not what we want. To make it still an rvalue in func1, we have to make a little bit change in func2:
```C++
template <typename T>
void func2(T&& param) {
	cout << "In func2" << endl;
	func1(std::forward<T>(param));
}
```
Then the output becomes:
```C++
Default Constructing...
In func2
Move Constructing...
In func1
```
