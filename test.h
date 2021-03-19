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
#define EXIT_KEY 'x'    //종료 키 x

int termy,termx;
char wd[PATH_MAX];
char dp[PATH_MAX];
char tp[PATH_MAX];
char strbuf[PATH_MAX];
char xpath[PATH_MAX];
char strex[PATH_MAX];
char name[100];

typedef struct list{
    char a[PATH_MAX];       //파일 이름
    int type;               //파일 타입
    int op;                 //디렉토리일 때 그 디렉토리를 열면 1이 되고 닫으>면 다시 0이 됨
    int clos;               //어떤 디렉토리를 열었을 때 그 디렉토리의 마지막 파일이면 1로 저장, 그 외에는 0으로 저장
    char list_change[PATH_MAX];         //수정 시간 문자열 형태로 저장
    long int list_size;                 //파일 크기
    time_t list_time;                   //수정 시간 time_t 형으로 받아온다
    int pin;            //pin 기능 확인하기
    int search_list;    //search기능을 사용했을 때 이름에 키워드가 포함되어 있다면 1로 변경
    struct list *front;
    struct list *back;
}linked;

char File_n[100];
struct stat st; 
linked START;
linked TAIL;
linked *start = &START;
linked *end = &TAIL;
linked *cur;            //cur는 현재 가리키고 있는 링크드 리스트 주소 포인터 - 링크>드리스트 삭제, 삽입하는 등의 함수에 사용
linked *cur2 = NULL;    //cur2는 cur와 하는 일은 같지만 키보드 값에 따라 현재 가리키고있는 파일에 ()표시하기 위해 사용
linked *cur3 = NULL;    //여러가지 상황에서 쓰이는 임시 링크드 리스트 포인터
linked *cur4 = NULL;
linked *least = NULL;   //least는 sort기능에서 최솟값을 가지는 링크드리스트를 가리키는 포인터

int temp = 0;
int x, y, row, column = 0;
int check = 0;          //check는 디렉토리를 open했을 때 open한 디렉토리가 같은 경로에 있는 다른 파일들 중 마지막일 때만 1값을 가짐
int stack[10001] = {0};
int top = 0;
int empty = 0;
int move_type = 0;
char newname[100];      //rename할때 바꿀 새로운 이름
char curname[100];      //현재 이름
char lastname[103];     //커서 포함할때 사용

int pop();
void push(int );
int tops();

void print_menu(int );
int copy();
void copy_file_select();
void rena();
void file_rename();
void location_p(int );
void location_home(int );
void select_location(int );
void move_file();
void move_file_select();
void delete_file();
void delete_file_select();
void search(char *);
void select_search_list();
void get_name();
void print_search_list();
void use_search();
void save_tree_();
void delete_l();
int dir_size(char *);
void insert_l(char *);
void change_l();
void read_tree();
void open_dir();
void close_dir();
void sort_n();
void sorting_l();
void read_detail();
void detail();
void print_detail();
void sorting_pin();
void use_pin();
void print_pin();
void use_search();
void print_search_list();
void select_search_list();
void print_tree();
void move_file_select();
void move_file();
void select_location(int con);
void save_tree();
void location_p();
void location_home();
void File_rename();
void rena();
void move_key(char);
