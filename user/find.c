#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"


char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;
  
  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), 0, DIRSIZ-strlen(p));
  return buf;
}


void find(char* cur,char* tgt)
{

   // printf("cur:%s\n",cur);
    struct dirent de;
    struct stat st;

    int fd;
    fd=open(cur,O_RDONLY);
    if(fd<0){
        fprintf(2, "find: cannot open %s\n", cur);//事实证明，没有它可能会出现隐性bug
        return;
    }

    fstat(fd,&st);

    switch(st.type){
        //这种写法是两种case一起处理吗。。
        case T_DEVICE:
        case T_FILE:
           // printf("fmtname(cur):%s\n",fmtname(cur));
            //printf("tgt:%s\n",tgt);
            //printf("strlen(tgt):%d\n",strlen(tgt));

            //if(fmtname(cur)==tgt){//大哥，你这里是在给两个指针作比较，是不是string用太多了。。
            if(strcmp(tgt,fmtname(cur))==0){
                printf("%s\n",cur);
            }
            close(fd);
            return;
            break;
        case T_DIR:
            // if(cur=="."||cur==".."){//不支持==
            //     break;
            // }
            
            //如何获取目录下的所有的广义文件的路径？--不断read即可
            char path[1024];
            strcpy(path,cur);
            int len=strlen(cur);


            
            while(read(fd,&de,sizeof(de))== sizeof(de)){
                // printf("de.name:");
                // for(int i=0;i<DIRSIZ;i++){
                //     printf("%c|",de.name[i]);
                // }
                // printf("\n");
                if(de.inum == 0)
                    continue;
                if(strcmp(de.name,".")==0||strcmp(de.name,"..")==0){
                    continue;
                }
                int idx=len;
                path[idx]='/';
                strcpy((char*)path+idx+1,de.name);//数组和字符还是要区分的？
                find(path,tgt);
            }
            close(fd);
            break;
    }


}

int main(int argc,char* argv[]){

    if(argc!=3){
        printf("wrong arguments!\n");
    }


    char* cur=argv[1];
    char* target=argv[2];

    find(cur,target);


    exit(0);
}