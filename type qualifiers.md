# Three Kinds of Type Qualifiers
- ## **Const, Volatile, Mutable**
Type qualifiers is used to refine the declaration of a variable, a function and parameters. In C++ there are three kinds of qualifiers. The type qualifiers specify something about how it is accessed or where it is stored. Specifically, type qualifiers const and volatile also named as **cv-qualifiers**. The type qualifiers are only meaningful for lvalues.
- ## **Const**
The const qualifier defines the type that is a constant whoes value can not be changed. When define a variable that is a constant it must be initialized. Because if we do not initialize it as we can not modify its value this makes the variable useless.
In C++ there are two kinds of constant that always comfuse us: **constant pointer, pointer to constant**.

```C++
const int *ptr; 
int const *ptr;
```

The two kinds of defination declares a pointer to const int type. The content of the pointer point to can not be changed, but the pointer itself can point to other place.

```C++
int * const ptr;
```

The two kinds of defination declares a pointer to const int type. The content of the pointer point to can not be changed, but the pointer itself can point to other place. This defines a const pointer to int type. The pointer itself is a constant which means that the pointer can not point to other location, but the content of the pointer point to can be changed.

Dose constant must can not be changed? The answer is "NO". Below gives two example:
``` C++
int main(){
    const int i = 200;
    int *p = (int *)&i;
    *p = 100;
    cout << i << " " << *p << endl;
    cout << &i << " " << p;
    return 0;
}
```
```C++
200 100
0x6dfee8 0x6dfee8
```
The constant defined here is a local constant, we can change its value through pointer. Through the output we know that p and i are point to the same location, but why it still output different values? When const qualifier qualified the basic data type, the variable will be replaced with the value first give to it during the compliation phase. So when we output the value of i it will always gives us 200. But we changed the value in its physical memory, the output gives us the changed value 100. When we assign value to a constant, the complier will gives us an error. So for a constant we can only make sure it is a constant in the compliation phase, but at the running time, the value can be changed through pointer.
```C++
struct node{
    int a;
    int b;
    node(int v1, int v2):a(v1), b(v2){}
};

int main(void)
{
    const node i(10, 20);
    cout << i.a << " " << i.b << endl;
    node *p = (node*)&i;
    p->a = 20;
    p->b = 10;
    cout << i.a << " " << i.b << endl;
    cout << p->a << " " << p->b;
    return 0;
}
```
```C++
10 20
20 10
20 10
```
For struct or class, although, we define the instance is a constant, the variable members are not constant, so we can change its value through a pointer. The complier can not detect this kind of change. And do not like basic type's constant, the complier do no replace with some value, when a constant struc/class object is created, the complier will give a unused memory to it and make sure the object hasn't been assgined value directly. So the above code tells us i and p have the same value after the change.

Can any constant's value can be changed through pointer? The answer is also "NO". Only **local constant** variable can be changed in this way. For **static local constant, global constant, static global constant**, these variables are stored on the read only data segment, they are "real constant", the value can not be changed during its lifetime.
 
When declare a const object it must be initialized;  
When there are some constant members in class, any non-constant member function of the class can not call the constant member variable. Constant member variable can only be called by constant member function;  
For constant variables of a class, it can only be initialized in the initialization list after the constructor. For static constant int/enumeration type variables of a class, it can initialized both in the class and outside the class. But for other kinds of static constant variables can only be initialized outside the class.

- ## **Volatile**

- ## **Mutable**
Mutable can only be used in **class** or **struct**. We can qualify a data member of a class or struct as mutable type. A mutable data member of a constant class/struct object can be modified. We can change mutable members' value in const member function and assign value to it directly. Let me give a scenario to explain the reason why we want to change the data members' value of a constant class/struct object.  
For every student, the school will keep students' personal information. After that, the sheet of the personal information should be kept unchanged. But what if some students have their phone number changed or address changed and so on, they need to changed their personal sheet. The following code gives the details:
```C++
#include<iostream>

using namespace std;

class Student{
public:
    int ID;
    string name;
    mutable string phone;
    mutable string address;
    Student(int id, string name_, string number, string addr){
        ID = id;
        name = name_;
        phone = number;
        address = addr;
    }
    void changePhone(string newPhone) const{
        phone = newPhone;
    }
    void changeAddr(string newAddr) const{
        address = newAddr;
    }
    void display() const{
        cout << ID << endl;
        cout << name << endl;
        cout << phone << endl;
        cout << address << endl;
    }
};

int main(){
    const Student Jack(1232, "Jack", "18921783456", "SJTU");
    Jack.display();
    Jack.changePhone("18721817629");
    Jack.address = "Shanghai";
    Jack.display();
    return 0;
}
```
```C++
1232
Jack
18921783456
SJTU
1232
Jack
18721817629
Shanghai
```
Jack is a constant instance of the Student class, its data member phone and address are mutable, the value can be changed through both const function and assign value directly. 
For a class or struct, most of its members should be constant but few need to change, in this scenario, define this kind of member as mutable will be very convenient and safe.
