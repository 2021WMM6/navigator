#include "test.h"

int pop(){return top == 0 ? -1 : stack[--top];}
void push(int a){stack[top++] = a;}
int tops(){return top == 0 ? -1 : stack[top - 1];}

void search(char name[100]){
    temp = 0;
    char *str;
    cur = cur2;
    while(cur -> front -> op != 1 && cur -> front != start)
        cur = cur -> front;
    while(cur -> back -> back != NULL && cur -> clos != 1){
        str = cur -> a;
        while(*str){
            if(strncmp(str, name, strlen(name)) == 0){
                temp = 1;
                cur2 = cur;
                cur -> search_list = 1;
                break;
            }
            else{
                str++;
            }
        }
        cur = cur -> back;
    }
}
void select_search_list(){
    int ch;
    time_t lasttime;
    cbreak();   
    noecho();
    nodelay(stdscr,TRUE);
    snprintf(strbuf,PATH_MAX*2,"(%s)", cur2 -> a);
    strcpy(cur2 -> a, strbuf);
    while(1)    //탈출 조건-> 1 눌렀을때.
    {
        getmaxyx(curscr,termy,termx);     //가로세로 구하기
        lasttime=time(NULL);
        switch(ch)
        {
            case KEY_DOWN:
            //TODO: 커서 다운
            if(cur2 -> back -> back != NULL && cur2 -> clos == 0){
                snprintf(strbuf,PATH_MAX, "%s", cur2 -> a + 1);
                strncpy(cur2 -> a, strbuf, strlen(cur2 -> a) - 2);
                cur2 -> a[strlen(cur2 -> a) - 2] = '\0';
                cur = cur2;
                do{
                    cur = cur -> back;
                } while (cur -> search_list != 1 && cur -> back != NULL && cur -> front -> clos == 0);
                if(cur -> back != NULL && cur -> front -> clos == 0)
                    cur2 = cur;
                snprintf(strbuf,PATH_MAX*2,"(%s)", cur2 -> a);
                strcpy(cur2 -> a, strbuf);
            }
            break;
            case KEY_UP:
            //TODO: 커서 업
            if(cur2 -> front != start && cur2 -> front -> op == 0){
                snprintf(strbuf,PATH_MAX, "%s", cur2 -> a + 1);
                strncpy(cur2 -> a, strbuf, strlen(cur2 -> a) - 2);
                cur2 -> a[strlen(cur2 -> a) - 2] = '\0';
                cur = cur2;
                do{
                    cur = cur -> front;
                } while (cur -> search_list != 1 && cur != start && cur -> back -> op == 0);
                if(cur != start && cur -> back -> op == 0)
                    cur2 = cur;
                snprintf(strbuf,PATH_MAX*2,"(%s)", cur2 -> a);
                strcpy(cur2 -> a, strbuf);
            }
            break;
        }
        if (ch == '1')
            break;
        print_search_list();
        refresh();
        while((ch=getch())==ERR && time(NULL)-lasttime<3);   //키 입력 될때까지 대기
    }
    clear();
}
void get_name(){
    char name1[100];
    mvprintw(0,86,"------------------------------------------\n");
    for(int i=1;i<5;i++){
        mvprintw(i,85,"|                                          |");
    }
    mvprintw(5,86,"------------------------------------------\n");
    mvprintw(1,86,"File Name: ");
    nocbreak();
    echo();
    nodelay(stdscr,FALSE);
    mvscanw(1,97,"%s",name1);
    search(name1);
    if(temp == 1){
        select_search_list();
    }
    else{
        mvprintw(3,86,"name not found");
        refresh();
        getch();
        nodelay(stdscr,TRUE);
        noecho();
        cbreak();
        snprintf(strbuf,PATH_MAX*2,"(%s)", cur2 -> a);
        strcpy(cur2 -> a, strbuf);
        return;
    }
}
void print_search_list(){
    int i = 3;
    cur = cur2;
    while(cur -> front -> op != 1 && cur -> front != start)
        cur = cur -> front;
    while(cur -> back != NULL && cur -> front -> clos != 1){
        if(cur -> search_list == 1){
            mvprintw(i,85,"|                                          |");
            mvprintw(i, 86, "%s", cur -> a);
            i++;
        }
        cur = cur -> back;
    }
    mvprintw(i,85,"|                                          |");
    mvprintw(i + 1,86,"------------------------------------------\n");
}
void use_search(){
    int ch;
    time_t lasttime;
    snprintf(strbuf,PATH_MAX, "%s", cur2 -> a + 1);
    strncpy(cur2 -> a, strbuf, strlen(cur2 -> a) - 2);
    cur2 -> a[strlen(cur2 -> a) - 2] = '\0';
    getmaxyx(curscr,termy,termx);     //가로세로 구하기
    lasttime=time(NULL);
    clear();
    printw("| MENU | 1. Go Back\t2. Sorting\t3. Searching\n\n");
    printw("%.*s\n",termx-1, tp);     //현재 경로가 어디인지 출력
    print_detail();
    get_name();
    cur = cur2;
    while(cur -> front -> op != 1 && cur -> front != start)
        cur = cur -> front;
    while(cur -> back -> back != NULL && cur -> clos != 1){
        cur -> search_list = 0;
        cur = cur -> back;
    }
    clear();
}
//precondition: start가 NULL값을 가지면 안 된다. wd가 현재 디렉토리의 경로를 문자열로 가지고 있어야 한다.
//postcondition: 링크드 리스트를 만들고 현재 디렉토리안의 파일들의 정보를 링크드 리스트에 삽입한다.
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
            insert_l(str);
        }
    }
}
//precondition: cur가 start, end를 포인터하고 있으면 안 된다.
//postcondition: 링크드리스트에서 cur가 가리키는 구조체를 삭제
void delete_l(){
    linked *p = cur;
    if(p == start)  return ;
    p -> back -> front = p -> front;
    p -> front -> back = p -> back;
    cur = cur -> front;
    free(p);
}
//precondition: ch는 readdir로 읽은 파일의 이름으로 NULL이면 안 된다.
//postcondition: 동적으로 할당된 메모리에 파일 정보를 저장하여 링크드리스트에 삽입한다.
void insert_l(char *ch){
    linked *p = (linked *)malloc(sizeof(linked));
    strcpy(p -> a, ch);
    p -> type = temp;
    p -> op = 0;
    p -> clos = 0;
    p -> pin = 0;
    p  -> search_list = 0;
    stat(strbuf, &st);
    p -> list_size = st.st_size;
    p -> list_time = st.st_mtim.tv_sec;
    sprintf(p -> list_change,"%s", ctime(&st.st_mtim.tv_sec));         //수정 시간을 요일/월/일/시간/년 형태로 바꾸어 문자열에 저장
    p -> back = cur -> back;
    p -> front = cur;
    cur -> back -> front = p;
    cur -> back = p;
    cur = cur -> back;
}
//precondition: cur와 least가 교환하고자 하는 구조체들을 각각 포인트하고 있어야한다.
//postcondition: cur가 가리키고 있는 구조체와 least가 가리키고 있는 구조체의 순서를 바꾼다.
//               cur3은 교환된 cur의 주소값을 가진고 cur는 교환된 least의 주소값을 가지게 한다.
void change_l(){
    linked *p;
    if(least -> clos == 1){
        least -> clos = 0;
        cur -> clos = 1;
    }
    if(cur -> back == least){           //least가 cur바로 다음에 있는 경우
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
    cur3 = cur;
    cur = least;
}
//precondition: cur은 start -> back의 주소값을 가져야 한다.
//postcondition: 링크드 리스트의 파일들을 tree view형식으로 출력한다.
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
//precondition: cur2는 디렉토리 파일을 포인트하고 있어야 하고, tp는 현재 디렉토리 경로를 문자열로 가지고 있어야 한다.
//postcondition: 디렉토리를 열어서 그 안에 있는 파일들의 정보를 링크드리스트에 저장한다.
//               열기로 한 디렉토리의 op값을 1로 변경, 열어놓은 디렉토리 안의 파일들 중 마지막 파일의 경우 clos값을 1로 저장한다.
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
//precondition:  cur2는 열어놓은 디렉토리 안의 파일들 중 첫번쨰 파일을 포인트하고 있어야한다. 
//               dp는 현재 디렉토리의 경로를 문자열로 가지고 있어야 한다.
//postcondition: 열어놓은 디렉토리의 파일들을 링크드리스에서 삭제하고 열었었던 디렉토리의 op값을 0으로 변경한다.
//               cur2는 닫은 디렉토리를 포인트하고 있고, 바뀐 경로를 tp와 dp에 문자열로 저장한다.
void close_dir(){
    cur = cur2 -> front;
    cur -> op = 0;
    while(1){
        cur = cur -> back;
        if(cur -> clos == 1){
            delete_l();     
            break;
        }
        else
            delete_l();
    }
    cur2 = cur;
    int k = strlen(dp) - strlen(cur -> a) - 1;
    sprintf(tp, "%.*s", k, dp);                                
    strcpy(dp, tp);
}
//precondition: 1 <= ch <= 3 이어야하고 cur는 NULL이거나 정렬하고자 하는 파일들 중 마지막 파일을 포인트하고 있으면 안 된다. 
//              cur3은 cur -> back을 포인트하고 있어야한다.
//postcondition: ch값에 따라 파일이름, 크기, 수정날짜 중 하나를 기준으로 선택하여
//               cur가 포인트하고 있는 파일부터 현재 디렉토리의 마지막 파일까지 중 기준값이 가장 작은 값의 파일이 cur가 가리키는 위치에 들어간다.
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
    else if(ch == '4'){
        while(cur3 -> back != NULL && cur3 -> front -> clos != 1){
            if(least -> type > cur3 -> type)
                least = cur3;
            cur3 = cur3 -> back;
        }
        if(cur -> type != least -> type)
            change_l();
    }
}
//precondition: cur는 현재 디렉토리안의 첫번째 파일을 포인트하고 있어야 한다.
//postcondition: 현재 디렉토리안의 파일들 중 pin으로 고정된 파일들을 맨 앞으로 보내고 cur는 pin으로 고정되지 않은 첫번째 파일을 가리킨다.
void except_pin(){
    cur3 = cur;
    while(cur -> back != NULL && cur -> front -> clos != 1){
        if(cur -> pin == 1){
            cur -> pin = 0;
            least = cur3;
            change_l();
            cur3 = cur3 -> back;
        }
        cur = cur -> back;
    }
    cur = cur3;
}
//precondition: cur2가 NULL값을 가지거나 현재 디렉토리 밖의 파일을 포인트하면 안 된다.
//postcondition: 현재 디렉토리안의 파일을 ch값에 따라 파일이름, 크기, 수정날짜 중 하나를 선택하여 오름차순으로 정렬한다.
void sort(char ch){
    snprintf(strbuf,PATH_MAX, "%s", cur2 -> a + 1);
    strncpy(cur2 -> a, strbuf, strlen(cur2 -> a) - 2);
    cur2 -> a[strlen(cur2 -> a) - 2] = '\0';
    cur = cur2;
    while(cur -> front -> op != 1 && cur -> front != start)
        cur = cur -> front;
    except_pin();
    while(cur -> back -> back != NULL && cur -> clos != 1){
        least = cur;
        cur3 = cur -> back;
        sort_type(ch);
        cur = cur -> back;
    }
    snprintf(strbuf,PATH_MAX*2,"(%s)", cur2 -> a);
    strcpy(cur2 -> a, strbuf);
}
//precondition: tp는 현재 디렉토리의 경로를 문자열로 가지고 있어야 한다.
//postcondition: Sorting을 어떤 기준으로 할 것인지 메뉴를 화면에 출력해주고 ch값을 입력받고 정렬 기능을 실행한다.
void sorting_l(){
    char ch;
    time_t lasttime;
    printw("| MENU | 1. Go Back\t2. Sorting\n\n");
    printw("%.*s\n",termx-1, tp);
    printw("\n1. File Name\n2. Size\n3. Last Modification\n4. File Type\n");
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
        else if(ch == '4'){
            sort(ch);
            break;
        }
        refresh();
        while((ch=getch())==ERR && time(NULL)-lasttime<3);
    }
    clear();
}
//precondition: cur2가 NULL값을 가지거나 현재 디렉토리 밖의 파일을 포인트하면 안 된다.
//postcondition: Pin으로 고정된 파일들의 목록을 detail 옆에 출력해준다.
void print_pin(){
    int i = 3;
    mvprintw(0,86,"------------------------------------------\n");
    for(int i=1;i<3;i++){
        mvprintw(i,85,"|                                          |");
    }
    mvprintw(1,86,"PIN List: ");
    cur = cur2;
    while(cur -> front -> op != 1 && cur -> front != start)
        cur = cur -> front;
    while(cur -> back != NULL && cur -> front -> clos != 1){
        if(cur -> pin == 1){
            mvprintw(i,85,"|                                          |");
            if(cur == cur2){
                snprintf(strbuf,PATH_MAX, "%s", cur2 -> a + 1);
                mvprintw(i, 86, "%.*s",strlen(cur2 -> a) - 2, strbuf);
            }
            else
                mvprintw(i, 86, "%s", cur -> a);
            i++;
        }
        cur = cur -> back;
    }
    mvprintw(i,85,"|                                          |");
    mvprintw(i + 1,86,"------------------------------------------\n");
}
//precondition: cur2가 NULL값을 가지거나 현재 디렉토리 밖의 파일을 포인트하면 안 된다.
//postcondition: 현재 ()로 가리키고 있는 파일들을 p를 눌러서 Pin list에 추가하거나 뺄 수 있고 1누르면 Pin 기능을 종료한다.
void use_pin(){
    int ch;
    time_t lasttime;
    while(1)    //탈출 조건-> 1 눌렀을때.
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
            case 'p':
            if(cur2 -> pin == 1)
                cur2 -> pin = 0;
            else
                cur2 -> pin = 1;
            break;
        }
        printw("| MENU | 1. Quit\n\n");
        printw("|| If you want to use pin, press 'p' ||\n");
        if (ch == '1')
            break;
        cur = start -> back;
        print_detail(); // 현재 디렉토리 안에 있는 파일만 자세하게 출력;
        print_pin();
        refresh();
        while((ch=getch())==ERR && time(NULL)-lasttime<3);   //키 입력 될때까지 대기
    }
    clear();
}
//precondition: tp는 현재 디렉토리의 경로를 문자열로 가지고 있어야 한다.
//postcondition: Pin기능을 사용할 것인지 메뉴로 출력하고 1을 누르면 Pin기능과 정렬 기능을 실행, 2를 누르면 정렬 기능만 실행한다.
void sorting_pin(){
    char ch;
    time_t lasttime;
    printw("| MENU | 1. Go Back\t2. Sorting\n\n");
    printw("%.*s\n",termx-1, tp);
    printw("\n1. Use pin\n2. Don't use pin\n");
    while(1){
        if(ch == '1'){
            clear();
            use_pin();
            clear();
            sorting_l();
            break;
        }
        else if(ch == '2'){
            clear();
            sorting_l();
            break;
        }
        refresh();
        while((ch=getch())==ERR && time(NULL)-lasttime<3);
    }
}
//precondition: cur2가 NULL값을 가지거나 현재 디렉토리 밖의 파일을 포인트하면 안 된다.
//postcondition: 현재 디렉토리의 파일들의 이름, 크기, 수정날짜를 detail하게 화면에 출력한다.
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
//precondition: cur2가 NULL값을 가지거나 현재 디렉토리 밖의 파일을 포인트하면 안 된다. tp는 현재 디렉토리의 경로를 문자열로 가지고 있어야 한다.
//postcondition: 메뉴를 출력하고 ch값에 따라 2를 누르면 정렬 기능을 실행, 3을 누르면 검색기능을 실행하며 1을 누르면 detail 기능을 종료한다.
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
            sorting_pin();        //정렬 기능
            break;
            case '3':
			use_search();
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