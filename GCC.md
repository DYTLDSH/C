# gcc 和 g++的区别
https://www.cnblogs.com/samewang/p/4774180.html
# gcc  
## 语法  
gcc (选项) (参数)  

选项有：  
  - **-o** 指定生成的输出文件；  
  - **-E** 仅执行预编译处理止， 生成预编译好的文件，对应预编译阶段；  
  - **-S** 仅执行到编译处理止，可将c代码编译为汇编代码，也可将预编译输出文件编译为汇编文件，对应编译阶段；  
  - **-C** 将汇编代码转成目标文件，但不进行链接，对应于汇编阶段；  
  - **-Wall** 显示警告信息；
  - **-g** 打开debug。  

若要用gcc编译.cpp文件，参考如下格式：  
```gcc helloworld.cpp -lstdc++ -o helloworld```  
**-lstdc++** 的作用是链接标准库。
