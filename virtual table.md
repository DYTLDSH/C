# V-table in memory
    V-table is been stored in initialized data segment. It been created at the compile stage. So how many V-table are there in a program? That depends on how many
    classes are there in a program, each class has a V-table and each instance of a class has a vptr(only when the class has virtual function). If the class do not 
    have virtual function, then the class will do not have V-table, neither the vptr of its instance. As we can check that by the difference of the size of its instance.
