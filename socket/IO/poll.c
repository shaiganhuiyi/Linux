#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>//网络编程接口头文件
#include<arpa/inet.h>//字节序转换接口头文件
#include<sys/socket.h>//套接字接口头文件
#include<poll.h>
int main()     //一个客户端只能与服务端通信一次，如果循环就会卡在获取新连接这里。
{              // 在获取新连接后，上一个accept还没有销毁资，造成资源泄露

     //1.创建套接字
     int sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
     if(sockfd < 0){
      perror("socket error");
      return -1;
      }
     //2.绑定地址信息
     struct sockaddr_in addr;
     addr.sin_family = AF_INET;
     addr.sin_port = htons(9000);
     addr.sin_addr.s_addr = inet_addr("0.0.0.0");
     socklen_t len =sizeof(addr);
     int ret = bind(sockfd, (struct sockaddr *)&addr, len);
     if(ret< 0){
          perror("bind error");
	  return -1;
	  }
     //3.开始监听
     ret = listen(sockfd,5); //同一时间所能受理的请求数为5
     if(ret< 0){
      perror("listen error");
      return -1;
      }
      int fd_num = 1;
      struct pollfd fds[10];
      fds[0].fd = sockfd;
      fds[0].events = POLLIN;//监控socketd的可读事件
      while(1){ 
     //4.获取新连接
       int ret = poll(fds, fd_num,3000);
       if (ret < 0){
           perror("poll error");
	   continue;
	   }else if(ret == 0){
	   printf("No descriptor ready, wait timeout\n");
	   continue;
	   }
	 for(int i =0; i< fd_num; i++){
	      if(fds[i].revents & POLLIN){//可读事件
                 if(fds[i].fd == sockfd){//监听描述符有新连接             
       struct sockaddr_in cliaddr;
       int newfd = accept(sockfd, (struct sockaddr*)&cliaddr,&len);//阻塞接口--没有新连接则会等待知到有新连接
       if (newfd <0){
         perror("accept error");
	 continue;
	 }
	 fds[fd_num].fd = newfd; fds[fd_num].events = POLLIN;
	 fd_num++;
                 }else{
     //5.使用新连接接受发送数据--与客户端进行通信
        char buf[1024] = {0};
	ret = recv(fds[i].fd,buf,1024,0);//阻塞接口
	if (ret <= 0){
	perror("recv error");
	close(fds[i].fd);
	fds[i].fd = -1;
	continue;
	}
       printf("cilent say:%s\n",buf);
       ret = send(fds[i].fd, buf, strlen(buf),0);

		 }
	      }else if (fds[i].revents & POLLOUT){//可写事前
	      } 
	    }
     }

     //6.关闭套接字
     close(sockfd);
     return 0;
}
