#include<stdio.h>  //輸出函數
#include<unistd.h>  //close()
#include<string.h> 
#include<netinet/in.h>  //地址結構
#include<arpa/inet.h>//字節序轉換接口
#include<sys/socket.h>//套接字接口

int main()
{
       //1.創建套接字
       int sockfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
       if (sockfd < 0){ 
        perror("socket error");
	return -1;
	   }
       //2.綁定地址信息
       struct sockaddr_in addr;
       addr.sin_family = AF_INET;//描述地址結構（ipv4或ipv6）
       addr.sin_port = htons(9000);
       addr.sin_addr.s_addr = inet_addr("0.0.0.0"); //將字符串的點分十進制IP地址轉換為網絡字節序地址  
       socklen_t len = sizeof(addr);
       int ret = bind(sockfd,(struct sockaddr*)&addr,len);
       if(ret < 0){
         perror("bind error");
	 return -1;
	 }
       
       while(1)
       {  // 接收數據
        char tmp[1024] = {0};
        struct sockaddr_in cliaddr;
	ret = recvfrom(sockfd,tmp,1023,0,(struct sockaddr*)&cliaddr,&len);
	if(ret < 0){
	 perror("recvfrom error");
           }
	 printf("%s:%d say:%s\n",inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port),tmp);
        
       //4.發送數據
        memset(tmp,0x00,1024);
	printf("server say: ");
	fflush(stdout);//刷新緩衝區
	scanf("%s",tmp);
        ret = sendto(sockfd, tmp,strlen(tmp),0,(struct sockaddr*)&cliaddr,len);    
        if(ret < 0)
	{
	perror("sendto error");
	return -1;
        }
       }


        //5.關閉套接字
       close(sockfd);
       return 0;
}
