
#include "param.h"
#include "types.h"
#include "memlayout.h"
#include "elf.h"
#include "riscv.h"
#include "defs.h"
#include "fs.h"

void backtrace(){
   // struct proc* p=myproc();
     uint64 cur_fp=r_fp();

    uint64 end=PGROUNDUP(cur_fp);
    //printf("end:%p\n",end);

   

    uint64 prev_fp=*(uint64*)(cur_fp-16);
    uint64 cur_ra=*(uint64*)(cur_fp-8);


    while(1){
      //  printf("===============\n");
        //printf("cur_ra:%p\n",cur_ra);
        printf("%p\n",cur_ra);
       // printf("prev_fp:%p\n",prev_fp);
       // printf("cur_fp:%p\n",cur_fp);
      //  printf("===============\n\n");

        cur_fp=prev_fp;
         if(cur_fp>=end){
            break;
        }
        prev_fp=*(uint64*)(cur_fp-16);
        cur_ra=*(uint64*)(cur_fp-8);
       
    }
    
}