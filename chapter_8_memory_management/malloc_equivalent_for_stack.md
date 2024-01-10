### malloc equivalent in stack

alloca() system call can be used to allocate dynamic memory in stack. It is faster than malloc() as it just needs the adjustment of stack-frame, plus it is not required to manually free() the memory. When the function returns out of scope the memory is automatically free by stack-frame adjustment. 

However, it's use is discouraged for portable systems. On linux, it performs well but the same cannot be said in even other unix os. Also, it is not suitable for big chunk of memory. It doesn't free when going out of scope on a bracket for-example, which is a pecularity to be remembered. It is only free when function goes out of scope.