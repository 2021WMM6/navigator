#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <signal.h>
#include <time.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>

#define ARGNUM 6
#define MNTRDIR "watching"      //볼 디렉토리

char programpath[PATH_MAX],monitorpath[PATH_MAX];

int dotfilter(const struct dirent *ent) //scandir시 ., .. 제외
{ return strcmp(ent->d_name,".") && strcmp(ent->d_name,".."); }


int main()
{
    getcwd(programpath,PATH_MAX);                           //프로그램 경로를 저장
    sprintf(monitorpath,"%s",programpath);    //모니터링 디렉토리 경로를 저장

    do_tree();
}
void print_tree(const char *curpath,const char *fname,int curdepth,int peod,int eod)    //curdepth:지금 디렉토리 몇개나 들어갔는지 eod:디렉토리 끝 여부 peod:이전의 eod
{
    struct stat stbuf;
    struct dirent **items;
    char itempath[PATH_MAX];
    int i,itemnum;
    for(i=0;i<curdepth-1;i++)   printf(" %s\t ",i==curdepth-2&&peod&&eod?" ":"│");
    if(eod) printf(" └────────");   //디렉토리의 마지막인 경우
    else printf(" ├────────");
    printf("%s\n",fname);
    
    lstat(curpath,&stbuf);
    if(S_ISDIR(stbuf.st_mode))
    {
        itemnum = scandir(curpath,&items,dotfilter,alphasort);  //알파벳 순 출력
        for(i=0;i<itemnum-1;i++)
        {
            sprintf(itempath,"%s/%s",curpath,items[i]->d_name);
            lstat(itempath,&stbuf);
            print_tree(itempath,items[i]->d_name,curdepth+1,eod,0);
            free(items[i]);
        }
        if(itemnum)
        {
            sprintf(itempath,"%s/%s",curpath,items[i]->d_name);
            lstat(itempath,&stbuf);
            print_tree(itempath,items[i]->d_name,curdepth+1,eod,1);        //디렉토리 마지막 처리
            free(items[i]);
            free(items);
        }
    }
}
int do_tree()
{
    //TODO: 기본동작 : 디렉토리 구조 보여주기
    struct stat stbuf;
    struct dirent **items;
    char curpath[PATH_MAX],itempath[PATH_MAX];
    int i,itemnum;

    chdir(monitorpath);
    if(access(programpath,F_OK))
    { fprintf(stderr,"%s is not exist!\n",MNTRDIR);    return -1;}

    strcpy(curpath,monitorpath);

    printf("%s\n",monitorpath); //depth : 0

    //하위 디렉토리 출력
    itemnum = scandir(curpath,&items,dotfilter,alphasort);  //알파벳 순 출력
    for(i=0;i<itemnum-1;i++)
    {
        sprintf(itempath,"%s/%s",curpath,items[i]->d_name);
        lstat(itempath,&stbuf);
        print_tree(itempath,items[i]->d_name,1,0,0);
        free(items[i]);
    }
    if(itemnum)
    {
        sprintf(itempath,"%s/%s",curpath,items[i]->d_name);
        lstat(itempath,&stbuf);
        print_tree(itempath,items[i]->d_name,1,0,1);        //디렉토리 마지막 처리
        free(items[i]);
        free(items);
    }

    chdir(programpath);
}