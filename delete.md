- ## **Delete**
When deleting an object, what is going on?  
Delete an object will release the memory where the object point to, which tells the operating system the memory can be reallocated for others to use. But for the object itself still occupy 4 bytes memory, as the meaning of a pointer is storing the address of real data. Once deleting, the memory of real data occupied will be release, but for the pointer itself will still exist. Some compiler will make the pointer point to a random place, but some will still let the pointer point to the same place. So after deleting the pointer, it has to be careful do not use the pointer again to avoid unknown mistake.
```C++
#include<iostream>

using namespace std;

int main()
{
    int *a = new int(10);
    cout << *a <<endl;
    delete a;
    int *b = new int(20);
    cout << *a << endl;
    return 0;
}
```
For the above code, the result may different, that will depend on the current situation of the computer. As a has been deleted before allocate memory for b, the system have a chance to allocate the same address to b, then the running result is uncontrol. If a and b point to the same place, them the output is 10 and 20, if not, the output will be 10 and 10.
