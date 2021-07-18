#include<stdio.h>
#include<string.h>
int main()
{
   FILE *fp=NULL;
   fp=fopen("./test.txt","r+");
   if(fp==NULL){
    perror("fopen error") ;
    return -1;
    }
    //fseek跳转读写位置 SEEK_SET-从文件起始偏移 SEEK_CUR-从当前读写位置偏移
    //SEEK_END-从文件末尾偏移
    fseek(fp,0,SEEK_END);//将文件的读写位置偏移末尾

    char buf[] ="今天天气真好，适合再睡一觉\n";
    int ret= fwrite(buf,strlen(buf),1,fp);//(数据，块大小，块个数，流指针）;
    if(ret == 0){
       perror("fwrite error");
       return -1;
       }
      printf("write ret:%d\n",ret);
       fseek(fp,0,SEEK_SET);
       char tmp[1024]={0};
        ret=fread(tmp,1023,1,fp);
       printf("ret:%d-[%s]\n",ret,tmp);
     fclose(fp);
     return 0;
}
