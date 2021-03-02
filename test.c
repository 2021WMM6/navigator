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
    char a[PATH_MAX];      //���� �̸�
    int type;               //���� Ÿ��
    int op;                 //���丮�� �� �� ���丮�� ���� 1�� �ǰ� ������ �ٽ� 0�� ��
    int clos;               //� ���丮�� ������ �� �� ���丮�� ������ �����̸� 1�� ����, �� �ܿ��� 0���� ����
    struct list *front;
    struct list *back;
    char list_change[PATH_MAX];         //���� �ð�
    long int list_size;                 //���� ũ��
}linked;
struct stat st;
linked START;
linked TAIL;
linked *start = &START;
linked *end = &TAIL;
linked *cur;     //cur�� ���� ����Ű�� �ִ� ��ũ�� ����Ʈ �ּ� ������ - ��ũ�帮��Ʈ ����, �����ϴ� ���� �Լ��� ���
linked *cur2 = NULL;    //cur2�� cur�� �ϴ� ���� ������ Ű���� ���� ���� ���� ����Ű���ִ� ���Ͽ� ()ǥ���ϱ� ���� ���
int temp = 0;
int x, y, row, column = 0;
int check = 0;          //check�� ���丮�� open���� �� open�� ���丮�� ���� ��ο� �ִ� �ٸ� ���ϵ� �� �������� ���� 1���� ����
void delete_l();
void insert_l(char *);
int stack[10001] = {0};
int top = 0;
int pop(){return top == 0 ? -1 : stack[--top];}
void push(int a){stack[top++] = a;}
int tops(){return top == 0 ? -1 : stack[top - 1];}
void save_tree(){
    start -> front = NULL;
    start -> back = NULL;
    TAIL = *start;
    end = &TAIL;
    start -> back = end;
    end -> front = start;
    cur = start;
    DIR *dirp;      //���丮 ������
    struct dirent *diritem;    //���丮 �׸� ������

    dirp = opendir(wd);
    while((diritem=readdir(dirp)) != NULL){
        if(strncmp(diritem -> d_name, ".", 1) != 0){
            temp = diritem -> d_type;
            char *str = diritem->d_name;
            insert_l(str);      //��ũ�� ����Ʈ�� ������ ����
        }
    }
}
void delete_l(){
    linked *p = cur;
    if(p == start)  return ;
    p -> back -> front = p -> front;
    p -> front -> back = p -> back;
    cur = cur -> front;
    free(p);
}

void insert_l(char *ch){
    linked *p = (linked *)malloc(sizeof(linked));
    snprintf(strbuf, PATH_MAX*2, "./%s", ch);
    strcpy(p -> a, ch);
    p -> type = temp;
    p -> op = 0;
    p -> clos = 0;
    stat(strbuf, &st);
    p -> list_size = st.st_size;
    sprintf(p -> list_change,"%s", ctime(&st.st_mtim));         //���� �ð��� ����/��/��/�ð�/�� ���·� �ٲپ� ���ڿ��� ����
    p -> back = cur -> back;
    p -> front = cur;
    cur -> back -> front = p;
    cur -> back = p;
    cur = cur -> back;
}

void read_tree()
{
    int i = 0;
    printw(" --- ");
    getyx(stdscr, y, x); //���� Ŀ�� ��ǥ ���ϴ� �Լ�
    row = x - 3;
    push(row);      //row���� stack�� ����
    while(--termy > 0){
        if(cur -> op == 1){             //���� ���丮�� ���� ��
            mvprintw(column, row, "\\");
            printw("-%.*s",termx-1,cur -> a);
            if(cur -> clos == 1)
                check = 1;
            cur = cur -> back;
            read_tree();
        }
        else if(cur -> clos == 1){ //���� ���丮�� ������ ������ ��
            if(i == 0){                 //���� ���丮 ������ �ϳ��� ���� ��
                printw("%.*s",termx-1,cur -> a);
                column = y + 1;
            }
            else{
                mvprintw(column, row, "\\");
                printw("-%.*s",termx-1,cur -> a);
                getyx(stdscr, y, x);
                column = y + 1;
            }
            pop();          //���� ���丮���� ���� ���丮�� row������ ���ư��� ����ϱ� ����
            if(check == 1){       //���� ���丮�� ���� ����� ���� �� ������ �������� Ȯ��
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
    snprintf(dp, PATH_MAX*2, "%s/%s", tp, cur -> a);           //dp�� ������ �ϴ� ���丮 ���, tp�� ���� ���丮 ���
    cur -> op = 1;
    DIR *dirp;      //���丮 ������
    struct dirent *diritem;    //���丮 �׸� ������
    dirp = opendir(dp);
    while((diritem=readdir(dirp)) != NULL){
        if(strncmp(diritem -> d_name, ".", 1) != 0){        //.���� �����ϴ� ���� �����ϰ� ��ũ�� ����Ʈ�� ����
            temp = diritem -> d_type;
            char *str = diritem->d_name;
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
            delete_l();     //��ũ�� ����Ʈ���� ����
            break;
        }
        else
            delete_l();
    }
    cur2 = cur;
    int k = strlen(dp) - strlen(cur -> a) - 1;      //���丮�� �ݾұ� ������ ���� ��ο���
    sprintf(tp, "%.*s", k, dp);                     //���丮 �̸� �����Ͽ� ��� �����Ͽ� tp�� ����           
    strcpy(dp, tp);
}
void read_detail(){
    cur = cur2;
    while(cur -> front -> op != 1 && cur -> front != start)
        cur = cur -> front;
    printw("\n%-30s\tSize\tLast modification\n\n", "File name");
    while(--termy > 0){
        if(cur -> clos == 1 || cur -> back == &TAIL){
            printw("%-30.*s\t%ld\t%s", termx-1,cur -> a, cur -> list_size, cur ->list_change);
            break;
        }
        else{
            printw("%-30.*s\t%ld\t%s", termx-1,cur -> a, cur -> list_size, cur ->list_change);
        }
        cur = cur -> back;
    }
}
void detail(){
    int ch;
    time_t lasttime;
    while(1)    //Ż�� ����-> �ڷΰ��� ��������.
    {
        getmaxyx(curscr,termy,termx);     //���μ��� ���ϱ�
        lasttime=time(NULL);
        clear();
        printw("%.*s\n",termx-1, tp);     //���� ��ΰ� ������� ���
        switch(ch)
        {
            case KEY_DOWN:
            //TODO: Ŀ�� �ٿ�
            if(cur2 -> back != &TAIL){
                if(cur2 -> clos == 0){
                    snprintf(strbuf,PATH_MAX, "%s", cur2 -> a + 1);
                    strncpy(cur2 -> a, strbuf, strlen(cur2 -> a) - 2);
                    cur2 -> a[strlen(cur2 -> a) - 2] = '\0';
                    cur2 = cur2 -> back;
                    snprintf(strbuf,PATH_MAX*2,"(%s)", cur2 -> a);
                    strcpy(cur2 -> a, strbuf);
                }
            }
            break;
            case KEY_UP:
            //TODO: Ŀ�� ��
            if(cur2 -> front != start){
                if(cur2 -> front -> op == 0){
                    snprintf(strbuf,PATH_MAX, "%s", cur2 -> a + 1);
                    strncpy(cur2 -> a, strbuf, strlen(cur2 -> a) - 2);
                    cur2 -> a[strlen(cur2 -> a) - 2] = '\0';
                    cur2 = cur2 -> front;
                    snprintf(strbuf,PATH_MAX*2,"(%s)", cur2 -> a);
                    strcpy(cur2 -> a, strbuf);
                }
            }
            break;
        }
        if (ch == KEY_LEFT) //left������ tree view�� ���ư�
            break;
        cur = start -> back;
        read_detail(); // ���� ���丮 �ȿ� �ִ� ���ϸ� �ڼ��ϰ� ���;
        refresh();
        while((ch=getch())==ERR && time(NULL)-lasttime<3);   //Ű �Է� �ɶ����� ���
    }
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
    getcwd(wd,PATH_MAX);    //���� ���丮 ��������
    save_tree(); //���� ���� ��ũ�帮��Ʈ�� ����
    cur2 = end -> front;
    snprintf(strbuf,PATH_MAX*2,"(%s)", cur2 -> a);
    strcpy(cur2 -> a, strbuf);
    strcpy(tp, wd);         //wd�� �ֻ��� ���丮 ��η� �ٲ��� ����
    while(1)    //Ż�� ����-> x��������.
    {
        getmaxyx(curscr,termy,termx);     //���μ��� ���ϱ�
        lasttime=time(NULL);
        clear();
        //printw("%.*s\n",termx-1, tp);     //���� ��ΰ� ������� ���
        printw("| MENU | 1. see detail\n\n");
        snprintf(strbuf,PATH_MAX,"%s",wd);
        printw("%.*s",termx-1,strbuf);
        switch(ch)
        {
            case KEY_DOWN:
            //TODO: Ŀ�� �ٿ�
            if(cur2 -> back != &TAIL){
                if(cur2 -> clos == 0){
                    snprintf(strbuf,PATH_MAX, "%s", cur2 -> a + 1);
                    strncpy(cur2 -> a, strbuf, strlen(cur2 -> a) - 2);
                    cur2 -> a[strlen(cur2 -> a) - 2] = '\0';
                    cur2 = cur2 -> back;
                    snprintf(strbuf,PATH_MAX*2,"(%s)", cur2 -> a);
                    strcpy(cur2 -> a, strbuf);
                }
            }
            break;
            case KEY_UP:
            //TODO: Ŀ�� ��
            if(cur2 -> front != start){
                if(cur2 -> front -> op == 0){
                    snprintf(strbuf,PATH_MAX, "%s", cur2 -> a + 1);
                    strncpy(cur2 -> a, strbuf, strlen(cur2 -> a) - 2);
                    cur2 -> a[strlen(cur2 -> a) - 2] = '\0';
                    cur2 = cur2 -> front;
                    snprintf(strbuf,PATH_MAX*2,"(%s)", cur2 -> a);
                    strcpy(cur2 -> a, strbuf);
                }
            }
            break;
            case KEY_LEFT:
            //TODO: Ŀ�� ����(���� ��ġ)
            if(cur2 -> front -> op != 0){
                close_dir();        //���丮 �ݱ�
                snprintf(strbuf,PATH_MAX*2,"(%s)", cur2 -> a);
                strcpy(cur2 -> a, strbuf);
            }
            break;
            case KEY_RIGHT:
            //TODO: Ŀ�� ������(���丮 ����/���۾���)
            if(cur2 -> type == DT_DIR && cur2 -> op == 0){
                snprintf(strbuf,PATH_MAX, "%s", cur2 -> a + 1);
                strncpy(cur2 -> a, strbuf, strlen(cur2 -> a) - 2);
                cur2 -> a[strlen(cur2 -> a) - 2] = '\0';
                open_dir();         //���丮 ����
                cur2 = cur2 -> back;
                snprintf(strbuf,PATH_MAX*2,"(%s)", cur2 -> a);
                strcpy(cur2 -> a, strbuf);
            }
            break;
        }
        if (ch == EXIT_KEY) //���α׷� ����
            break;
        cur = start -> back;
        read_tree(); // tree view �������� ���
        refresh();
        if(ch == '1')       //1�� ������ detail ���� �ٲ�
            detail();
        while((ch=getch())==ERR && time(NULL)-lasttime<3);   //Ű �Է� �ɶ����� ���
    }
    endwin();
    return 0;
}