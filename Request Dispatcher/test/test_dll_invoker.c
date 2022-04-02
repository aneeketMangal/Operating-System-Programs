#include <stdlib.h>
  
int main(){
    system("gcc test_dll_invoker_1.c -o test -ldl");
    system("./test");
    system("gcc test_dll_invoker_2.c -o test -ldl");
    system("./test");
    system("gcc test_dll_invoker_3.c -o test -ldl");
    system("./test");
    system("gcc test_dll_invoker_4.c -o test -ldl");
    system("./test");
    system("gcc test_dll_invoker_5.c -o test -ldl");
    system("./test");
    system("gcc test_dll_invoker_6.c -o test -ldl");
    system("./test");
    system("gcc test_dll_invoker_7.c -o test -ldl");
    system("./test");
    system("gcc test_dll_invoker_8.c -o test -ldl");
    system("./test");
    system("gcc test_dll_invoker_9.c -o test -ldl");
    system("./test");
}