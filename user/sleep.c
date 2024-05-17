
#include "kernel/types.h"
#include <user/user.h>


int main(int argc,char* argv[]){
    if(argc!=2){
        printf("incorrect arguments!\n");
        exit(-1);
    }

    int n=atoi(argv[1]);

    sleep(n);






    //用exit退出子进程是更加规范的做法
    exit(0);
}