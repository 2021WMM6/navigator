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

typedef struct list{
    char a[PATH_MAX];      //파일 이름
    int type;               //파일 타입
    int op;                 //디렉토리일 때 그 디렉토리를 열면 1이 되고 닫으면 다시 0이 됨
    int clos;               //어떤 디렉토리를 열었을 때 그 디렉토리의 마지막 파일이면 1로 저장, 그 외에는 0으로 저장
    struct list *front;
    struct list *back;
    char list_change[PATH_MAX];
    long int list_size;
}linked;
struct stat st;
linked word[10001];
linked TAIL;
linked *start = word;
linked *end = &TAIL;
linked *cur = word;     //cur는 현재 가리키고 있는 링크드 리스트 주소 포인터 - 링크드리스트 삭제, 삽입하는 등의 함수에 사용
linked *cur2 = NULL;    //cur2는 cur와 하는 일은 같지만 키보드 값에 따라 현재 가리키고있는 파일에 ()표시하기 위해 사용
int j = 0;
int temp = 0;
int x, y, row, column = 0;
int check = 0;          //check는 디렉토리를 open했을 때 open한 디렉토리가 같은 경로에 있는 다른 파일들 중 마지막일 때만 1값을 가짐
void delete_l();
void insert_l(char *);
int stack[10001] = {0};
int top = 0;
int pop(){return top == 0 ? -1 : stack[--top];}
void push(int a){stack[top++] = a;}
int tops(){return top == 0 ? -1 : stack[top - 1];}
void save_tree(){
    word -> front = NULL;
    word -> back = NULL;
    TAIL = *word;
    start = word;
    end = &TAIL;
    start -> back = end;
    end -> front = start;
    j = 0;
    cur = start;
    DIR *dirp;      //디렉토리 포인터
    struct dirent *diritem;    //디렉토리 항목 포인터

    dirp = opendir(wd);
    while((diritem=readdir(dirp)) != NULL){
        if(strncmp(diritem -> d_name, ".", 1) != 0){
            snprintf(strbuf,PATH_MAX,"%s",diritem->d_name);
            temp = diritem -> d_type;
            char *str = strbuf;
            insert_l(str);      //링크드 리스트에 정보를 삽입
        }
    }
}
void delete_l(){
    linked *p = cur;
    if(p == start)  return ;
    p -> back -> front = p -> front;
    p -> front -> back = p -> back;
    cur = cur -> front;
}

void insert_l(char *ch){
    linked *p = word + j + 1;
    strcpy(p -> a, ch);
    p -> type = temp;
    p -> op = 0;
    p -> clos = 0;
    p -> back = cur -> back;
    p -> front = cur;
    cur -> back -> front = p;
    cur -> back = p;
    j++;
    cur = cur -> back;
}

void read_tree()
{
    int i = 0;
    printw(" --- ");
    getyx(stdscr, y, x); //현재 커서 좌표 구하는 함수
    row = x - 3;
    push(row);      //row정보 stack에 저장
    while(--termy > 0){
        if(cur -> op == 1){             //열린 디렉토리가 있을 때
            mvprintw(column, row, "\\");
            printw("-%.*s",termx-1,cur -> a);
            if(cur -> clos == 1)
                check = 1;
            cur = cur -> back;
            read_tree();
        }
        else if(cur -> clos == 1){ //열린 디렉토리의 마지막 파일일 때
            if(i == 0){                 //열린 디렉토리 파일이 하나만 있을 때
                printw("%.*s",termx-1,cur -> a);
                column = y + 1;
            }
            else{
                mvprintw(column, row, "\\");
                printw("-%.*s",termx-1,cur -> a);
                getyx(stdscr, y, x);
                column = y + 1;
            }
            pop();          //하위 디렉토리에서 상위 디렉토리의 row값으로 돌아가서 출력하기 위함
            if(check == 1){       //상위 디렉토리가 같은 경로의 파일 중 마지막 파일인지 확인
                pop();
                row = tops();
                check = 0;
            }
            else
                row = tops();
            return ;
        }
        else if(i == 0){
            printw("%.*s",termx-1,cur -> a);
            column = y + 1;
        }
        else{
            mvprintw(column, row, "|");
            printw("-%.*s",termx-1,cur -> a);
            getyx(stdscr, y, x);
            column = y + 1;
        }
        if(cur -> back == &TAIL)
            break;
        cur = cur -> back;
        i++;
    }
    if(termy > 0)
        mvprintw(column - 1, row, "\\");
}
void open_dir(){
    cur = cur2;
    snprintf(dp, PATH_MAX, "%s/%s", tp, cur -> a);           //dp는 열려고 하는 디렉토리 경로, tp는 현재 디렉토리 경로
    cur -> op = 1;
    DIR *dirp;      //디렉토리 포인터
    struct dirent *diritem;    //디렉토리 항목 포인터
    dirp = opendir(dp);
    while((diritem=readdir(dirp)) != NULL){
        if(strncmp(diritem -> d_name, ".", 1) != 0){        //.으로 시작하는 파일 제외하고 링크드 리스트에 저장
            snprintf(strbuf,PATH_MAX,"%s",diritem->d_name);
            temp = diritem -> d_type;
            char *str = strbuf;
            insert_l(str);
        }
    }
    strcpy(tp, dp);
    cur -> clos = 1;
}
void close_dir(){
    cur = cur2 -> front;
    cur -> op = 0;
    while(1){
        cur = cur -> back;
        if(cur -> clos == 1){
            delete_l();     //링크드 리스트에서 삭제
            break;
        }
        else
            delete_l();
    }
    cur2 = cur;
    int k = strlen(dp) - strlen(cur -> a) - 1;      //디렉토리를 닫았기 때문에 현재 경로에서
    sprintf(tp, "%.*s", k, dp);                     //디렉토리 이름 제거하여 경로 변경하여 tp에 저장           
    strcpy(dp, tp);
}
int main()
{
    int ch;
    time_t lasttime;
    initscr();
    cbreak();   //no linebuffering
    noecho();   //no echo of inputs
    nodelay(stdscr,TRUE);
    keypad(stdscr,TRUE);    //enable keypad, arrows
    getcwd(wd,PATH_MAX);    //현재 디렉토리 가져오기
    save_tree(); //파일 정보 링크드리스트에 저장
    cur2 = end -> front;
    snprintf(strbuf,PATH_MAX,"(%.4093s)", cur2 -> a);
    strcpy(cur2 -> a, strbuf);
    strcpy(tp, wd);         //wd는 최상위 디렉토리 경로로 바뀌지 않음
    while(1)    //탈출 조건-> x눌렀을때.
    {
        getmaxyx(curscr,termy,termx);     //가로세로 구하기
        lasttime=time(NULL);
        clear();
        //printw("%.*s\n",termx-1, tp);     //현재 경로가 어디인지 출력
        snprintf(strbuf,PATH_MAX,"%s",wd);
        printw("%.*s",termx-1,strbuf);
        switch(ch)
        {
            case KEY_DOWN:
            //TODO: 커서 다운
            if(cur2 -> back != &TAIL){
                if(cur2 -> clos == 0){
                    snprintf(strbuf,PATH_MAX, "%s", cur2 -> a + 1);
                    strncpy(cur2 -> a, strbuf, strlen(cur2 -> a) - 2);
                    cur2 -> a[strlen(cur2 -> a) - 2] = '\0';
                    cur2 = cur2 -> back;
                    snprintf(strbuf,PATH_MAX,"(%.4093s)", cur2 -> a);
                    strcpy(cur2 -> a, strbuf);
                }
            }
            break;
            case KEY_UP:
            //TODO: 커서 업
            if(cur2 -> front != start){
                if(cur2 -> front -> op == 0){
                    snprintf(strbuf,PATH_MAX, "%s", cur2 -> a + 1);
                    strncpy(cur2 -> a, strbuf, strlen(cur2 -> a) - 2);
                    cur2 -> a[strlen(cur2 -> a) - 2] = '\0';
                    cur2 = cur2 -> front;
                    snprintf(strbuf,PATH_MAX,"(%.4093s)", cur2 -> a);
                    strcpy(cur2 -> a, strbuf);
                }
            }
            break;
            case KEY_LEFT:
            //TODO: 커서 왼쪽(이전 위치)
            if(cur2 -> front -> op != 0){
                close_dir();        //디렉토리 닫기
                snprintf(strbuf,PATH_MAX,"(%.4093s)", cur2 -> a);
                strcpy(cur2 -> a, strbuf);
            }
            break;
            case KEY_RIGHT:
            //TODO: 커서 오른쪽(디렉토리 들어가기/동작없음)
            if(cur2 -> type == DT_DIR && cur2 -> op == 0){
                snprintf(strbuf,PATH_MAX, "%s", cur2 -> a + 1);
                strncpy(cur2 -> a, strbuf, strlen(cur2 -> a) - 2);
                cur2 -> a[strlen(cur2 -> a) - 2] = '\0';
                open_dir();         //디렉토리 열기
                cur2 = cur2 -> back;
                snprintf(strbuf,PATH_MAX,"(%.4093s)", cur2 -> a);
                strcpy(cur2 -> a, strbuf);
            }
            break;
        }
        if (ch == EXIT_KEY) //프로그램 종료
            break;
        cur = start -> back;
        read_tree(); // tree view 형식으로 출력
        refresh();
        while((ch=getch())==ERR && time(NULL)-lasttime<3);   //키 입력 될때까지 대기
    }
    endwin();
    return 0;
}