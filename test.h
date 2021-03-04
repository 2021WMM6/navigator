#include <stdio.h>                                                           
#include <ncurses.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#define EXIT_KEY 'x'
int termy,termx;
char wd[PATH_MAX];
char dp[PATH_MAX];
char tp[PATH_MAX];
char strbuf[PATH_MAX];
char name[100];
typedef struct list{
    char a[PATH_MAX];      //파일 이름
    int type;               //파일 타입
    int op;                 //디렉토리일 때 그 디렉토리를 열면 1이 되고 닫으>면 다시 0이 됨
    int clos;               //어떤 디렉토리를 열었을 때 그 디렉토리의 마지막 파일이면 1로 저장, 그 외에는 0으로 저장
    struct list *front;
    struct list *back;
    char list_change[PATH_MAX];         //수정 시간
    long int list_size;                 //파일 크기
}linked;
struct stat st; 
linked START;
linked TAIL;
linked *start = &START;
linked *end = &TAIL;
linked *cur;     //cur는 현재 가리키고 있는 링크드 리스트 주소 포인터 - 링크>드리스트 삭제, 삽입하는 등의 함수에 사용
linked *cur2 = NULL;    //cur2는 cur와 하는 일은 같지만 키보드 값에 따라 현재 가리키고있는 파일에 ()표시하기 위해 사용
linked *cur3 = NULL;   //링크드 리스트 정렬할때 쓰는 포인터
linked *least = NULL;
int temp = 0;
int x, y, row, column = 0;
int check = 0;          //check는 디렉토리를 open했을 때 open한 디렉토리가 같은 경로에 있는 다른 파일들 중 마지막일 때만 1값을 가짐
int stack[10001] = {0};
int top = 0;   
int pop();
void push(int );
int tops();
void save_tree_();
void delete_l();
void insert_l(char *);
void change_l();
void read_tree();
void open_dir();
void close_dir();
void sort_n();
void sorting_l();
void read_detail();
void detail();


