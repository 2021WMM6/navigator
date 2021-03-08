#include "test.h"

int pop(){return top == 0 ? -1 : stack[--top];}
void push(int a){stack[top++] = a;}
int tops(){return top == 0 ? -1 : stack[top - 1];}

linked* search(char name[100]){
    linked *q=start;
/*  while(q->front->op!=1&&q->front!=start)
        q=q->front;*/
    while(q->back!=NULL){
    q=q->back;
//  printw("%s\n",q->a);

    if(strcmp(q->a,name)==0){
        return q;
        }
    }
    refresh();
    return NULL;
}
void get_name(){
        char name1[100];
        printw("| MENU | 1. go back\t2. sorting\t3. searching\n\n");
        printw("%.*s\n",termx-1, tp);     //현재 경로가 어디인지 출력

        print_detail();
        mvprintw(0,80,"--------------------------------------------\n");
        for(int i=1;i<5;i++){
            mvprintw(i,80,"|                                          |");
        }
        mvprintw(5,80,"--------------------------------------------\n");
        mvprintw(1,81,"File Name: ");
        nocbreak();
        echo();
        nodelay(stdscr,FALSE);
        mvscanw(1,92,"%s",name1);

        linked *p=search(name1);
       if(p!=NULL){
                    snprintf(strbuf,PATH_MAX, "%s", cur2 -> a + 1);
                    strncpy(cur2 -> a, strbuf, strlen(cur2 -> a) - 2);
                    cur2 -> a[strlen(cur2 -> a) - 2] = '\0';
                    cur2 = p;
                    snprintf(strbuf,PATH_MAX*2,"(%s)", cur2 -> a);
                    strcpy(cur2 -> a, strbuf);

        mvprintw(3,81,"success!");
        }
        else{
            mvprintw(3,81,"name not found");


        }
    refresh();

    getch();
    nodelay(stdscr,TRUE);
    noecho();
    cbreak();
}
void save_tree(){
    start -> front = NULL;
    start -> back = NULL;
    TAIL = *start;
    end = &TAIL;
    start -> back = end;
    end -> front = start;
    cur = start;
    DIR *dirp;      //디렉토리 포인터
    struct dirent *diritem;    //디렉토리 항목 포인터

    dirp = opendir(wd);
    while((diritem=readdir(dirp)) != NULL){
        if(strncmp(diritem -> d_name, ".", 1) != 0){
            temp = diritem -> d_type;
            char *str = diritem->d_name;
            snprintf(strbuf, PATH_MAX*2, "./%s", str);
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
    p -> list_time = st.st_mtim.tv_sec;
    sprintf(p -> list_change,"%s", ctime(&st.st_mtim));         //수정 시간을 요일/월/일/시간/년 형태로 바꾸어 문자열에 저장
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
void print_tree(){
    int i = 0;
    printw(" --- ");
    getyx(stdscr, y, x); //현재 커서 좌표 구하는 함수
    row = x - 3;
    push(row);      //row정보 stack에 저장
    while(--termy > 0){
        if(cur -> op == 1){             //열린 디렉토리가 있을 때
            if(cur -> front -> op != 1){
                mvprintw(column, row, "\\");
                
                if (cur->type == DT_DIR)
                    printw("+%.*s",termx-1,cur -> a);
                else if (cur->type == DT_REG)
                    printw("-%.*s",termx-1,cur -> a);
            }
            else
                printw("%.*s",termx-1,cur -> a);
            if(cur -> clos == 1)
                check = 1;
            cur = cur -> back;
            print_tree();
        }
        else if(cur -> clos == 1){ //열린 디렉토리의 마지막 파일일 때
            if(i == 0){                 //열린 디렉토리 파일이 하나만 있을 때
                printw("%.*s",termx-1,cur -> a);
                column = y + 1;
            }
            else{
                mvprintw(column, row, "\\");
                
                if (cur->type == DT_DIR)
                    printw("+%.*s",termx-1,cur -> a);
                else if (cur->type == DT_REG)
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
            
            if (cur->type == DT_DIR)
                printw("+%.*s",termx-1,cur -> a);
            else if (cur->type == DT_REG)
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
    snprintf(dp, PATH_MAX*2, "%s/%s", tp, cur -> a);           //dp는 열려고 하는 디렉토리 경로, tp는 현재 디렉토리 경로
    cur -> op = 1;
    DIR *dirp;      //디렉토리 포인터
    struct dirent *diritem;    //디렉토리 항목 포인터
    dirp = opendir(dp);
    strcpy(tp, dp);
    while((diritem=readdir(dirp)) != NULL){
        if(strncmp(diritem -> d_name, ".", 1) != 0){        //.으로 시작하는 파일 제외하고 링크드 리스트에 저장
            temp = diritem -> d_type;
            char *str = diritem->d_name;
            snprintf(strbuf, PATH_MAX*2, "%s/%s",tp, str);
            insert_l(str);
        }
    }
    cur -> clos = 1;
    if(cur == cur2){
        cur -> op = 0;
        cur -> clos = 0;
        int k = strlen(dp) - strlen(cur -> a) - 1;  
        sprintf(tp, "%.*s", k, dp);                               
        strcpy(dp, tp);
    }
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
void sort_type(char ch){
    if(ch == '1'){
        while(cur3 -> back != NULL && cur3 -> front -> clos != 1){
            if(strcmp(least -> a, cur3 -> a) > 0)
                least = cur3;
            cur3 = cur3 -> back;
        }
        if(strcmp(cur -> a, least -> a) != 0)
            change_l();
    }
    else if(ch == '2'){
        while(cur3 -> back != NULL && cur3 -> front -> clos != 1){
            if(least -> list_size > cur3 -> list_size)
                least = cur3;
            cur3 = cur3 -> back;
        }
        if(cur -> list_size != least -> list_size)
            change_l();
    }
    else if(ch == '3'){
        while(cur3 -> back != NULL && cur3 -> front -> clos != 1){
            if(least -> list_time > cur3 -> list_time)
                least = cur3;
            cur3 = cur3 -> back;
        }
        if(cur -> list_time != least -> list_time)
            change_l();
    }
}
void sort(char ch){
    snprintf(strbuf,PATH_MAX, "%s", cur2 -> a + 1);
    strncpy(cur2 -> a, strbuf, strlen(cur2 -> a) - 2);
    cur2 -> a[strlen(cur2 -> a) - 2] = '\0';
    cur = cur2;
    while(cur -> front -> op != 1 && cur -> front != start)
        cur = cur -> front;
    while(cur -> back -> back != NULL && cur -> clos != 1){
        least = cur;
        cur3 = cur -> back;
        sort_type(ch);
        cur = cur -> back;
    }
    snprintf(strbuf,PATH_MAX*2,"(%s)", cur2 -> a);
    strcpy(cur2 -> a, strbuf);
}
void sorting_l(){
    char ch;
    time_t lasttime;
    printw("| MENU | 1. Go Back\t2. Sorting\n\n");
    printw("%.*s\n",termx-1, tp);
    printw("\n1. File Name\n2. Size\n3. Last Modification\n");
    while(1){
        if(ch == '1'){
            sort(ch);
            break;
        }
        else if(ch == '2'){
            sort(ch);
            break;
        }
        else if(ch == '3'){
            sort(ch);
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
    printw("\n%-30s\tFile Type\tSize\tLast Modification\n\n", "File name");
    while(--termy > 0){
        if(cur -> clos == 1 || cur -> back == &TAIL){
            printw("%-30.*s\t%s\t\t%ld\t%s", termx-1, cur -> a, cur->type == 4 ? "Dir" : "Reg", cur -> list_size, cur ->list_change);
            break;
        }
        else{
            printw("%-30.*s\t%s\t\t%ld\t%s", termx-1, cur -> a, cur->type == 4 ? "Dir" : "Reg", cur -> list_size, cur ->list_change);
        }
        cur = cur -> back;
    }
}
void detail(){
    int ch;
    time_t lasttime;
    while(1)    //탈출 조건-> 뒤로가기 눌렀을때.
    {
        getmaxyx(curscr,termy,termx);     //가로세로 구하기
        lasttime=time(NULL);
        clear();
        switch(ch)
        {
            case KEY_DOWN:
            //TODO: 커서 다운
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
            //TODO: 커서 업
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
            sorting_l();        //정렬 기능
            break;
            case '3':
			get_name();
            break;
        }
        printw("| MENU | 1. Go Back\t2. Sorting\t3. Searching\n\n");
        printw("%.*s\n",termx-1, tp);     //현재 경로가 어디인지 출력
        if (ch == '1') //left누르면 tree view로 돌아감
            break;
        cur = start -> back;
        print_detail(); // 현재 디렉토리 안에 있는 파일만 자세하게 출력;
        refresh();
        while((ch=getch())==ERR && time(NULL)-lasttime<3);   //키 입력 될때까지 대기
    }
    clear();
}

int main(){
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
    snprintf(strbuf,PATH_MAX*2,"(%s)", cur2 -> a);
    strcpy(cur2 -> a, strbuf);
    strcpy(tp, wd);         //wd는 최상위 디렉토리 경로로 바뀌지 않음
    while(1)    //탈출 조건-> x눌렀을때.
    {
        getmaxyx(curscr,termy,termx);     //가로세로 구하기
        lasttime=time(NULL);
        clear();
        //printw("%.*s\n",termx-1, tp);     //현재 경로가 어디인지 출력
        switch(ch)
        {
            case KEY_DOWN:
            //TODO: 커서 다운
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
            //TODO: 커서 업
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
            //TODO: 커서 왼쪽(이전 위치)
            if(cur2 -> front -> op != 0){
                close_dir();        //디렉토리 닫기
                snprintf(strbuf,PATH_MAX*2,"(%s)", cur2 -> a);
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
                if(cur2 -> op == 1)
                    cur2 = cur2 -> back;
                snprintf(strbuf,PATH_MAX*2,"(%s)", cur2 -> a);
                strcpy(cur2 -> a, strbuf);
            }
            break;
            case '1':   //1번 누르면 detail 모드로 바뀜
            detail();
            break;
        }
        if (ch == EXIT_KEY) //프로그램 종료
            break;
        printw("| MENU | 1. See Detail\t\tx. Exit\n\n");
        snprintf(strbuf,PATH_MAX,"%s",wd);
        printw("%.*s",termx-1,strbuf);
        cur = start -> back;
        print_tree(); // tree view 형식으로 출력
        refresh();
        while((ch=getch())==ERR && time(NULL)-lasttime<3);   //키 입력 될때까지 대기
    }
    endwin();
    return 0;
}