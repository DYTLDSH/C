# Three Kinds of Type Qualifiers
- ## **Const, Volatile, Mutable**
Type qualifiers is used to refine the declaration of a variable, a function and parameters. In C++ there are three kinds of qualifiers. The type qualifiers specify something about how it is accessed or where it is stored. Specifically, type qualifiers const and volatile also named as **cv-qualifiers**. The type qualifiers are only meaningful for lvalues.
- ## **Const**
The const qualifier defines the type that is a constant whoes value can not be changed. When define a variable that is a constant it must be initialized. Because if we do not initialize it as we can not modify its value this makes the variable useless.
In C++ there are two kinds of constant that always comfuse us: **constant pointer, pointer to constant**.

```C++
const int *ptr; 
int const *ptr;```

The two kinds of defination declares a pointer to const int type. The content of the pointer point to can not be changed, but the pointer itself can point to other place.

`int * const ptr;`

The two kinds of defination declares a pointer to const int type. The content of the pointer point to can not be changed, but the pointer itself can point to other place. This defines a const pointer to int type. The pointer itself is a constant which means that the pointer can not point to other location, but the content of the pointer point to can be changed.

``` C++
int main(){
    const int i = 200;
    int *p = (int *)&i;
    *p = 100;
    cout << i << endl << *p << endl;
    return 0;
}
```

When declare a const object it must be initialized.  
When there are some constant members in class, any non-constant member function of the class can not call the constant member variable. Constant member variable can only be called by constant member function.

- ## **Volatile**

- ## **Mutable**
