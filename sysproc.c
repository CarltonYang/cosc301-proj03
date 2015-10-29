#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}


int
sys_mprotect(void)
{
	void *addr;
	int len;

	//if addr is bigger than sz or its not evenly divible by PGSIZE, return -1
 	if(argint(0, addr) >= proc->sz || argint(0,addr)%PGSIZE!=0 )
     	{return -1;}

	//if len <= 0 or len is too big, return -1
	if(argint(1,&len) <=0 || argint(0,addr)+(argint(1,&len) >= proc->sz)
	{return -1;}

	return do_mprotect(addr, len);
}

int sys_munprotect(void) {

        void *addr;
	int len;

	//if addr is bigger than sz or its not evenly divible by PGSIZE, return -1
 	if(argint(0, addr) >= proc->sz || argint(0,addr)%PGSIZE!=0 )
     	{return -1;}

	//if len <= 0 or len is too big, return -1
	if(argint(1,&len) <=0 || argint(0,addr)+(argint(1,&len) >= proc->sz)
	{return -1;}

	return do_munprotect(addr, len);
}
