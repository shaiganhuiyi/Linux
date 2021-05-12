#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>

int main()
{
    //1.創建套接字
    int sockfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if (sockfd <0){
     perror("socket error");
     return -1;
     }
    struct sockaddr_in srv_addr;
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(9000);
    srv_addr.sin_addr.s_addr = inet_addr("192.168.80.131");
    socklen_t len =sizeof(srv_addr);
    //2.綁定地址信息(不推薦）
    while(1){
    //3.發送數據
    char tmp[1024] = {0};
    printf("client say: ");
    fflush(stdout);
    scanf("%s",tmp);
    int ret = sendto(sockfd,tmp,strlen(tmp),0,(struct sockaddr*)&srv_addr,len );
    if(ret<0){
        perror("sendto error");
	return -1;
	}
    //4.接受數據
    memset(tmp,0x00,1024);
    ret = recvfrom(sockfd,tmp,1023,0,NULL,NULL);
    if (ret < 0){
       perror("recvfrom error");
       return -1;
       }
       printf("server say: %s\n",tmp);
    }
    //5.關閉套接字
    close(sockfd);


    return 0;
}
