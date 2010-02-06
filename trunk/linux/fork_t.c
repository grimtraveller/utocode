/*
 *Name:		fork_t.c
 *Desc:		study fork()
 *Usage:	gcc fork_t.c -o fork_t
 *Date:		2008 12 31
 *Author:	zuohaitao
 */
#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int main(void)
{
pid_t result;
result=fork();
if(result==-1){
  perror("fork");
  exit;
}
else if(result==0){
printf("child, the return is %d\n",result);
}

else{
printf("father, the return is %d\n",result);}

return 0;
}
