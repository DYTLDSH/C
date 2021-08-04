# Memory Layout of C++ Program

In default, every function has a 2M memory space, but we can not use all of its space for variables, because it has to leave some space for some necessary information of the function.

The memory of a C++ program can be seperated into four main segment. Here the memory is the user space. They are code, data, heap and stack. And for data segment include the uninitialized data and the initialized data. Below gives an image which tells the memory layout of a program.

![image](https://github.com/DYTLDSH/C/blob/main/figs/memory_layout.png)

# [malloc](http://luodw.cc/2016/02/17/malloc/)
