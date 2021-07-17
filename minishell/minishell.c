#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>


int main()
{
  while(1){
  //增加一个shell提示
  printf("[Joker@minishell]$ ");
  fflush(stdout);//刷新缓冲区
  //1.等待标准输入
  char buf[1024] ={0};
  fgets(buf, 1023,stdin);
  buf[strlen(buf)-1]='\0';
  //2.对输入数据进行解析
  char *argv[32]={NULL};
  int argc = 0;
  char *ptr =buf;
  while(*ptr !='\0'){
       if(*ptr!=' '){
           argv[argc]=ptr;
	   argc++;
           while(*ptr !=' '&& *ptr!='\0'){
		 ptr++;           
	   }
	   *ptr ='\0';
       }
       ptr++;   
  }
  argv[argc]=NULL;//最后一个参数的下一个位置置NULL
  //3.创建子进程,在子进程中程序替换
   pid_t pid=fork();
   if(pid==0){
      execvp(argv[0],(char**)argv);//程序运行成功后就去运行新程序了，33行以后就不会运行
      //能够走到第35行，那么肯定程序替换失败了。
      perror("execvp error");//打印上一次系统调用接口使用的错误原因
      exit(0);
    }
   
  //5.进程等待
   wait(NULL);
  }
  return 0;


}
