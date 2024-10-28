#include<stdio.h>
#include<stdlib.h>
#include "../allocator/allocator.h"
#include "test.h"
void test(bool res,char * msg){
    if(res)
    printf("\033[1;32m[PASSED]\033[0m %s\n",msg);
    else{
        printf("\033[1;31m[FAILED]\033[0m %s\n",msg);
        exit(EXIT_FAILURE);
    }
}
int main(){
    init_allocator(0);
    test(tree_test(),"Tree test");
    test(linked_list_test(),"Linked list test");
    test(array_test(),"Array test");
}