#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>//网络编程接口头文件
#include<arpa/inet.h>//字节序转换接口头文件
#include<sys/socket.h>//套接字接口头文件
#include<sys/select.h>
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
      fd_set set;//解决客户端只能与服务器通信一次
      FD_ZERO(&set);//清空集合
      FD_SET(sockfd,&set);//将监听描述符添加到监控集合中，监控是否有新连接到来
     int max_fd = sockfd;
     while(1){
       struct timeval tv;
       tv.tv_sec = 3;
       tv.tv_usec = 0;
       fd_set rfds;
       memcpy(&rfds,&set,sizeof(fd_set));//因为select每次返回前都会移除集合中没有就绪的描述符
       //因此使用set备份所有要监控的描述符，监控的时候单独创建临时集合进行      监控
       int ret = select(max_fd +1 ,&set, NULL,NULL,&tv );
       if(ret < 0){
       perror("select error");
       continue ;
       }else if ( ret == 0){
             printf("No descriptor ready, wait timeout\n");
	     continue;
	     }
	for (int i=0; i<=max_fd; i++){
	  if (FD_ISSET(i,&rfds)){
	  //i这个描述符就绪了
	  if(i==sockfd){ //监听描述符就绪 
       struct sockaddr_in cliaddr;
       int newfd = accept(sockfd, (struct sockaddr*)&cliaddr,&len);//阻塞接口--没有新连接则会等待知到有新连接
       if (newfd <0){
         perror("accept error");
	 continue;
	 }
	 FD_SET(newfd, &set);//将新连接也添加监控
	 max_fd = newfd > max_fd ? newfd: max_fd;
	 printf("new connet\n");
	 }else { //就绪的是通信描述符
	 
     //5.使用新连接接受发送数据--与客户端进行通信
        char buf[1024] = {0};
	ret = recv(i,buf,1024,0);//阻塞接口
	if (ret < 0){
	perror("recv error");
	close(i);
	FD_CLR(i,&set);//解除监控
	continue;
	}else if (ret == 0){
	printf("peer shutdown");
	close(i);
	FD_CLR(i,&set);//解除监控
	continue;
	}
       printf("client say:%s\n",buf);
       ret = send(i, buf, strlen(buf),0);
       if(ret < 0){
           
	    close(i);
	    FD_CLR(i,&set);//解除监控
	    continue;
	    }
	 }   
     }

	}
    }	
     //6.关闭套接字
     close(sockfd);
     return 0;
}
