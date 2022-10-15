#include <stdio.h>
int recursive_fib(unsigned long int i){
    unsigned long int return_val;
    if (i==0)
        return 0;
    else if (i==1)
        return 1;
    else{
        return_val = recursive_fib(i-1)+recursive_fib(i-2);
        return return_val;
    }
}

int cyclic_fib(unsigned long int i){
    unsigned long int res,num_prev,num_next;
    num_prev = 0;
    num_next = 1;
    if (i==1){
        return 1;
    }
    if (i==0){
        return 0;
    }
    for (int k =1;k<=i-1;k++){
        res = num_prev+num_next;
        num_prev = num_next;
        num_next = res;
    }
    return res;

}

int main(){
    unsigned long int cyclic_fib_res;
    int i;
    unsigned long int recursive_fib_res;
    while (scanf("%i",&i)!=EOF ){
        cyclic_fib_res =cyclic_fib(i);
        printf("%lu\n",cyclic_fib_res);
        recursive_fib_res = recursive_fib(i);
        printf("%lu\n",recursive_fib_res);
    }
    return 0;
     
}
