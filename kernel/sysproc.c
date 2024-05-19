#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

//pgtbltest
#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  uint64 va=0;
  argaddr(0,&va);
 //printf("va:%p\n",va);
  int len=-1;
  argint(1,&len);
  uint64 maskadd=0;
  argaddr(2,&maskadd);

  struct proc* p=myproc();
   vmprint(p->pagetable,1);

  uint32 kmask=0;
  for(int i=0;i<32;i++){
    pte_t* pte=walk(p->pagetable,(uint64)(va+i*PGSIZE),0);

    if(PTE_A&*pte){
     // printf("i:%d\n",i);
      //printf("va:%p\n",(va+i*PGSIZE));
      kmask=kmask|(1<<i);
      *pte=*pte&(~PTE_A);
    }
    
  }
  //printf("kmask:%x\n",kmask);

  copyout(p->pagetable,maskadd,(char*)&kmask,sizeof(kmask));

  return 0;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

//
