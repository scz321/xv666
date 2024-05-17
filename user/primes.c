#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void helper(int fd[2]){
   // printf("--------start------\n");

    int arr[36];

    read(fd[0],arr,sizeof arr);
    // for(int i=2;i<36;i++){
    //     printf("%d ",arr[i]);
    // }
  //  printf("\n");

    //int newarr[36];

    int idx=-1;
    int flag=0;

    for(int i=2;i<36;i++){
        if(arr[i]!=-1){
            idx=i;
            flag=1;
            break;
        }
    }
    if(flag==0){
        exit(0);
    }
    printf("prime %d\n",arr[idx]);

    for(int i=idx+1;i<36;i++){
        if(arr[i]%arr[idx]==0){
            arr[i]=-1;
        }
    }
    arr[idx]=-1;

    close(fd[0]);
    close(fd[1]);

    int newfd[2];
    pipe(newfd);
   // printf("--------end------\n");


    if(fork()==0){  
        write(newfd[1],arr,sizeof arr);
         //close(newfd[1]);
        helper(newfd);

        exit(0);

    }else{
        close(newfd[0]);
        close(newfd[1]);

        wait(0);
    }


}

int main(int argc,char* argv[]){

    int a[36];
    for(int i=0;i<35;++i){
        a[i]=i;
    }

   

    int fd[2];
    pipe(fd);
    

    int pid=-1;

    pid=fork();
    if(pid==0){
        write(fd[1],a,sizeof(a));

        helper(fd);
    }
    else{
        close(fd[0]);

        
        close(fd[1]);

        wait(0);


    }




}