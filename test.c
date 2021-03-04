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
linked *cur3 = NULL;   //��ũ�� ����Ʈ �����Ҷ� ���� ������
linked *least = NULL;
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
            snprintf(strbuf, PATH_MAX*2, "./%s", str);
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
void change_l(){
    linked *p;
    if(least -> clos == 1){
        least -> clos = 0;
        cur -> clos = 1;
    }
    if(cur -> back == least){
        cur -> front -> back = least;
        least -> back -> front = cur;
        p = cur -> front;
        cur -> front = least;
        least -> front = p;
        p = least -> back;
        least -> back = cur;
        cur -> back = p;
    }
    else{
        cur -> front -> back = least;
        cur -> back -> front = least;
        least -> front -> back = cur;
        least -> back -> front = cur;
        p = cur -> front;
        cur -> front = least -> front;
        least -> front = p;
        p = cur -> back;
        cur -> back = least -> back;
        least -> back = p;
    }
    cur = least;
}
void print_tree()
{
    int i = 0;
    printw(" --- ");
    getyx(stdscr, y, x); //���� Ŀ�� ��ǥ ���ϴ� �Լ�
    row = x - 3;
    push(row);      //row���� stack�� ����
    while(--termy > 0){
        if(cur -> op == 1){             //���� ���丮�� ���� ��
            if(cur -> front -> op != 1){
                mvprintw(column, row, "\\");
                printw("-%.*s",termx-1,cur -> a);
            }
            else
                printw("%.*s",termx-1,cur -> a);
            if(cur -> clos == 1)
                check = 1;
            cur = cur -> back;
            print_tree();
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
    strcpy(tp, dp);
    while((diritem=readdir(dirp)) != NULL){
        if(strncmp(diritem -> d_name, ".", 1) != 0){        //.���� �����ϴ� ���� �����ϰ� ��ũ�� ����Ʈ�� ����
            temp = diritem -> d_type;
            char *str = diritem->d_name;
            snprintf(strbuf, PATH_MAX*2, "%s/%s",tp, str);
            insert_l(str);
        }
    }
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
void sort_n(){
    int ch;
    snprintf(strbuf,PATH_MAX, "%s", cur2 -> a + 1);
    strncpy(cur2 -> a, strbuf, strlen(cur2 -> a) - 2);
    cur2 -> a[strlen(cur2 -> a) - 2] = '\0';
    cur = cur2;
    while(cur -> front -> op != 1 && cur -> front != start)
        cur = cur -> front;
    while(cur -> back -> back != NULL && cur -> clos != 1){
        least = cur;
        cur3 = cur -> back;
        while(cur3 -> back != NULL && cur3 -> front -> clos != 1){
            if(strcmp(least -> a, cur3 -> a) > 0)
                least = cur3;
            cur3 = cur3 -> back;
        }
        if(strcmp(cur -> a, least -> a) != 0)
            change_l();
        cur = cur -> back;
    }
    snprintf(strbuf,PATH_MAX*2,"(%s)", cur2 -> a);
    strcpy(cur2 -> a, strbuf);
}
void sorting_l(){
    char ch;
    time_t lasttime;
    printw("| MENU | 1. go back\t2. sorting\n\n");
    printw("%.*s\n",termx-1, tp);
    printw("\n1. FILE name\n2. SIZE\n3. Last modification\n");
    while(1){
        if(ch == '1'){
            sort_n();
            break;
        }
        else if(ch == '2'){
            break;
        }
        else if(ch == '3'){
            break;
        }
        refresh();
        while((ch=getch())==ERR && time(NULL)-lasttime<3);
    }
    clear();
}
void print_detail(){
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
        switch(ch)
        {
            case KEY_DOWN:
            //TODO: Ŀ�� �ٿ�
            if(cur2 -> back -> back != NULL){
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
            case '2':
            sorting_l();        //���� ���
            break;
            case '3':
            break;
        }
        printw("| MENU | 1. go back\t2. sorting\n\n");
        printw("%.*s\n",termx-1, tp);     //���� ��ΰ� ������� ���
        if (ch == '1') //left������ tree view�� ���ư�
            break;
        cur = start -> back;
        print_detail(); // ���� ���丮 �ȿ� �ִ� ���ϸ� �ڼ��ϰ� ���;
        refresh();
        while((ch=getch())==ERR && time(NULL)-lasttime<3);   //Ű �Է� �ɶ����� ���
    }
    clear();
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
        switch(ch)
        {
            case KEY_DOWN:
            //TODO: Ŀ�� �ٿ�
            if(cur2 -> back -> back != NULL){
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
            case '1':   //1�� ������ detail ���� �ٲ�
            detail();
            break;
        }
        if (ch == EXIT_KEY) //���α׷� ����
            break;
        printw("| MENU | 1. see detail\n\n");
        snprintf(strbuf,PATH_MAX,"%s",wd);
        printw("%.*s",termx-1,strbuf);
        cur = start -> back;
        print_tree(); // tree view �������� ���
        refresh();
        while((ch=getch())==ERR && time(NULL)-lasttime<3);   //Ű �Է� �ɶ����� ���
    }
    endwin();
    return 0;
}