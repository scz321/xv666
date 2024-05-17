#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(){



    int p2c[2];
    int c2p[2];

    pipe(p2c);
    pipe(c2p);
    char ch;
    int pid=fork();

    if(pid==0){
        //step 1,wait
        close(p2c[1]);

        read(p2c[0],&ch,1);
        printf("<%d>: received ping\n",getpid());

        close(p2c[0]);

        ch='B';

        write(c2p[1],&ch,1);
        close(c2p[1]);
        close(c2p[0]);
        
        exit(0);




    }
    else{
        close(p2c[0]);
        ch='A';
        write(p2c[1],&ch,sizeof(ch));
        close(p2c[1]);

        read(c2p[0],&ch,1);
        printf("<%d>: received pong\n",getpid());
         close(c2p[1]);
        close(c2p[0]);




    }



}