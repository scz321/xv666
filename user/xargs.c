#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"
#define MAX_LEN 100

void tokenReset(char* token){
    memset(token,0,20);
}

void ARGVprint(char* a[]){
    int i=0;
    while(a[i]!=0&&i<32){
        if(a[i][0]!=' ')
        printf("&a[i]:0x%x,argv[%d]: %s\n",a[i],i,a[i]);
        i++;
    }
}

//值得积累的bug
void tokenprint(char* token){
    //int i=0;
    for(int i=0;i<20;i++){
        if(token[i]==0){
            break;
        }
        printf("%c",token[i]);
    }
    printf("\n");
}

char*
mystrcpy(char *s, const char *t)
{
  char *os;

  os = s;
  while(((*s++ = *t++) != 0)){
   // printf("copy 1 char%c\n",*t);
  }
    ;
  return os;
}



int main(int argc, char *argv[]) {

    //先把标准输入里面的内容都读进来：

    char buf[MAX_LEN];
    char ch;
    //while(read(0,&ch,1))
    read(0,buf,sizeof(buf));//默认不会超过100字节

   
    //char ch=buf[idx];

   

    //可以基于状态机的视角去进行思考设计
    char token[20];
    tokenReset(token);
     int idx=0;
    char* ARGV[MAXARG];

    //memset(ARGV,0,sizeof(ARGV));
    for(int i=0;i<MAXARG;i++){
        ARGV[i]=malloc(20);
       // printf("%x\n",ARGV[i]);
        //ARGV[i][0]=0;
    }
    int Aidx=argc-1;
     for(int i=0;i<argc;i++){//最后一个是NULL，这里需要把这种情况排除掉
            //ARGV[i]=argv[i+1];这里还是对指针进行赋值，还是十分危险的！
            strcpy(ARGV[i],argv[i+1]);
    }
    int i=0;
    int flag=1;
    while(1){
        ch=buf[i++];

        switch(ch){
            case 0:
                //ARGV[Aidx++]=token;这种写法是一个典型的指针错误，token这个指针的值是在动态更新的！
                //对指针进行赋值是很危险的！！！！！
                //正确的做法是使用memcpy进行深拷贝
                
                // printf("case '\\0':\n");
                // printf("Aidx:%d\n",Aidx);
                // mystrcpy(ARGV[Aidx++],token);
                
                // ARGVprint(ARGV);
                // tokenprint(token);
                // if(fork()==0){
                //     printf("Aidx:%d\n",Aidx);
                //     ARGV[Aidx]=0;
                //     exec(ARGV[0],ARGV);
                // }
                // else{
                    
                //     wait(0);
                //     printf("exec finish\n");
                // }
                flag=0;
                break;

            case ' ':
            
                //ARGV[Aidx++]=token;
               printf("case ' ':\n");
                //printf("Aidx:%d\n",Aidx);
               // printf("%d\n",ARGV[Aidx]);
                mystrcpy(ARGV[Aidx++],token);
                ARGVprint(ARGV);
                tokenprint(token);
                tokenReset(token);
                idx=0;
                break;

            case '\n':
                //ARGV[Aidx++]=token;
                mystrcpy(ARGV[Aidx++],token);


                printf("case '\\n':\n");
                ARGVprint(ARGV);
                tokenprint(token);
                char *tmp=ARGV[Aidx];
                if(fork()==0){
                    ARGV[Aidx]=0;
                    exec(ARGV[0],ARGV);
                }
                else{
                    ARGV[Aidx]=tmp;
                    wait(0);
                }

                //reset
                //memset(ARGV,0,sizeof(ARGV));
                Aidx=argc-1;
                for(int i=0;i<argc;i++){
                        //ARGV[i]=argv[i+1];指针赋值，警钟长鸣
                        strcpy(ARGV[i],argv[i+1]);
                }
                for(int i=argc;i<MAXARG;i++){
                    memset(ARGV[i],0,20);
                }
                tokenReset(token);
                idx=0;
                break;
            default:
                printf("case default:\n");
                
                token[idx++]=ch;
                tokenprint(token);
                
                break;
        }
        
       // ARGVprint(ARGV);
        if(flag==0){
            //printf("break!!\n");
            break;
        }

    }

    exit(0);
    
}